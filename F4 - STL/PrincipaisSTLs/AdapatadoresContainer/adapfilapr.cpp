#include <iostream>
#include <queue>


// Fila de Prioridade, retornando o maior valor inicialmente


int main() {
    std::priority_queue<int> fila_prioridade;
    fila_prioridade.push(10);
    fila_prioridade.push(30);
    fila_prioridade.push(20);

    while (!fila_prioridade.empty()) {
        std::cout << fila_prioridade.top() << " "; // sempre o maior primeiro
        fila_prioridade.pop();
    }
    // saida: 30 20 10

    return 0;
}
