#include <iostream>
#include "iso_reader.h"
#include "capstone_wrapper.h"

int main() {
    // Rutas de los archivos que deseas leer y desensamblar
    std::string filePathBD = "iso/CRASH6/CRASH.BD";
    std::string filePathBH = "iso/CRASH6/CRASH.MH";
    std::string filePathSLUS = "iso/SLUS_209.09";
    std::string filePathMUSIC_BD = "iso/CRASH6/MUSIC.MB";
    std::string filePathMUSIC_MH = "iso/CRASH6/MUSIC.MH";
    std::string filePathAMERICAN_MB = "iso/CRASH6/AMERICAN.MB";
    std::string filePathAMERICAN_MH = "iso/CRASH6/AMERICAN.MH";



    // Leer los archivos binarios
    std::vector<uint8_t> binaryCodeBD = readFile(filePathBD);
    std::vector<uint8_t> binaryCodeSLUS = readFile(filePathSLUS);
    std::vector<uint8_t> binaryCodeMUSIC_BD = readFile(filePathMUSIC_BD);
    std::vector<uint8_t> binaryCodeMUSIC_MH = readFile(filePathMUSIC_MH);
    std::vector<uint8_t> binaryCodeAMERICAN_MB = readFile(filePathAMERICAN_MB);
    std::vector<uint8_t> binaryCodeAMERICAN_MH = readFile(filePathAMERICAN_MH);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty() && binaryCodeMUSIC_BD.empty() && binaryCodeMUSIC_MH.empty() && binaryCodeAMERICAN_MB.empty() && binaryCodeAMERICAN_MH.empty()){
        std::cerr << "No se pudo leer ninguno de los archivos." << std::endl;
        return 1;
    }

    // Desensamblar los archivos
    if (!binaryCodeBD.empty()) {
        std::cout << "Desensamblando CRASH.BD..." << std::endl;
        disassembleCode(binaryCodeBD, "CRASH.BD");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeSLUS.empty()) {
        std::cout << "Desensamblando SLUS_209.09..." << std::endl;
        disassembleCode(binaryCodeSLUS, "SLUS_209.09");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_BD.empty()) {
        std::cout << "Desensamblando MUSIC.MB..." << std::endl;
        disassembleCode(binaryCodeMUSIC_BD, "MUSIC.MB");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_MH.empty()) {
        std::cout << "Desensamblando MUSIC.MH..." << std::endl;
        disassembleCode(binaryCodeMUSIC_MH, "MUSIC.MH");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MB.empty()) {
        std::cout << "Desensamblando AMERICAN.MB..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MB, "AMERICAN.MB");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MH.empty()) {
        std::cout << "Desensamblando AMERICAN.MH..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MH, "AMERICAN.MH");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    return 0;
}
