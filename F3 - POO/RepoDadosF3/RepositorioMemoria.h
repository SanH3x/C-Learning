#ifndef REPOSITORIO_MEMORIA_H
#define REPOSITORIO_MEMORIA_H

#include "Repositorio.h"
#include <vector>

template<typename T>
class RepositorioMemoria : public Repositorio<T> {

private:
    std::vector<T> dados;

public:

    void salvar(const T& objeto) override
    {
        dados.push_back(objeto);
    }

    std::vector<T> listar() const override
    {
        return dados;
    }

    void limpar() override
    {
        dados.clear();
    }
};

#endif
