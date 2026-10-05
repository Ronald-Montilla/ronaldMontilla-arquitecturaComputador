#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include "cache.hpp"
#include "motorPrefetch.hpp"
#include "secPrefetch.hpp"
#include "salPrefetch.hpp"
#include "controlador.hpp"
int main() {
    Controlador program;
    program.menu();
    return 0;
}