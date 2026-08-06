#ifndef REPOSITORIO_H
#define REPOSITORIO_H

#include <vector>

template<typename T>
class Repositorio {
public:
    virtual ~Repositorio() = default;

    virtual void salvar(const T& objeto) = 0;

    virtual std::vector<T> listar() const = 0;

    virtual void limpar() = 0;
};

#endif
