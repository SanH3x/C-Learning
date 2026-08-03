#include <iostream>
#include <string>


//Código que utilza 2 a 3 notas de uma aluno para o cálculo da média


double calcularMedia(double a, double b) {
    return (a + b) / 2.0;
}

double calcularMedia(double a, double b, double c) {
    return (a + b + c) / 3.0;
}

int main() {
    std::string nome = "Heitor";
    double nota1 = 5.5, nota2 = 7.5, nota3 = 9.0;
    
    std::cout << "Aluno: " << nome << "\n";
    std::cout << "Media: " << calcularMedia(nota1, nota2) << "\n";
    std::cout << "Media Secundaria: " << calcularMedia(nota1, nota2, nota3) << "\n";

    return 0;
}

