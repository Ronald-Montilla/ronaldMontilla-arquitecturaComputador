#include <iostream>
#include <vector>

int main() {
    // Dimensiones de la matriz
    const int FILAS = 1024;
    const int COLUMNAS = 32; 
    
    std::vector<int> matriz(FILAS * COLUMNAS, 1);
    long long suma = 0;

    std::cout << "Iniciando recorrido por columnas (Patron Stride)...\n";

    // Esto obliga a la CPU a saltar de fila en fila.
    for (int j = 0; j < COLUMNAS; j++) {
        for (int i = 0; i < FILAS; i++) {
            // Como cada entero ocupa 4 bytes, el salto en memoria física 
            // entre iteraciones será exactamente de: COLUMNAS * 4 = 128 bytes.
            suma += matriz[i * COLUMNAS + j];
        }
    }

    std::cout << "Suma total: " << suma << "\n";
    std::cout << "Traza de salto constante generada con exito.\n";

    return 0;
}