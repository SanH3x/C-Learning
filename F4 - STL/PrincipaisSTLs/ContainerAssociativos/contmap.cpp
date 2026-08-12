#include <iostream>
#include <map>


// Ordenação em chaves únicas e por chave-valor


int main() {
    std::map<std::string, int> estoque;
    estoque["Teclado"] = 15;
    estoque["Mouse"] = 30;
    estoque["Monitor"] = 8;

    for (const auto& [produto, qtd] : estoque)
        std::cout << produto << ": " << qtd << "\n";
    // saida ordenada alfabeticamente pela chave

    return 0;
}
