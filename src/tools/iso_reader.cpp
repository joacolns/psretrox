#include "iso_reader.h"
#include <iostream>
#include <fstream>

std::vector<uint8_t> readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::vector<uint8_t> buffer;

    if (file) {
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer.resize(fileSize); // Redimensionar el buffer
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    }
    else {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
    }

    return buffer;
}