#ifndef SALPREFETCH_H
#define SALPREFETCH_H
#include "motorPrefetch.hpp"

struct referencePT {
    uint32_t tag, lastAddress, cont = 0;
    int32_t lastStride : 30;
    uint32_t confidence : 2;
    bool validez = false;
};

class PrefetchSal : public Prefetch {
    private:
        uint32_t numLineas;
        std::vector<referencePT> tablaRPT; // Tabla de prediccion de referencia;
    public:
        PrefetchSal(uint32_t _numLineas, uint32_t _tamB);
        void actualizarCont(uint32_t linea);
        uint32_t buscarLineaLRU();
        bool logicaPrefetch(uint32_t pc, uint32_t dir) override;
        void cargarLinea(uint32_t pc, uint32_t dir);
        bool notificarTipo() override;
        void notificarAccion(uint32_t pc, uint32_t *contenedor) override;
        ~PrefetchSal();
};
#endif