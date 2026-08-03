#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;


// Calculadora de média ponderada com a inserção de valores e nomes no CLI


struct Avaliacao {
    string nome;
    double nota;
    double peso;
};

int main() {
    int N;

    cout << "=========================================\n";
    cout << " CALCULADORA DE MEDIA PONDERADA\n";
    cout << "=========================================\n\n";

    cout << "Quantidade de avaliacoes: ";
    cin >> N;

    while (N <= 0) {
        cout << "Digite um valor maior que zero: ";
        cin >> N;
    }

    vector<Avaliacao> avaliacoes(N);

    double somaPesos = 0.0;
    double somaPonderada = 0.0;

    cin.ignore();

    cout << "\nCadastro das avaliacoes\n";
    cout << "-------------------------\n";

    for (int i = 0; i < N; i++) {

        cout << "\nAvaliacao " << i + 1 << endl;

        cout << "Nome: ";
        getline(cin, avaliacoes[i].nome);

        cout << "Nota: ";
        cin >> avaliacoes[i].nota;

        cout << "Peso: ";
        cin >> avaliacoes[i].peso;

        somaPesos += avaliacoes[i].peso;
        somaPonderada += avaliacoes[i].nota * avaliacoes[i].peso;

        cin.ignore();
    }

    if (somaPesos == 0) {
        cout << "\nErro: a soma dos pesos nao pode ser zero.\n";
        return 1;
    }

    double media = somaPonderada / somaPesos;

    cout << "\n=========================================\n";
    cout << "RESUMO\n";
    cout << "=========================================\n";

    cout << fixed << setprecision(2);

    for (const auto &a : avaliacoes) {
        cout << "Nome : " << a.nome << endl;
        cout << "Nota : " << a.nota << endl;
        cout << "Peso : " << a.peso << endl;
        cout << "-------------------------\n";
    }

    cout << "Soma dos pesos: " << somaPesos << endl;
    cout << "Media ponderada: " << media << endl;

    return 0;
}
