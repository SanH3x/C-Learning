#include <iostream>
#include <unordered_map>


// Menos frequente, usado quando possui muitas duplicatas e não precisa de ordem, só de contagem/agrupamento rápido.


int main() {
    std::unordered_multimap<std::string, int> vendas_por_dia;
    vendas_por_dia.insert({"Segunda", 100});
    vendas_por_dia.insert({"Segunda", 150});
    vendas_por_dia.insert({"Terca", 80});

    auto range = vendas_por_dia.equal_range("Segunda");
    for (auto it = range.first; it != range.second; ++it)
        std::cout << it->second << " ";
    // saida: 100 150 (ordem entre eles não garantida)

    return 0;
}
