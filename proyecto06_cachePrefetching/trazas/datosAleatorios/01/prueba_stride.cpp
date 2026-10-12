#include <iostream>
#include <vector>

int main() {
    const int TAMANO = 20000;
    std::vector<int> vectorPrueba(TAMANO, 0);
    long long suma = 0;

    // Bucle con salto constante y perfecto (+4 bytes)
    for (int i = 0; i < TAMANO; i++) {
        vectorPrueba[i] = i * 5; 
        suma += vectorPrueba[i]; // Genera lecturas y escrituras predecibles
    }

    std::cout << "Suma de control: " << suma << '\n';
    return 0;
}