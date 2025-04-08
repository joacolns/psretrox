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
// Funci�n para leer un archivo binario
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::cout << "Leyendo archivo: " << filePath << std::endl;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::cout << "Archivo le�do: " << filePath << " (" << data.size() << " bytes)" << std::endl;
    return data;
}

// Funci�n para escribir un archivo binario
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

//Extract 3D models from CRASH.BH and CRASH.BD files
void extractModels(const std::string& crashBHPath, const std::string& crashBDPath, const std::string& outputDir) {
    std::cout << "Initiating model extraction..." << std::endl;
    std::vector<uint8_t> crashBH = readBinaryFile(crashBHPath);
    std::vector<uint8_t> crashBD = readBinaryFile(crashBDPath);

    std::filesystem::create_directories(outputDir);

    size_t entrySize = 12; //Let's assume each entry has 12 bytes (offset, size, type)
    size_t numEntries = crashBH.size() / entrySize;

    std::cout << "Number of entries: " << numEntries << std::endl;

    for (size_t i = 0; i < numEntries; ++i) {
        uint32_t offset = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize]);
        uint32_t size = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize + 4]);
        uint32_t type = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize + 8]);

        // Validar los valores le�dos
        if (offset > crashBD.size() || size > crashBD.size() || offset + size > crashBD.size()) {
            std::cerr << "Error: The block " << i + 1 << " has invalid values. Offset: " << offset << ", Size: " << size << std::endl;
            continue;
        }

        std::cout << "Extracting block " << i + 1 << " offset: 0x" << std::hex << offset
            << " size: " << size << " bytes, type: " << type << std::endl;

        std::vector<uint8_t> blockData(crashBD.begin() + offset, crashBD.begin() + offset + size);
        std::string blockFileName = outputDir + "/block_" + std::to_string(i + 1) + ".bin";
        writeBinaryFile(blockFileName, blockData);

        std::cout << "Block " << i + 1 << " saved in: " << blockFileName << std::endl;
    }

    std::cout << "Model extraction completed" << std::endl;
}
