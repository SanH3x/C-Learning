#include <iostream>

#include "Pessoa.h"
#include "RepositorioArquivo.h"
#include "RepositorioMemoria.h"

int main()
{
    Pessoa p1(1, "Joao");
    Pessoa p2(2, "Maria");
    Pessoa p3(3, "Pedro");

    Repositorio<Pessoa>* memoria =
        new RepositorioMemoria<Pessoa>();

    memoria->salvar(p1);
    memoria->salvar(p2);
    memoria->salvar(p3);

    std::cout << "=== Memoria ===\n";

    for (const auto& p : memoria->listar())
    {
        std::cout
            << p.getId()
            << " "
            << p.getNome()
            << std::endl;
    }

    delete memoria;

    std::cout << "\n";

    Repositorio<Pessoa>* arquivo =
        new RepositorioArquivo<Pessoa>("dados.txt");

    arquivo->limpar();

    arquivo->salvar(p1);
    arquivo->salvar(p2);
    arquivo->salvar(p3);

    std::cout << "=== Arquivo ===\n";

    arquivo->listar();

    delete arquivo;

    return 0;
}