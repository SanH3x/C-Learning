#include <iostream>
#include <queue>


// FIla (Queue)


int main() {
    std::queue<std::string> atendimento;
    atendimento.push("Ana");
    atendimento.push("Bruno");

    while (!atendimento.empty()) {
        std::cout << "Atendendo: " << atendimento.front() << "\n";
        atendimento.pop();
    }
    // saida: Ana, depois Bruno

    return 0;
}
