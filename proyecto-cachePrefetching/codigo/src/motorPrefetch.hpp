#ifndef PREFETCH_H
#define PREFETCH_H
#define MAXPAG 4096 // Tamanio de pagina.
#include <vector>
#include <string>

// Clase base:
class Prefetch {
    protected:
        uint32_t tamB, bitsOffsetPagina;

        bool limitePagina(uint32_t numPagina, uint32_t dir);
    public:
        Prefetch(uint32_t _tamB);
        virtual bool logicaPrefetch(uint32_t pc, uint32_t dir) = 0;
        virtual bool notificarTipo() = 0;
        virtual void notificarAccion(uint32_t pc, uint32_t *contenedor) = 0;
        virtual ~Prefetch();
};
#endif 