#include <iostream>
#include <memory>


// Implementação de ponteiros com sensores e depois destruição


class Sensor {
    std::string nome;
public:
    Sensor(const std::string& n) : nome(n) {
        std::cout << "Sensor " << nome << " criado\n";
    }
    ~Sensor() {
        std::cout << "Sensor " << nome << " destruido\n";
    }
    void ler() const {
        std::cout << "Lendo dados de " << nome << "\n";
    }
};

int main() {
    // unique_ptr: posse exclusiva, liberado automaticamente
    std::unique_ptr<Sensor> s1 = std::make_unique<Sensor>("Temperatura");
    s1->ler();

    // shared_ptr: posse compartilhada, contagem de referências
    std::shared_ptr<Sensor> s2 = std::make_shared<Sensor>("Pressao");
    {
        std::shared_ptr<Sensor> s3 = s2; // conta = 2
        s3->ler();
    } // s3 sai de escopo, conta = 1

    return 0;
} // s1 e s2 destruidos automaticamente aqui