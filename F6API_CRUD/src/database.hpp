#pragma once

#include <sqlite3.h>

#include <mutex>
#include <optional>
#include <string>
#include <vector>

struct Task {
    long long id = 0;
    std::string title;
    std::string description;
    bool done = false;
    std::string created_at;  // ISO-8601 UTC, ex.: 2026-11-09T20:15:00Z
};

// Campos opcionais: usado tanto em PUT (substituição) quanto em PATCH (parcial).
struct TaskPatch {
    std::optional<std::string> title;
    std::optional<std::string> description;
    std::optional<bool> done;
};

// Camada de acesso ao SQLite. Uma conexão única protegida por mutex,
// com prepared statements (sem concatenar SQL => sem SQL injection).
class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    Task create(const std::string& title, const std::string& description, bool done = false);
    std::vector<Task> list(std::optional<bool> done, int limit, int offset);
    std::optional<Task> get(long long id);
    std::optional<Task> update(long long id, const TaskPatch& patch);
    bool remove(long long id);

private:
    void exec(const char* sql);
    std::optional<Task> get_unlocked(long long id);

    sqlite3* db_ = nullptr;
    std::mutex mutex_;
};
