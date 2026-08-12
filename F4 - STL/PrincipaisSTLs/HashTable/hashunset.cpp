#include <iostream>
#include <unordered_set>


// Utilizado quando os elementos não precisam estar ordenado


int main() {
    std::unordered_set<std::string> emails_cadastrados;
    emails_cadastrados.insert("heitor@email.com");
    emails_cadastrados.insert("outro@email.com");

    if (emails_cadastrados.count("heitor@email.com"))
        std::cout << "Email ja cadastrado\n";

    return 0;
}
