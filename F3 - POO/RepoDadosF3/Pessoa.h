#ifndef PESSOA_H
#define PESSOA_H

#include <iostream>
#include <string>

class Pessoa {
private:
    int id;
    std::string nome;

public:
    Pessoa() = default;

    Pessoa(int id, const std::string& nome)
        : id(id), nome(nome) {}

    int getId() const {
        return id;
    }

    std::string getNome() const {
        return nome;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const Pessoa& p)
    {
        os << p.id << "," << p.nome;
        return os;
    }
};

#endif
