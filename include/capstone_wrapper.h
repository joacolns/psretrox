#ifndef CAPSTONE_WRAPPER_H
#define CAPSTONE_WRAPPER_H

#include <vector>

// Función para desensamblar un archivo binario usando Capstone
void disassembleCode(const std::vector<uint8_t>& binaryCode, const std::string& fileName);


#endif
