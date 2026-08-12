#include <iostream>
#include <vector>


// Array Dinâmico baseado em vetor


int main() {
    std::vector<int> notas = {8, 7, 9};
    notas.push_back(10);          // adiciona no final
    notas.insert(notas.begin(), 6); // insere no início (O(n), custoso)

    for (int n : notas)
        std::cout << n << " ";
    // saida: 6 8 7 9 10

    std::cout << "\nTamanho: " << notas.size() << "\n";
    return 0;
}
