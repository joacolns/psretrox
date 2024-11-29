#include "file_utils.h"
#include <fstream>
#include <stdexcept>

// Función para leer un archivo binario en un vector de bytes
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Función para escribir un archivo binario
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo crear el archivo: " + filePath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}
