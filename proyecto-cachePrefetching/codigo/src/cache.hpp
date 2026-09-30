#ifndef CACHE_H
#define CACHE_H
#include <cstdint>
#include <vector>
#include <string>
#include "motorPrefetch.hpp"

// Estructura para cada linea de cache.
struct linea {
    uint32_t tag = 0;
    int cont = 0;
    bool bitValidez = false;
};

// Estructura para cada via de la cache
struct conjunto {
    std::vector<linea> vias;
};

// Clase para simular la cache.
class Cache {
    private:
        uint32_t tamBloques, numVias, numConjuntos;
        uint32_t bitsOffset, bitsConjunto, bitsTag;
        std::vector<conjunto> conjuntos;
        Prefetch *motorPrefetch;
        void actualizarContadores(const uint32_t conjunto, const int via);
    public:
        // Constructor:
        Cache(uint32_t _tamBloques, uint32_t _numVias, uint32_t _numConjuntos, Prefetch *motor);
        // Modificadores:
        void inicializarCache(const uint32_t numConjuntos, const uint32_t numVias);
        void agregarDato(const uint32_t dir);
        // Observadores:
        bool buscarDato(const uint32_t dir);
        // Destructor:
        ~Cache();
};
#endif