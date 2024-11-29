// En 3d_proccesor.cpp
#include "3d_proccesor.h"
#include "file_utils.h" // Incluir el nuevo archivo de cabecera
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdint>

/*
// Función para leer un archivo binario
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::cout << "Leyendo archivo: " << filePath << std::endl;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::cout << "Archivo leído: " << filePath << " (" << data.size() << " bytes)" << std::endl;
    return data;
}

// Función para escribir un archivo binario
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    std::cout << "Escribiendo archivo: " << filePath << std::endl;
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo crear el archivo: " + filePath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    std::cout << "Archivo escrito: " << filePath << " (" << data.size() << " bytes)" << std::endl;
}
*/

// Función para extraer modelos 3D de los archivos CRASH.BH y CRASH.BD
void extractModels(const std::string& crashBHPath, const std::string& crashBDPath, const std::string& outputDir) {
    std::cout << "Iniciando extracción de modelos..." << std::endl;
    std::vector<uint8_t> crashBH = readBinaryFile(crashBHPath);
    std::vector<uint8_t> crashBD = readBinaryFile(crashBDPath);

    std::filesystem::create_directories(outputDir);

    size_t entrySize = 12; // Supongamos que cada entrada tiene 12 bytes (offset, tamaño, tipo)
    size_t numEntries = crashBH.size() / entrySize;

    std::cout << "Número de entradas: " << numEntries << std::endl;

    for (size_t i = 0; i < numEntries; ++i) {
        uint32_t offset = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize]);
        uint32_t size = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize + 4]);
        uint32_t type = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize + 8]);

        // Validar los valores leídos
        if (offset > crashBD.size() || size > crashBD.size() || offset + size > crashBD.size()) {
            std::cerr << "Error: El bloque " << i + 1 << " tiene valores inválidos. Offset: " << offset << ", Tamaño: " << size << std::endl;
            continue;
        }

        std::cout << "Extrayendo bloque " << i + 1 << " en offset: 0x" << std::hex << offset
            << " con tamaño: " << size << " bytes, tipo: " << type << std::endl;

        std::vector<uint8_t> blockData(crashBD.begin() + offset, crashBD.begin() + offset + size);
        std::string blockFileName = outputDir + "/block_" + std::to_string(i + 1) + ".bin";
        writeBinaryFile(blockFileName, blockData);

        std::cout << "Bloque " << i + 1 << " guardado en: " << blockFileName << std::endl;
    }

    std::cout << "Extracción de modelos completada." << std::endl;
}
