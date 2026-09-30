#include <iostream>
#include <fstream>
#include <bitset>
#include "cache.hpp"
#include "motorPrefetch.hpp"
#include "secPrefetch.hpp"
int main() {
    // !--- Probando el motor Prefetch Secuencial ---!
    Prefetch *motorPrefetch;
    motorPrefetch = new PrefetchSec(4, 4, 64, 3, 23, 6);
    Cache miCache(64, 4, 8, motorPrefetch);
    int cont1 = 0, cont2 = 0;
    std::ifstream archIn;
    std::string linea;
    archIn.open("datos/direcciones.txt", std::ios::in);
    if (!archIn.is_open()) {
        std::cout << "Se produjo un error con el archivo de direccioens.\n";
        return 1;
    }
    uint64_t direccion;
    char operacion;
    while (std::getline(archIn, linea)) {
        if (linea[0] == ' ') {
            operacion = linea[1];
            linea = linea.substr(3);
            direccion = std::stoull(linea, nullptr, 16);
            if (operacion == 'S' || operacion == 'L') {
                if (miCache.buscarDato((uint32_t)direccion)) {
                    cont1++;
                } else {
                    cont2++;
                }
            } else {
                if (miCache.buscarDato((uint32_t)direccion)) {
                    cont1++;
                } else {
                    cont2++;
                }
                if (miCache.buscarDato((uint32_t)direccion)) {
                    cont1++;
                } else {
                    cont2++;
                }
            }
        }
    }
    std::cout << "Cantidad de aciertos: " << cont1 << '\n' << "cantidad de fallos: " << cont2 << '\n';
    delete motorPrefetch ;
    return 0;
}