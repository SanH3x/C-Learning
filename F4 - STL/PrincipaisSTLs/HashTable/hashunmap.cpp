#include <iostream>
#include <unordered_map>
#include <string>


// Mais otimizado para contagem, cache e lookup rápido


int main() {
    std::string texto = "a b a c b a";
    std::unordered_map<std::string, int> contagem;

    std::string palavra;
    for (char c : texto) {
        if (c == ' ') {
            contagem[palavra]++;
            palavra.clear();
        } else {
            palavra += c;
        }
    }
    contagem[palavra]++;

    for (const auto& [p, qtd] : contagem)
        std::cout << p << ": " << qtd << "\n";
    // a: 3, b: 2, c: 1 (ordem não garantida)

    return 0;
}