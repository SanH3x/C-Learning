#include <iostream>
#include <tuple>


// Semelhante com pair mas com quantidade arbitrárias de valores


int main() {
    std::tuple<std::string, int, double> registro = {"Heitor", 21, 8.7};

    auto [nome, idade, media] = registro; // structured binding, precisa C++17
    std::cout << nome << ", " << idade << " anos, media " << media << "\n";

    return 0;
}
