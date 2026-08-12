#include <iostream>
#include <deque>


// Fila Dupla, utilizado para inserções/remoções nas extremidades 


int main() {
    std::deque<int> fila;
    fila.push_back(2);
    fila.push_back(3);
    fila.push_front(1); // O(1), diferente do vector

    for (int v : fila)
        std::cout << v << " ";
    // saida: 1 2 3

    return 0;
}
