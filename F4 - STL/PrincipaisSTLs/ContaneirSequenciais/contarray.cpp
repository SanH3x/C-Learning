#include <iostream>
#include <array>


// Array de tamanho fixo


int main() {
    std::array<int, 5> notas = {8, 7, 9, 6, 10};

    std::cout << "Elemento 2: " << notas[2] << "\n";
    std::cout << "Tamanho fixo: " << notas.size() << "\n";
    // notas.push_back(1); // ERRO: array não cresce

    return 0;
}
