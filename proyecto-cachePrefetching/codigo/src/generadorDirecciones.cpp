#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct Elemento {
    int fila, columna;
    double valor;
};

int main() {
    std::ifstream archivo("matriz.mtx"); 
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se encontro el archivo matriz.mtx\n";
        return 1;
    }

    std::string linea;
    // Saltar las líneas de comentarios (empiezan con %)
    while (std::getline(archivo, linea)) {
        if (linea.empty()) {
            continue;
        }
        if (linea[0] != '%') {
            break;
        }
    }

    // Extraer las dimensiones (Filas, Columnas, Elementos no nulos)
    int filas = 0, cols = 0, no_nulos = 0;
    std::istringstream iss(linea);
    iss >> filas >> cols >> no_nulos;
    
    std::cout << "Leyendo -> Filas: " << filas << " Cols: " << cols << " Datos: " << no_nulos << "\n";
    // Cargar la matriz en memoria
    std::vector<Elemento> matriz(no_nulos);
    for (int i = 0; i < no_nulos; i++) {
        archivo >> matriz[i].fila >> matriz[i].columna;
        matriz[i].valor = 1.0;
        // Matrix Market empieza a contar desde 1, pero C++ desde 0:
        matriz[i].fila--; 
        matriz[i].columna--;
    }

    std::vector<double> vector_x(cols, 1.5);
    std::vector<double> resultado(filas, 0.0);

    // Esto generará los fallos y aciertos de caché
    for (int i = 0; i < no_nulos; i++) {
        int f = matriz[i].fila;
        int c = matriz[i].columna;
        resultado[f] += matriz[i].valor * vector_x[c];
    }

    std::cout << "Matriz procesada con exito. Trazas generadas.\n";
    return 0;
}