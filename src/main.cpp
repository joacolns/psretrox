#include <iostream>
#include "iso_reader.h"
#include "capstone_wrapper.h"

int main() {
    // Rutas de los archivos que deseas leer y desensamblar
    std::string filePathBD = "iso/CRASH6/CRASH.BD";  // Archivo .BD de ejemplo
    std::string filePathSLUS = "iso/SLUS_209.09";   // Archivo SLUS_209.09 que es crucial

    // Leer los archivos binarios
    std::vector<uint8_t> binaryCodeBD = readFile(filePathBD);
    std::vector<uint8_t> binaryCodeSLUS = readFile(filePathSLUS);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty()) {
        std::cerr << "No se pudo leer ninguno de los archivos." << std::endl;
        return 1;
    }

    // Desensamblar los archivos
    if (!binaryCodeBD.empty()) {
        std::cout << "Desensamblando CRASH.BD..." << std::endl;
        disassembleCode(binaryCodeBD);
    }

    if (!binaryCodeSLUS.empty()) {
        std::cout << "Desensamblando SLUS_209.09..." << std::endl;
        disassembleCode(binaryCodeSLUS);
    }

    return 0;
}