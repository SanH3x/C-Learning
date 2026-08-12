#include <iostream>
#include <set>


// Ordenação de valores únicos


int main() {
    std::set<int> ids = {5, 1, 3, 1, 3}; // duplicatas são ignoradas

    for (int id : ids)
        std::cout << id << " ";
    // saida: 1 3 5  (ordenado, sem duplicatas)

    if (ids.find(3) != ids.end())
        std::cout << "\n3 esta presente\n";

    return 0;
}
