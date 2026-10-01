#ifndef SECPREFETCH_H
#define SECPREFETCH_H
#include "motorPrefetch.hpp"
#include <vector>
// Clase para el tipo Prefetching Secuencial:
struct lineaBuffer {
    uint32_t conjunto, tag;
};

struct buffer {
    uint32_t contLRU = 0, numElem = 0;
    bool validez = false;
    std::vector<lineaBuffer> lineas;
};

class PrefetchSec : public Prefetch{
    private:
        uint32_t prof, cant, bitsConjunto, bitsTag, bitsOffset;
        std::vector<buffer> buferes;
    public:
        PrefetchSec(uint32_t _prof, uint32_t _cant, uint32_t tamB, uint32_t bitsC, uint32_t bitsT, uint32_t bitsOff);
        void actualizarContLRU(uint32_t indiceBuffer);
        void actualizarBuffer(uint32_t indiceBuffer);
        uint32_t bufferLRU();
        void cargarBuffer(uint32_t indiceBuffer, uint32_t dirFallo);
        bool logicaPrefetch(uint32_t pc, uint32_t dir) override;
        bool notificarTipo() override;
        void notificarAccion(uint32_t pc, uint32_t *contenedor) override;
        ~PrefetchSec();
};
#endif