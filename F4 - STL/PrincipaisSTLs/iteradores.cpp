#include <iostream>
#include <vector>


// Ponte entre containers e algoritmos, permitindo percorrer qualquer container de qualquer estrutura interna


int main() {
    std::vector<int> numeros = {10, 20, 30};

    // Iterador explícito
    for (std::vector<int>::iterator it = numeros.begin(); it != numeros.end(); ++it)
        std::cout << *it << " ";

    // Mesma coisa, mais idiomático (C++11+)
    for (auto it = numeros.begin(); it != numeros.end(); ++it)
        std::cout << *it << " ";

    // Range-based for (mais usado no dia a dia, esconde o iterador)
    for (int n : numeros)
        std::cout << n << " ";

    return 0;
}