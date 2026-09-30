#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>
#include "motorPrefetch.hpp"
// Metodos de la clase base "Prefetch"
Prefetch::Prefetch(uint32_t _tamB) : tamB(_tamB){
    uint32_t temp = MAXPAG;
    bitsOffsetPagina = 0;
    while (temp > 1) {
        temp >>= 1;
        bitsOffsetPagina++;
    }
}

bool Prefetch::limitePagina(uint32_t numPagina, uint32_t dir) {
    return numPagina == (dir >> bitsOffsetPagina);
}

Prefetch::~Prefetch() {
}