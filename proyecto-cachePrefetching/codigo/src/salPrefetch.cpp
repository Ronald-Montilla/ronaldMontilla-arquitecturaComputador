//!--- En desarrollo ---!
#include <iostream>
#include <cstdint>
#include <vector>
#include "salPrefetch.hpp"

// Constructor:
PrefetchSal::PrefetchSal(uint32_t _numLineas, uint32_t _tamB) 
: Prefetch(_tamB), numLineas(_numLineas){
    tablaRPT.resize(_numLineas);
}

void PrefetchSal::actualizarCont(uint32_t linea) {
    uint32_t contLinea = tablaRPT[linea].cont;
    for (int i = 0; i < numLineas; i++) {
        if (tablaRPT[i].validez && i != linea && tablaRPT[i].cont <= contLinea) {
            tablaRPT[i].cont++;
        }
    }
    tablaRPT[linea].cont = 0;
}

uint32_t PrefetchSal::buscarLineaLRU() {
    int mayor = 0, indice = 0;
    for (int i = 0; i < numLineas; i++) {
        if (!tablaRPT[i].validez) {
            return i;
        } else if (tablaRPT[i].cont >= mayor) {
            mayor = tablaRPT[i].cont;
            indice = i;
        }
    }
    return indice;
}

bool PrefetchSal::logicaPrefetch(uint32_t pc, uint32_t dir) {
    // Determino si el pc ya se encuentra registrado:
    for (int i = 0; i < numLineas; i++) {
        if (tablaRPT[i].validez && tablaRPT[i].tag == pc) {
            actualizarCont(i);
            if (tablaRPT[i].confidence == 0) { // Esta en estado "initial".
                tablaRPT[i].lastStride = dir - tablaRPT[i].lastAddress;
                tablaRPT[i].lastAddress = dir;
                tablaRPT[i].confidence = 1; // Pasa a estado transient.
                return false;
            } else { //Esta en estado "transient" o "confident"
                uint32_t proximaDir = tablaRPT[i].lastAddress + tablaRPT[i].lastStride;
                if (dir == proximaDir) {
                    uint32_t futuraDir = dir + tablaRPT[i].lastStride;
                    uint32_t paginaActual = dir >> bitsOffsetPagina;
                    uint32_t paginaFutura = futuraDir >> bitsOffsetPagina;
                    tablaRPT[i].lastAddress = dir;
                    tablaRPT[i].confidence = 2; // Pasa a estado confident.
                    if (paginaFutura == paginaActual) {
                        return true; // Cumple con los limites de pagina.
                    } else {
                        return false; // No cumple con los limites de pagina.
                    }
                } else {
                    tablaRPT[i].lastStride = dir - tablaRPT[i].lastAddress;
                    tablaRPT[i].lastAddress = dir;
                    tablaRPT[i].confidence = 1;
                    return false;
                }
            }
        }
    }
    cargarLinea(pc, dir);
    return false;
}

void PrefetchSal::cargarLinea(uint32_t pc, uint32_t dir) {
    uint32_t indice = buscarLineaLRU();
    tablaRPT[indice].validez = true;
    tablaRPT[indice].confidence = 0;
    tablaRPT[indice].cont = 0;
    tablaRPT[indice].lastAddress = dir;
    tablaRPT[indice].tag = pc;
}

bool PrefetchSal::notificarTipo() {
    return false;
}

void PrefetchSal::notificarAccion(uint32_t pc, uint32_t *contenedor) {
    for (int i = 0; i < numLineas; i++) {
        if(tablaRPT[i].tag == pc && tablaRPT[i].confidence == 2) {
            *contenedor = *contenedor + tablaRPT[i].lastStride;
            break;
        }
    }
}

// Destructor:
PrefetchSal::~PrefetchSal() {
}