//!--- En desarrollo ---!
#include <iostream>
#include <cstdint>
#include <fstream>
#include "controlador.hpp"

Controlador::Controlador() {

}

void Controlador::menu() {
    char opcion;
    std::cout << "============================================================\n";
    std::cout << "=¡Bienvenido a la Simulacion de una Cache con Prefetching! =\n";
    std::cout << "=                                                          =\n";
    std::cout << "= Proyecto final de: Arquitectura del Computador           =\n";
    std::cout << "= Estudiante: Ronald Montilla                              =\n";
    std::cout << "= Profesor: Jose Canache                                   =\n";
    std::cout << "= Numero de proyecto: 06                                   =\n";
    std::cout << "============================================================\n";
    std::cout << "- Presione 1 para continuar con la simulacion.\n";
    std::cout << "- Presione cualquier otra letra para salir.\n";
    std::cout << "Opcion a seleccionar: ";
    std::cin >> opcion;

    if (opcion == '1') {
        configuracionCache();
    }
}

void Controlador::configuracionCache() {
    char opcion;
    std::cout << "============================================================\n";
    std::cout << "Configuracion de Cache Asociativa por Conjuntos.\n";
    std::cout << "      * Bloques de " << cache.tamBloques << " Bytes\n";
    std::cout << "      * Numero de vias: " << cache.numeroVias << '\n';
    std::cout << "      * Numero de conjuntos: " << cache.numeroConjuntos << '\n';
    std::cout << "Opciones disponibles: \n";
    std::cout << "- Presione 1 si desea continuar.\n";
    std::cout << "- Presione 2 para establecer caracteristicas especificas de cache.\n";
    std::cout << "- Presione cualquier otra tecla para salir.\n"; 
    std::cout << "Opcion a seleccionar: ";
    std::cin >> opcion;
    if (opcion == '1') {
        configuracionPrefetch();
    } else if (opcion == '2') {
        datosCache();
    }

}

void Controlador::datosCache() {
    uint32_t tempBloque, tempConjunto, tempVia, bloque, via, conjunto;
    std::cout << "============================================================\n";
    std::cout << "= Por favor, ingrese valores que sean potencia de dos!     =\n";
    std::cout << "Tamaño de bloques(bytes): ";
    std::cin >> tempBloque;
    std::cout << "Numero de vias: ";
    std::cin >> tempVia;
    std::cout << "Numero de conjuntos: ";
    std::cin >> tempConjunto;
    bloque = tempBloque & (tempBloque - 1);
    via = tempVia & (tempVia - 1);
    conjunto = tempConjunto & (tempConjunto - 1);
    if (bloque != 0 || via != 0 || conjunto != 0) {
        return;
    }
    cache.tamBloques = tempBloque;
    cache.numeroVias = tempVia;
    cache.numeroConjuntos = tempConjunto;
    configuracionPrefetch();
}

void Controlador::configuracionPrefetch() {
    char opcion;
    std::cout << "============================================================\n";
    std::cout << "Configuracion del motor Prefetch:\n";
    std::cout << "- Presione 1 para una cache sin Prefetch.\n";
    std::cout << "- Presione 2 para una cache con Prefetch Secuencial.\n";
    std::cout << "- Presione 3 para una cache con Prefetch de Salto.\n";
    std::cout << "- Presione 4 para probar las tres opciones anteriores.\n";
    std::cout << "- Presione cualquier otra tecla para salir.\n";
    std::cout << "Opcion a selecionar: ";
    std::cin >> opcion;
    std::cout << '\n';
    if (opcion != '1' && opcion != '2' && opcion != '3' && opcion != '4') {
        return;
    } else if (opcion == '4') {
        todosPrefetch = true;
    }
    if (opcion == '1' || todosPrefetch) { // Simulacion de cache sin prefetching.
        cobertura = false; // Como se selecciono una cache sin prefetch no se muestra la
        // cobertura(coverage).
        motorUno = nullptr;
    }
    if (opcion == '2' || todosPrefetch) { // Simulacion de cache con prefetching secuencial.
        // Estructura predefinida del motor de prefetching secuencial:
        uint32_t numBuffers = NUMERO_BUFFER, sizeBuffer = PROFUNDIDAD_BUFFER;
        std::cout << "============================================================\n";
        std::cout << "Configuracion del motor de Prefetch Secuencial: \n";
        std::cout << "- Presione 1 para simular un grado predefinido de prefetch.\n";
        std::cout << "     * Numero de buferes: " << numBuffers << '\n';
        std::cout << "     * Grado de profundidad de cada buffer: " << sizeBuffer << '\n';
        std::cout << "- Presione 2 para establecer un grado de prefetch especifico.\n";
        std::cout << "- Presione cualquier otra tecla pasa salir de la simulacion.\n";
        std::cout << "Opcion a selecionar: ";
        std::cin >> opcion;
        if (opcion != '1' && opcion != '2') {
            return;
        } else if (opcion == '2') {
            uint32_t tempNumBuffers, tempSizeBuffer;
            std::cout << "\n¡Por favor, ingrese valores que sean potencia de dos, de lo\n";
            std::cout << "contrario el programa finalizara!\n\n";
            std::cout << "Numero de buffers: ";
            std::cin >> numBuffers;
            std::cout << "Grado de cada buffer(profundidad): ";
            std::cin >> sizeBuffer;
            tempNumBuffers = numBuffers & (numBuffers - 1);
            tempSizeBuffer = sizeBuffer & (sizeBuffer - 1);
            if (tempNumBuffers || tempSizeBuffer) {
                std::cout << "No puedes ingresar estos valores!\n";
                return;
            }
        }
        if (todosPrefetch) {
            motorDos = new PrefetchSec(sizeBuffer, numBuffers, cache.tamBloques, cache.numeroConjuntos, cache.numeroVias);
        } else {
            motorUno = new PrefetchSec(sizeBuffer, numBuffers, cache.tamBloques, cache.numeroConjuntos, cache.numeroVias);
        }
    }
    if (opcion == '3' || todosPrefetch){
        // Estructura predefinida del motor de prefetching de salto:
        uint32_t numEntradas = ENTRADAS_SALTO;
        std::cout << "============================================================\n";
        std::cout << "Configuracion del motor de Prefetch de Salto: \n";
        std::cout << "- Presione 1 para simular un grado predefinido de prefetch.\n";
        std::cout << "      * Numero de entradas de la tabla RPT: " << numEntradas << '\n';
        std::cout << "- Presione 2 para establecer un grado de prefetch especifico.\n";
        std::cout << "- Presione cualquier otra tecla pasa salir de la simulacion.\n";
        std::cout << "Opcion a selecionar: ";
        std::cin >> opcion;
        if (opcion != '1' && opcion != '2') {
            return;
        } else if (opcion == '2') {
            uint32_t tempEntradas;
            std::cout << "\n¡Por favor, ingrese valores que sean potencia de dos, de lo \n";
            std::cout << "contrario el programa finalizara!\n\n";
            std::cout << "Numero de entradas: ";
            std::cin >> numEntradas;
            tempEntradas = numEntradas & (numEntradas - 1);
            if (tempEntradas) {
                std::cout << "No puedes ingresar estos valores!\n";
                return;
            }
        }
        if (todosPrefetch) {
            motorTres = new PrefetchSal(numEntradas, cache.tamBloques);
        } else {
            motorUno = new PrefetchSal(numEntradas, cache.tamBloques);
        }
    }
    simulacion();
}

void Controlador::simulacion() {
    if (todosPrefetch) {
        cacheUno = new Cache(cache.tamBloques, cache.numeroVias, cache.numeroConjuntos, motorUno);
        cacheDos = new Cache(cache.tamBloques, cache.numeroVias, cache.numeroConjuntos, motorDos);
        cacheTres = new Cache(cache.tamBloques, cache.numeroVias, cache.numeroConjuntos, motorTres);
    } else if (!cobertura){
        cacheUno = new Cache(cache.tamBloques, cache.numeroVias, cache.numeroConjuntos, motorUno);
    } else {
        cacheUno = new Cache(cache.tamBloques, cache.numeroVias, cache.numeroConjuntos, motorUno);
        cacheDos = new Cache(cache.tamBloques, cache.numeroVias, cache.numeroConjuntos, nullptr);
    }
    std::string direccionEntrada;
    std::cout << "==================================================\n";
    std::cout << "Configuracion del archivo de entrada de trazas: \n";
    std::cout << "¡Por favor, ingrese el nombre del archivo de trazas!\n";
    std::cout << "Ej: datos/direcciones.txt\n\n";
    std::cout << "Nombre: ";
    std::cin >> direccionEntrada;
    std::ifstream archivoEntrada(direccionEntrada, std::ios::in);
    if (!archivoEntrada.is_open()) {
        std::cout << "Se produjo un error al intentar abrir un archivo\n";
        return;
    }
    // Llamamos a la funcion que leera las direcciones:
    auxSimulacion(archivoEntrada);
}

void Controlador::auxSimulacion(std::ifstream &archivo) {
    uint64_t direccion;
    uint32_t hitUno, hitDos, hitTres, missUno, missDos, missTres, contadorPrograma;
    std::string cadena;
    hitUno = hitDos = hitTres = missUno = missDos = missTres = 0;
    if (todosPrefetch) {
        while (std::getline(archivo, cadena)) {
            if (cadena[0] == 'I') {
                cadena = cadena.substr(3);
                contadorPrograma = std::stoul(cadena, nullptr, 16);
            } else if (cadena[0] == ' ') {
                if (cadena[1] != 'M') {
                    cadena = cadena.substr(3);
                    direccion = std::stoul(cadena, nullptr, 16);
                    if (cacheUno->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                        hitUno++;
                    } else {
                        missUno++;
                    }
                    if (cacheDos->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                        hitDos++;
                    } else {
                        missDos++;
                    }
                    if (cacheTres->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                        hitTres++;
                    } else {
                        missTres++;
                    }
                } else {
                    cadena = cadena.substr(3);
                    direccion = std::stoul(cadena, nullptr, 16);
                    for (int i = 0; i < 2; i++) {
                        if (cacheUno->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                            hitUno++;
                        } else {
                            missUno++;
                        }
                    }
                    for (int i = 0; i < 2; i++) {
                        if (cacheDos->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                            hitDos++;
                        } else {
                            missDos++;
                        }
                    }
                    for (int i = 0; i < 2; i++) {
                        if (cacheTres->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                            hitTres++;
                        } else {
                            missTres++;
                        }
                    }
                }
            }
        }
    } else if (!cobertura){
        while (std::getline(archivo, cadena)) {
            if (cadena[0] == 'I') {
                cadena = cadena.substr(3);
                contadorPrograma = std::stoul(cadena, nullptr, 16);
            } else if (cadena[0] == ' ') {
                if (cadena[1] != 'M') {
                    cadena = cadena.substr(3);
                    direccion = std::stoul(cadena, nullptr, 16);
                    if (cacheUno->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                        hitUno++;
                    } else {
                        missUno++;
                    }
                } else {
                    cadena = cadena.substr(3);
                    direccion = std::stoul(cadena, nullptr, 16);
                    for (int i = 0; i < 2; i++) {
                        if (cacheUno->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                            hitUno++;
                        } else {
                            missUno++;
                        }
                    }
                }
            }
        }
    } else {
        while (std::getline(archivo, cadena)) {
            if (cadena[0] == 'I') {
                cadena = cadena.substr(3);
                contadorPrograma = std::stoul(cadena, nullptr, 16);
            } else if (cadena[0] == ' ') {
                if (cadena[1] != 'M') {
                    cadena = cadena.substr(3);
                    direccion = std::stoul(cadena, nullptr, 16);
                    if (cacheUno->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                        hitUno++;
                    } else {
                        missUno++;
                    }
                    if (cacheDos->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                        hitDos++;
                    } else {
                        missDos++;
                    }
                } else {
                    cadena = cadena.substr(3);
                    direccion = std::stoul(cadena, nullptr, 16);
                    for (int i = 0; i < 2; i++) {
                        if (cacheUno->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                            hitUno++;
                        } else {
                            missUno++;
                        }
                        if (cacheDos->buscarDato(contadorPrograma, (uint32_t)direccion)) {
                            hitDos++;
                        } else {
                            missDos++;
                        }
                    }
                }
            }
        }
    }
    double tasaAciertoUno = 0, tasaAciertoDos = 0, tasaAciertoTres = 0;
    double coberturaDos, coberturaTres;
    if ((hitUno + missUno) > 0) {
        tasaAciertoUno =  ((double)hitUno / (hitUno + missUno)) * 100;
    }
    std::cout << "==================================================\n";
    std::cout << "Resultados obtenidos: \n";
    if (!todosPrefetch) {
        if (cobertura) {
            coberturaDos = (((double)hitUno - hitDos)/ missDos) * 100;
            std::cout << "\n      * Numero de hits: " << hitUno << '\n';
            std::cout << "      * Numero de miss: " << missUno << '\n';
            std::cout << "      * Tasa de acierto: " << tasaAciertoUno << "%\n";
            std::cout << "      * Cobertura: " << coberturaDos << "%\n\n";
        } else {
            std::cout << "\n      * Numero de hits: " << hitUno << '\n';
            std::cout << "      * Numero de miss: " << missUno << '\n';
            std::cout << "      * Tasa de acierto: " << tasaAciertoUno << "%\n";
        }
    } else {
        if ((hitDos + missDos) > 0) {
            tasaAciertoDos = ((double)hitDos / (hitDos + missDos)) * 100;
        }
        if ((hitTres + missTres) > 0) {
            tasaAciertoTres = ((double)hitTres / (hitTres + missTres)) * 100;
        }
        if (missUno > 0) {
            coberturaDos = (((double)hitDos - hitUno)/ missUno) * 100;
            coberturaTres = (((double)hitTres - hitUno)/ missUno) * 100;
        }
        std::cout << "\n- Cache sin Prefetch: \n";
        std::cout << "      * Numero de hits: " << hitUno << '\n';
        std::cout << "      * Numero de miss: " << missUno << '\n';
        std::cout << "      * Tasa de acierto: " << tasaAciertoUno << "%\n";
        std::cout << "- Cache con Prefetch Secuencial: \n";
        std::cout << "      * Numero de hits: " << hitDos << '\n';
        std::cout << "      * Numero de miss: " << missDos << '\n';
        std::cout << "      * Tasa de acierto: " << tasaAciertoDos << "%\n";
        std::cout << "      * Cobertura: " << coberturaDos << "%\n";
        std::cout << "- Cache con Prefetch de Salto: \n";
        std::cout << "      * Numero de hits: " << hitTres << '\n';
        std::cout << "      * Numero de miss: " << missTres << '\n';
        std::cout << "      * Tasa de acierto: " << tasaAciertoTres << "%\n";
        std::cout << "      * Cobertura: " << coberturaTres << "%\n\n";
    }
}

Controlador::~Controlador(){
        delete motorUno;
        delete motorDos;
        delete motorTres;
        delete cacheUno;
        delete cacheDos;
        delete cacheTres;
}