//!--- En desarrollo ---!
#ifndef MENU_H
#define MENU_H
#define NUMERO_BUFFER 64
#define PROFUNDIDAD_BUFFER 16
#define ENTRADAS_SALTO 256
#include "cache.hpp"
#include "motorPrefetch.hpp"
#include "secPrefetch.hpp"
#include "salPrefetch.hpp"

struct cacheConfig {
    uint32_t tamBloques = 64, numeroConjuntos = 128, numeroVias = 8;
};

class Controlador {
    private:
        cacheConfig cache;
        bool todosPrefetch = false, cobertura = true;
        Prefetch *motorUno = nullptr, *motorDos = nullptr, *motorTres = nullptr;
        Cache *cacheUno = nullptr, *cacheDos = nullptr, *cacheTres = nullptr;
    public:
        Controlador();
        void menu();
        void configuracionCache();
        void datosCache();
        void configuracionPrefetch();
        void simulacion();
        void auxSimulacion(std::ifstream &archivo);
        ~Controlador();
};
#endif