#include <iostream>
#include <future>
#include <vector>

int ProcessarPedido(int id){
    return id* 100;
}

int main(){
    std::vector<std::future<int>> resultados;

    for(int i = 1; i <= 5; i++){
        resultados.push_back(std::async(std::launch::async, ProcessarPedido, i));
    }

    for(auto& r: resultados){
        std::cout << "Resultado: " << r.get() << "\n";
    }
    
    return 0;
}
