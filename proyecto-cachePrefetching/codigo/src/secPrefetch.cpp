#include <iostream>
#include <cstdint>
#include "secPrefetch.hpp"

// Metodos de la clase hija "PrefetchSec"
PrefetchSec::PrefetchSec(uint32_t _prof, uint32_t _cant, uint32_t tamB, uint32_t bitsC, uint32_t bitsT, uint32_t bitsOff)
: Prefetch(tamB), prof(_prof), cant(_cant), bitsConjunto(bitsC), bitsTag(bitsT), bitsOffset(bitsOff) {
    buferes.resize(_cant);
    for (int i = 0; i < _cant; i++) {
        buferes[i].lineas.resize(_prof);
    }
}

void PrefetchSec::actualizarContLRU(uint32_t indiceBuffer) {
    uint32_t lruACambiar = buferes[indiceBuffer].contLRU;
    for (int i = 0; i < cant; i++) {
        if (i != indiceBuffer && buferes[i].contLRU <= lruACambiar) {
            buferes[i].contLRU++;
        }
    }
    buferes[indiceBuffer].contLRU = 0;
}

void PrefetchSec::actualizarBuffer(uint32_t indiceBuffer) {
    for (int i = 0; i < buferes[indiceBuffer].numElem; i++) {
        buferes[indiceBuffer].lineas[i] = buferes[indiceBuffer].lineas[i + 1];
    }
}

uint32_t PrefetchSec::bufferLRU() {
    // Retorna el indice de un bufer disponible o el menos reciente usado(LRU).
    uint32_t mayor = 0, indice = 0;
    for (int i = 0; i < cant; i++) {
        if (!buferes[i].validez) {
            return i;
        } else if (buferes[i].contLRU >= mayor){
            indice = i;
            mayor = buferes[i].contLRU;
        }
    }
    return indice;
}

void PrefetchSec::cargarBuffer(uint32_t indiceBuffer, uint32_t dirFallo) {
    // Carga de bloques en los buferes de flujo segun la profundidad indicada.
    uint32_t numElementos = buferes[indiceBuffer].numElem;
    // Numero de pagina de la direccion de fallo:
    uint32_t numPagina = dirFallo >> bitsOffsetPagina;
    // Bloque siguiente al cual pertenece dirFallo:
    if (!numElementos) { // El buffer esta vacio.
        dirFallo = (dirFallo | ((1U << bitsOffset) - 1)) + 1;
    } else { // El buffer no esta vacio.
        uint32_t auxConjunto = buferes[indiceBuffer].lineas[numElementos - 1].conjunto;
        uint32_t auxTag = buferes[indiceBuffer].lineas[numElementos - 1].tag;
        dirFallo = ((auxTag << (bitsConjunto + bitsOffset)) | (auxConjunto << bitsOffset)) + tamB;
    }
    for (int i = numElementos; i < prof; i++) {
        if (!limitePagina(numPagina, dirFallo)) {
            // El motor de prefetching intenta acceder a otra pagina, por ende, se prohibe esta accion.
            if (buferes[indiceBuffer].numElem > 0) {
                break;
            }
            buferes[indiceBuffer].validez = false;
            return;
        }
        buferes[indiceBuffer].lineas[i].conjunto = (dirFallo >> bitsOffset) & ((1U << bitsConjunto) - 1);
        buferes[indiceBuffer].lineas[i].tag = (dirFallo >> (bitsOffset + bitsConjunto));
        buferes[indiceBuffer].numElem++;
        dirFallo += tamB;

    }
    actualizarContLRU(indiceBuffer);
    buferes[indiceBuffer].validez = true;
}

bool PrefetchSec::logicaPrefetch(uint32_t pc, uint32_t dir) {
    // Este metodo no usara "pc", pero por polimorfismo debo colocarla.
    // Ante un fallo, se buscara el dato en el buffer.
    uint32_t conjuntoSel = (dir >> bitsOffset) & ((1U << bitsConjunto) - 1);
    uint32_t tagSel = (dir >> (bitsOffset + bitsConjunto));
    for (int i = 0; i < cant; i++) {
        if (buferes[i].validez) {
            if (buferes[i].lineas[0].conjunto == conjuntoSel && buferes[i].lineas[0].tag == tagSel) {
                buferes[i].numElem--;
                actualizarBuffer(i);
                cargarBuffer(i, dir);
                return true;
            }
        }
    }
    uint32_t indiceModificar = bufferLRU();
    buferes[indiceModificar].numElem = 0;
    cargarBuffer(indiceModificar, dir);
    return false;
}

bool PrefetchSec::notificarTipo() {
    return true;
}

void PrefetchSec::notificarAccion(uint32_t pc, uint32_t *contenedor) {
}

PrefetchSec::~PrefetchSec() {
}