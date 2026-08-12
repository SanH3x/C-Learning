#include <iostream>
#include <forward_list>


// Lista simples, utilizada quando os elementos anteriores não precisam ser recorridos


int main() {
    std::forward_list<int> numeros = {3, 1, 2};
    numeros.push_front(0);
    numeros.sort();

    for (int n : numeros)
        std::cout << n << " ";
    // saida: 0 1 2 3

    return 0;
}