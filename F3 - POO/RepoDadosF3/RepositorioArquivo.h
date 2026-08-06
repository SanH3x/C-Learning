#ifndef REPOSITORIO_ARQUIVO_H
#define REPOSITORIO_ARQUIVO_H

#include "Repositorio.h"

#include <fstream>
#include <sstream>
#include <string>

template<typename T>
class RepositorioArquivo : public Repositorio<T> {

private:

    std::string nomeArquivo;

public:

    RepositorioArquivo(const std::string& arquivo)
        : nomeArquivo(arquivo)
    {
    }

    void salvar(const T& objeto) override
    {
        std::ofstream out(nomeArquivo, std::ios::app);

        out << objeto << std::endl;
    }

    std::vector<T> listar() const override
    {
        std::vector<T> lista;

        std::ifstream in(nomeArquivo);

        std::string linha;

        while (getline(in, linha))
        {
            std::cout << linha << std::endl;
        }

        return lista;
    }

    void limpar() override
    {
        std::ofstream out(nomeArquivo);

        out.close();
    }
};

#endif