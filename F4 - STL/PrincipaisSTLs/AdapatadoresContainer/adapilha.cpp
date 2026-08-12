#include <iostream>
#include <stack>


// Pilha (Stack)


int main() {
    std::stack<int> pilha;
    pilha.push(1);
    pilha.push(2);
    pilha.push(3);

    while (!pilha.empty()) {
        std::cout << pilha.top() << " "; // acessa o topo
        pilha.pop();                     // remove o topo
    }
    // saida: 3 2 1

    return 0;
}
