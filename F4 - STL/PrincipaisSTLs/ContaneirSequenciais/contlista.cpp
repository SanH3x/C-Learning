#include <iostream>
#include <list>


// Lista Duplamente Encadeada


int main() {
    std::list<std::string> fila_atendimento = {"Ana", "Bruno", "Carlos"};

    auto it = fila_atendimento.begin();
    std::advance(it, 1); // move o iterador até "Bruno"
    fila_atendimento.insert(it, "Prioritario"); // O(1), sem deslocar elementos

    for (const auto& nome : fila_atendimento)
        std::cout << nome << " ";
    // saida: Ana Prioritario Bruno Carlos

    return 0;
}