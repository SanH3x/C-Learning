#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>


// Funcionam com todos os container, operando de forma genérica sobre os iteradores


int main() {
    std::vector<int> dados = {5, 2, 8, 1, 9, 3};

    std::sort(dados.begin(), dados.end());               // ordena
    bool existe = std::binary_search(dados.begin(), dados.end(), 8); // busca O(log n) em vetor ordenado
    auto max_it = std::max_element(dados.begin(), dados.end());      // ponteiro para o maior
    int soma = std::accumulate(dados.begin(), dados.end(), 0);       // soma tudo
    int qtd_maiores_5 = std::count_if(dados.begin(), dados.end(),
                                       [](int v) { return v > 5; });

    
    std::cout << "8 existe? " << (existe ? "sim" : "nao") << "\n";                                       
    std::cout << "Maior: " << *max_it << "\n";
    std::cout << "Soma: " << soma << "\n";
    std::cout << "Maiores que 5: " << qtd_maiores_5 << "\n";

    return 0;
}
