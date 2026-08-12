#include <iostream>
#include <map>


// Semelhante com o Map mas com múltiplos valores


int main() {
    std::multimap<std::string, std::string> professores_disciplinas;
    professores_disciplinas.insert({"Joao", "Calculo I"});
    professores_disciplinas.insert({"Joao", "Calculo II"});
    professores_disciplinas.insert({"Maria", "Programacao"});

    auto range = professores_disciplinas.equal_range("Joao");
    for (auto it = range.first; it != range.second; ++it)
        std::cout << it->second << "\n";
    // saida: Calculo I, Calculo II

    return 0;
}
