#include <iostream>
#include <optional>
#include <vector>
#include <algorithm>

std::optional<double> calcularDesconto(double preco, double percentual) {
    if (percentual < 0 || percentual > 100)
        return std::nullopt; // ausencia de valor, sem exceção
    return preco * (1 - percentual / 100);
}

int main() {
    auto resultado = calcularDesconto(100.0, 15.0);

    if (resultado) {
        std::cout << "Preco com desconto: " << *resultado << "\n";
    } else {
        std::cout << "Percentual invalido\n";
    }

    // Lambda com captura, usada em algoritmo
    std::vector<int> precos = {50, 200, 75, 300, 120};
    int limite = 100;
    auto acimaDoLimite = std::count_if(precos.begin(), precos.end(),
        [limite](int p) { return p > limite; });

    std::cout << "Precos acima de " << limite << ": " << acimaDoLimite << "\n";

    return 0;
}
