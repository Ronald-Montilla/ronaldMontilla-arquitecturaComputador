#include "cache.hpp"
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

// Constructor:
Cache::Cache(uint32_t _tamBloques, uint32_t _numVias, uint32_t _numConjuntos, Prefetch *motor)
: tamBloques(_tamBloques), numVias(_numVias), numConjuntos(_numConjuntos), motorPrefetch(motor){
    // Deteminando cuantos bits usara el offset, conjunto y tag.
    uint32_t temp;
    // Cantidad de bits que seran usados para identificar el offset:
    temp = _tamBloques;
    bitsOffset = 0;
    while (temp > 1) {
        temp >>= 1;
        bitsOffset++;
    }
    // Cantidad de bits que seran usados para identificar el conjunto:
    temp = _numConjuntos;
    bitsConjunto = 0;
    while (temp > 1) {
        temp >>= 1;
        bitsConjunto++;
    }
    // Cantidad de bits que seran usados para identificar el tag:
    bitsTag = 32 - bitsOffset - bitsConjunto;
    inicializarCache(_numConjuntos, _numVias);
}

// Modificadores:
void Cache::inicializarCache(const uint32_t numConjuntos, const uint32_t numVias) {
    // Prec: la cache se tuvo que haber creado.
    conjuntos.resize(numConjuntos);
    for (int i = 0; i < numConjuntos; i++) {
        conjuntos[i].vias.resize(numVias);
    }
}

void Cache::actualizarContadores(const uint32_t conjunto, const int via) {
    uint32_t contViaSel = conjuntos[conjunto].vias[via].cont;
    for (int i = 0; i < numVias; i++) {
        if (i != via && conjuntos[conjunto].vias[i].bitValidez && conjuntos[conjunto].vias[i].cont <= contViaSel) {
            conjuntos[conjunto].vias[i].cont++;
        }
    }
    conjuntos[conjunto].vias[via].cont = 0;
}

void Cache::agregarDato(const uint32_t dir) {
    // Recibe una direccion de 32 bits y la busca en la cache. Retorna true(hit) o false(miss).
    uint32_t conjuntoSel = (dir >> bitsOffset) & ((1U << bitsConjunto) - 1);
    uint32_t tagSel = (dir >> (bitsOffset + bitsConjunto));
    int band = 0, indice, mayor = 0;
    for (int i = 0; i < numVias; i++) {
        if (!conjuntos[conjuntoSel].vias[i].bitValidez) {
            conjuntos[conjuntoSel].vias[i].cont = numVias;
            actualizarContadores(conjuntoSel, i);
            conjuntos[conjuntoSel].vias[i].bitValidez = true;
            conjuntos[conjuntoSel].vias[i].tag = tagSel;
            band = 1;
            break;
        } else {
            if (conjuntos[conjuntoSel].vias[i].cont >= mayor) {
                mayor = conjuntos[conjuntoSel].vias[i].cont;
                indice = i;
            }
        }
    }
    if (band == 0) {
            actualizarContadores(conjuntoSel, indice);
            conjuntos[conjuntoSel].vias[indice].bitValidez = true;
            conjuntos[conjuntoSel].vias[indice].tag = tagSel;
    }
}

bool Cache::buscarDato(const uint32_t pc, const uint32_t dir) {
    // Recibe una direccion de 32 bits y la busca en la cache. Retorna true(hit) o false(miss).
    uint32_t conjuntoSel = (dir >> bitsOffset) & ((1U << bitsConjunto) - 1);
    uint32_t tagSel = (dir >> (bitsOffset + bitsConjunto));
    for (int i = 0; i < numVias; i++) {
        if (conjuntos[conjuntoSel].vias[i].bitValidez && conjuntos[conjuntoSel].vias[i].tag == tagSel) {
            actualizarContadores(conjuntoSel, i);
            return true;
        }
    }
    // El dato debe guardarse en la cache debido al miss.
    if (motorPrefetch != nullptr && motorPrefetch->logicaPrefetch(pc, dir)) {
        if (motorPrefetch->notificarTipo()) {
            agregarDato(dir);
            return true;
        } else {
            uint32_t nuevaDir = dir;
            motorPrefetch->notificarAccion(pc, &nuevaDir);
            agregarDato(nuevaDir);
            return false;   
        }
    }
    agregarDato(dir);
    return false;
}

// Destructor:
Cache::~Cache() {
}