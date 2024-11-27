#include "iso_reader.h"
#include <iostream>
#include <fstream>

std::vector<uint8_t> readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::vector<uint8_t> buffer;

    if (file) {
        file.seekg(0, std::ios::end);  // Ir al final del archivo
        size_t fileSize = file.tellg(); // Obtener el tamaño del archivo
        file.seekg(0, std::ios::beg);   // Volver al inicio del archivo

        buffer.resize(fileSize); // Redimensionar el buffer
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize); // Leer el archivo en el buffer
    }
    else {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
    }

    return buffer;
}