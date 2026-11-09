#include "crow.h"

int main()
{
    crow::SimpleApp app;

    // --- Rota simples, sem parametros ---
    // Acesse em: http://localhost:8080/saude
    CROW_ROUTE(app, "/saude")
    ([]() {
        return "API no ar\n";
    });

    // --- Rota com parametro na URL ---
    // Acesse em: http://localhost:8080/usuarios/1
    // O <int> na rota captura um numero da URL e injeta como argumento
    // na funcao lambda, na mesma ordem em que aparece.
    CROW_ROUTE(app, "/usuarios/<int>")
    ([](int id) {
        crow::json::wvalue resposta;
        resposta["id"] = id;
        resposta["nome"] = "Heitor";
        resposta["status"] = "ativo";
        return resposta; // Crow converte automaticamente para JSON
    });

    // --- Rota que le query string ---
    // Acesse em: http://localhost:8080/buscar?termo=cpp
    CROW_ROUTE(app, "/buscar")
    ([](const crow::request& req) {
        auto termo = req.url_params.get("termo");

        crow::json::wvalue resposta;
        if (termo == nullptr)
        {
            resposta["erro"] = "parametro 'termo' e obrigatorio";
            return crow::response(400, resposta);
        }

        resposta["termo_buscado"] = termo;
        resposta["resultados"] = std::vector<std::string>{"resultado1", "resultado2"};
        return crow::response(200, resposta);
    });

    // --- Rota POST, recebendo JSON no corpo da requisicao ---
    // Teste com curl:
    // curl -X POST http://localhost:8080/usuarios -H "Content-Type: application/json" -d "{\"nome\":\"Ana\"}"
    CROW_ROUTE(app, "/usuarios").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto corpo = crow::json::load(req.body);

        if (!corpo)
        {
            crow::json::wvalue erro;
            erro["erro"] = "JSON invalido no corpo da requisicao";
            return crow::response(400, erro);
        }

        if (!corpo.has("nome"))
        {
            crow::json::wvalue erro;
            erro["erro"] = "campo 'nome' e obrigatorio";
            return crow::response(422, erro);
        }

        crow::json::wvalue resposta;
        resposta["mensagem"] = "usuario criado";
        resposta["nome"] = corpo["nome"].s();
        return crow::response(201, resposta); // 201 = Created
    });

    // multithreaded() usa varias threads para atender requisicoes em paralelo
    // port(8080) define a porta -- acesse tudo em http://localhost:8080
    app.port(8080).multithreaded().run();

    return 0;
}
