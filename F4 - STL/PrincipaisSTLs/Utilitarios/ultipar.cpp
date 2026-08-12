#include <iostream>
#include <utility>


// Agrupar dois valores de  tipos distintos


int main() {
    std::pair<std::string, int> aluno = {"Heitor", 2024};
    std::cout << aluno.first << " - " << aluno.second << "\n";

    return 0;
}
