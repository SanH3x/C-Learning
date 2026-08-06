#include <iostream>
#include <vector>
#include <memory>


//POO e abstração de um NoSQL (Postgres) e SQL


class Conexao {
public:
    virtual void executar(const std::string& query) = 0;
    virtual ~Conexao() = default;
};

class ConexaoPostgres : public Conexao {
public:
    void executar(const std::string& query) override {
        std::cout << "[Postgres] " << query << "\n";
    }
};

class ConexaoSQLite : public Conexao {
public:
    void executar(const std::string& query) override {
        std::cout << "[SQLite] " << query << "\n";
    }
};

// Template genérico
template <typename T>
T maiorValor(const std::vector<T>& valores) {
    T maior = valores.front();
    for (const auto& v : valores)
        if (v > maior) maior = v;
    return maior;
}

int main() {
    std::vector<std::unique_ptr<Conexao>> conexoes;
    conexoes.push_back(std::make_unique<ConexaoPostgres>());
    conexoes.push_back(std::make_unique<ConexaoSQLite>());

    for (auto& c : conexoes)
        c->executar("SELECT * FROM usuarios");

    std::vector<int> notas = {7, 9, 5, 10, 8};
    std::cout << "Maior nota: " << maiorValor(notas) << "\n";

    return 0;
}