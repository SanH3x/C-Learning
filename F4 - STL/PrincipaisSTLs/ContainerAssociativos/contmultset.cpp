#include <iostream>
#include <set>


// Ordenação de valores únicos e duplicados  


int main() {
    std::multiset<int> notas = {7, 8, 7, 9, 8, 7};

    std::cout << "Quantidade de 7s: " << notas.count(7) << "\n"; // 3

    for (int n : notas)
        std::cout << n << " ";
    // saida: 7 7 7 8 8 9

    return 0;
}
