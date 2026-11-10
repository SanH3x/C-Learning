#include "database.hpp"

#include <stdexcept>

namespace {

constexpr const char* kColumns = "id, title, description, done, created_at";

// RAII para sqlite3_stmt: garante sqlite3_finalize mesmo com exceções.
class Statement {
public:
    Statement(sqlite3* db, const char* sql) : db_(db) {
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt_, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("SQL inválido: ") + sqlite3_errmsg(db_));
        }
    }
    ~Statement() { sqlite3_finalize(stmt_); }

    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    void bind_null(int i) { check(sqlite3_bind_null(stmt_, i)); }
    void bind_int64(int i, long long v) { check(sqlite3_bind_int64(stmt_, i, v)); }
    void bind_text(int i, const std::string& v) {
        check(sqlite3_bind_text(stmt_, i, v.c_str(), static_cast<int>(v.size()), SQLITE_TRANSIENT));
    }

    // true = há uma linha disponível; false = terminou (SQLITE_DONE).
    bool step() {
        const int rc = sqlite3_step(stmt_);
        if (rc == SQLITE_ROW) return true;
        if (rc == SQLITE_DONE) return false;
        throw std::runtime_error(std::string("Erro no SQLite: ") + sqlite3_errmsg(db_));
    }

    long long column_int64(int i) const { return sqlite3_column_int64(stmt_, i); }
    std::string column_text(int i) const {
        const unsigned char* p = sqlite3_column_text(stmt_, i);
        return p ? std::string(reinterpret_cast<const char*>(p)) : std::string();
    }

private:
    void check(int rc) const {
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro no bind: ") + sqlite3_errmsg(db_));
        }
    }

    sqlite3* db_;
    sqlite3_stmt* stmt_ = nullptr;
};

Task row_to_task(const Statement& s) {
    Task t;
    t.id = s.column_int64(0);
    t.title = s.column_text(1);
    t.description = s.column_text(2);
    t.done = s.column_int64(3) != 0;
    t.created_at = s.column_text(4);
    return t;
}

}  // namespace

Database::Database(const std::string& path) {
    const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;
    if (sqlite3_open_v2(path.c_str(), &db_, flags, nullptr) != SQLITE_OK) {
        const std::string msg = db_ ? sqlite3_errmsg(db_) : "sem memória";
        sqlite3_close(db_);
        throw std::runtime_error("Falha ao abrir o banco '" + path + "': " + msg);
    }

    try {
        sqlite3_busy_timeout(db_, 5000);
        exec("PRAGMA journal_mode = WAL;");
        exec("PRAGMA foreign_keys = ON;");
        exec(R"sql(
            CREATE TABLE IF NOT EXISTS tasks (
                id          INTEGER PRIMARY KEY AUTOINCREMENT,
                title       TEXT    NOT NULL,
                description TEXT    NOT NULL DEFAULT '',
                done        INTEGER NOT NULL DEFAULT 0 CHECK (done IN (0, 1)),
                created_at  TEXT    NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%SZ', 'now'))
            );
        )sql");
    } catch (...) {
        sqlite3_close(db_);
        throw;
    }
}

Database::~Database() { sqlite3_close(db_); }

void Database::exec(const char* sql) {
    char* err = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &err) != SQLITE_OK) {
        const std::string msg = err ? err : "erro desconhecido";
        sqlite3_free(err);
        throw std::runtime_error("Falha ao executar SQL: " + msg);
    }
}

std::optional<Task> Database::get_unlocked(long long id) {
    const std::string sql = std::string("SELECT ") + kColumns + " FROM tasks WHERE id = ?1;";
    Statement st(db_, sql.c_str());
    st.bind_int64(1, id);
    if (!st.step()) return std::nullopt;
    return row_to_task(st);
}

Task Database::create(const std::string& title, const std::string& description, bool done) {
    std::lock_guard<std::mutex> lock(mutex_);
    {
        Statement st(db_, "INSERT INTO tasks (title, description, done) VALUES (?1, ?2, ?3);");
        st.bind_text(1, title);
        st.bind_text(2, description);
        st.bind_int64(3, done ? 1 : 0);
        st.step();
    }
    const long long id = sqlite3_last_insert_rowid(db_);
    return *get_unlocked(id);
}

std::vector<Task> Database::list(std::optional<bool> done, int limit, int offset) {
    std::lock_guard<std::mutex> lock(mutex_);
    const std::string sql = std::string("SELECT ") + kColumns +
                            " FROM tasks WHERE (?1 IS NULL OR done = ?1)"
                            " ORDER BY id LIMIT ?2 OFFSET ?3;";
    Statement st(db_, sql.c_str());
    if (done) {
        st.bind_int64(1, *done ? 1 : 0);
    } else {
        st.bind_null(1);
    }
    st.bind_int64(2, limit);
    st.bind_int64(3, offset);

    std::vector<Task> out;
    while (st.step()) out.push_back(row_to_task(st));
    return out;
}

std::optional<Task> Database::get(long long id) {
    std::lock_guard<std::mutex> lock(mutex_);
    return get_unlocked(id);
}

std::optional<Task> Database::update(long long id, const TaskPatch& patch) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto current = get_unlocked(id);
    if (!current) return std::nullopt;

    if (patch.title) current->title = *patch.title;
    if (patch.description) current->description = *patch.description;
    if (patch.done) current->done = *patch.done;

    {
        Statement st(db_, "UPDATE tasks SET title = ?1, description = ?2, done = ?3 WHERE id = ?4;");
        st.bind_text(1, current->title);
        st.bind_text(2, current->description);
        st.bind_int64(3, current->done ? 1 : 0);
        st.bind_int64(4, id);
        st.step();
    }
    return current;
}

bool Database::remove(long long id) {
    std::lock_guard<std::mutex> lock(mutex_);
    Statement st(db_, "DELETE FROM tasks WHERE id = ?1;");
    st.bind_int64(1, id);
    st.step();
    return sqlite3_changes(db_) > 0;
}
