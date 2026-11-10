#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "crow.h"
#include "database.hpp"

namespace {

constexpr std::size_t kMaxTitle = 200;
constexpr std::size_t kMaxDescription = 2000;

// ---------- helpers de resposta ----------

crow::response json_response(int code, const crow::json::wvalue& body) {
    crow::response res(code, body.dump());
    res.set_header("Content-Type", "application/json");
    return res;
}

crow::response json_error(int code, const std::string& message) {
    crow::json::wvalue body;
    body["error"] = message;
    return json_response(code, body);
}

crow::json::wvalue to_json(const Task& t) {
    crow::json::wvalue w;
    w["id"] = t.id;
    w["title"] = t.title;
    w["description"] = t.description;
    w["done"] = t.done;
    w["created_at"] = t.created_at;
    return w;
}

// ---------- helpers de parsing/validação ----------

std::string trim(std::string s) {
    const auto not_space = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
    return s;
}

std::optional<int> parse_int(const char* text) {
    if (!text) return std::nullopt;
    const std::string_view sv(text);
    int value = 0;
    const auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec != std::errc() || ptr != sv.data() + sv.size()) return std::nullopt;
    return value;
}

struct ParsedPatch {
    TaskPatch patch;
    std::string error;  // vazio = ok
};

ParsedPatch parse_patch(const crow::json::rvalue& body) {
    ParsedPatch out;
    if (body.t() != crow::json::type::Object) {
        out.error = "O corpo deve ser um objeto JSON";
        return out;
    }

    if (body.has("title")) {
        if (body["title"].t() != crow::json::type::String) {
            out.error = "'title' deve ser uma string";
            return out;
        }
        std::string title = trim(body["title"].s());
        if (title.empty() || title.size() > kMaxTitle) {
            out.error = "'title' deve ter entre 1 e " + std::to_string(kMaxTitle) + " caracteres";
            return out;
        }
        out.patch.title = std::move(title);
    }

    if (body.has("description")) {
        if (body["description"].t() != crow::json::type::String) {
            out.error = "'description' deve ser uma string";
            return out;
        }
        std::string description = body["description"].s();
        if (description.size() > kMaxDescription) {
            out.error = "'description' excede " + std::to_string(kMaxDescription) + " caracteres";
            return out;
        }
        out.patch.description = std::move(description);
    }

    if (body.has("done")) {
        const auto type = body["done"].t();
        if (type != crow::json::type::True && type != crow::json::type::False) {
            out.error = "'done' deve ser booleano";
            return out;
        }
        out.patch.done = (type == crow::json::type::True);
    }
    return out;
}

const char* env_or(const char* name, const char* fallback) {
    const char* v = std::getenv(name);
    return (v && *v) ? v : fallback;
}

}  // namespace

int main() {
    const std::string db_path = env_or("DB_PATH", "tasks.db");
    const std::string host = env_or("HOST", "0.0.0.0");
    const int port = parse_int(env_or("PORT", "8080")).value_or(8080);

    try {
        Database db(db_path);
        crow::SimpleApp app;

        // GET /health ---------------------------------------------------------
        CROW_ROUTE(app, "/health")([] {
            crow::json::wvalue body;
            body["status"] = "ok";
            return json_response(200, body);
        });

        // /tasks  (GET = listar, POST = criar) --------------------------------
        CROW_ROUTE(app, "/tasks").methods("GET"_method, "POST"_method)(
            [&db](const crow::request& req) -> crow::response {
                if (req.method == "POST"_method) {
                    const auto body = crow::json::load(req.body);
                    if (!body) return json_error(400, "JSON inválido");

                    auto parsed = parse_patch(body);
                    if (!parsed.error.empty()) return json_error(400, parsed.error);
                    if (!parsed.patch.title) return json_error(400, "'title' é obrigatório");

                    const Task result = db.create(*parsed.patch.title,
                                                  parsed.patch.description.value_or(""),
                                                  parsed.patch.done.value_or(false));

                    auto res = json_response(201, to_json(result));
                    res.set_header("Location", "/tasks/" + std::to_string(result.id));
                    return res;
                }

                // GET: filtros e paginação via query string
                std::optional<bool> done;
                if (const char* d = req.url_params.get("done")) {
                    const std::string_view v(d);
                    if (v == "true" || v == "1") done = true;
                    else if (v == "false" || v == "0") done = false;
                    else return json_error(400, "'done' deve ser true ou false");
                }

                const auto limit = req.url_params.get("limit") ? parse_int(req.url_params.get("limit"))
                                                               : std::optional<int>(50);
                const auto offset = req.url_params.get("offset") ? parse_int(req.url_params.get("offset"))
                                                                 : std::optional<int>(0);
                if (!limit || *limit < 1 || *limit > 200)
                    return json_error(400, "'limit' deve estar entre 1 e 200");
                if (!offset || *offset < 0)
                    return json_error(400, "'offset' deve ser >= 0");

                crow::json::wvalue::list items;
                for (const auto& t : db.list(done, *limit, *offset)) items.push_back(to_json(t));

                crow::json::wvalue body;
                body["count"] = items.size();
                body["items"] = std::move(items);
                return json_response(200, body);
            });

        // /tasks/<id>  (GET, PUT, PATCH, DELETE) -------------------------------
        CROW_ROUTE(app, "/tasks/<int>")
            .methods("GET"_method, "PUT"_method, "PATCH"_method, "DELETE"_method)(
                [&db](const crow::request& req, int id) -> crow::response {
                    if (id < 1) return json_error(400, "ID inválido");

                    if (req.method == "GET"_method) {
                        const auto task = db.get(id);
                        if (!task) return json_error(404, "Tarefa não encontrada");
                        return json_response(200, to_json(*task));
                    }

                    if (req.method == "DELETE"_method) {
                        if (!db.remove(id)) return json_error(404, "Tarefa não encontrada");
                        return crow::response(204);
                    }

                    // PUT / PATCH
                    const auto body = crow::json::load(req.body);
                    if (!body) return json_error(400, "JSON inválido");

                    const auto parsed = parse_patch(body);
                    if (!parsed.error.empty()) return json_error(400, parsed.error);

                    TaskPatch patch = parsed.patch;
                    if (req.method == "PUT"_method) {
                        // PUT = substituição completa: 'title' obrigatório e
                        // campos omitidos voltam ao valor padrão.
                        if (!patch.title) return json_error(400, "'title' é obrigatório no PUT");
                        if (!patch.description) patch.description = "";
                        if (!patch.done) patch.done = false;
                    } else if (!patch.title && !patch.description && !patch.done) {
                        return json_error(400, "Informe ao menos um campo para atualizar");
                    }

                    const auto updated = db.update(id, patch);
                    if (!updated) return json_error(404, "Tarefa não encontrada");
                    return json_response(200, to_json(*updated));
                });

        // 404 em JSON para rotas inexistentes
        CROW_CATCHALL_ROUTE(app)([](const crow::request&) { return json_error(404, "Rota não encontrada"); });

        CROW_LOG_INFO << "Banco: " << db_path;
        app.bindaddr(host).port(static_cast<std::uint16_t>(port)).multithreaded().run();
    } catch (const std::exception& e) {
        std::cerr << "Erro fatal: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
