#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>


int main() {
    std::vector<int> vendas = {120, 85, 200, 150, 90};

    // Soma total com accumulate
    int total = std::accumulate(vendas.begin(), vendas.end(), 0);

    // Ordenar decrescente
    std::sort(vendas.begin(), vendas.end(), std::greater<int>());

    // Transformar (aplicar imposto de 10%)
    std::vector<double> comImposto(vendas.size());
    std::transform(vendas.begin(), vendas.end(), comImposto.begin(),
                    [](int v) { return v * 1.10; });

    // Agrupar por categoria com unordered_map
    std::unordered_map<std::string, int> vendasPorCategoria = {
        {"Eletronicos", 500}, {"Livros", 200}, {"Roupas", 350}
    };

    std::cout << "Total: " << total << "\n";
    for (const auto& [categoria, valor] : vendasPorCategoria)
        std::cout << categoria << ": " << valor << "\n";

    return 0;
}
