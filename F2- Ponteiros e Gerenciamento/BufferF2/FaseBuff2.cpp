#include <iostream>
#include <algorithm>


// Class Buffer com array dinâmico em CRUD


class Buffer {
private:
    size_t size;
    int* data;

public:
    // Construtor padrão
    Buffer() : size(0), data(nullptr) {}

    // Construtor parametrizado
    Buffer(size_t n) : size(n), data(new int[n]) {
        std::fill(data, data + size, 0);
        std::cout << "Construtor\n";
    }

    // Destrutor
    ~Buffer() {
        delete[] data;
        std::cout << "Destrutor\n";
    }

    // Construtor de cópia
    Buffer(const Buffer& other)
        : size(other.size), data(new int[other.size]) {

        std::copy(other.data, other.data + size, data);
        std::cout << "Construtor de copia\n";
    }

    // Operador de atribuição por cópia
    Buffer& operator=(const Buffer& other) {

        if (this != &other) {
            delete[] data;

            size = other.size;
            data = new int[size];

            std::copy(other.data, other.data + size, data);
        }

        std::cout << "Atribuicao por copia\n";
        return *this;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept
        : size(other.size), data(other.data) {

        other.size = 0;
        other.data = nullptr;

        std::cout << "Move constructor\n";
    }

    // Operador de atribuição por movimento
    Buffer& operator=(Buffer&& other) noexcept {

        if (this != &other) {

            delete[] data;

            size = other.size;
            data = other.data;

            other.size = 0;
            other.data = nullptr;
        }

        std::cout << "Move assignment\n";
        return *this;
    }

    void set(size_t index, int value) {
        if (index < size)
            data[index] = value;
    }

    int get(size_t index) const {
        return (index < size) ? data[index] : -1;
    }

    size_t getSize() const {
        return size;
    }
};

Buffer criarBuffer() {
    Buffer temp(10);
    temp.set(0, 42);
    return temp;    // Move constructor (ou RVO)
}

int main() {

    Buffer a(5);

    a.set(0, 10);
    a.set(1, 20);

    Buffer b = a;               // Cópia

    Buffer c = criarBuffer();   // Movimento (ou RVO)

    Buffer d;
    d = c;                      // Atribuição por cópia

    Buffer e;
    e = std::move(c);           // Movimento

    std::cout << b.get(0) << '\n';
    std::cout << e.get(0) << '\n';

    return 0;
}
