#include "3d_proccesor.h"
#include "file_utils.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdint>

void extractModels(const std::string& crashBHPath, const std::string& crashBDPath, const std::string& outputDir) {
    std::cout << "Starting model extraction..." << std::endl;
    std::vector<uint8_t> crashBH = readBinaryFile(crashBHPath);
    std::vector<uint8_t> crashBD = readBinaryFile(crashBDPath);

    std::filesystem::create_directories(outputDir);

    size_t entrySize = 12;
    size_t numEntries = crashBH.size() / entrySize;

    std::cout << "ENTRY NUMBERS: " << numEntries << std::endl;

    for (size_t i = 0; i < numEntries; ++i) {
        uint32_t offset = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize]);
        uint32_t size = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize + 4]);
        uint32_t type = *reinterpret_cast<const uint32_t*>(&crashBH[i * entrySize + 8]);

        // Validar los valores le�dos
        if (offset > crashBD.size() || size > crashBD.size() || offset + size > crashBD.size()) {
            std::cerr << "Error: Block " << i + 1 << " had some invalid values. Offset: " << offset << ", Size: " << size << std::endl;
            continue;
        }

        std::cout << "Extracting block " << i + 1 << " offset: 0x" << std::hex << offset
            << " Size: " << size << " bytes, Type: " << type << std::endl;

        std::vector<uint8_t> blockData(crashBD.begin() + offset, crashBD.begin() + offset + size);
        std::string blockFileName = outputDir + "/block_" + std::to_string(i + 1) + ".bin";
        writeBinaryFile(blockFileName, blockData);

        std::cout << "Block " << i + 1 << " saved in: " << blockFileName << std::endl;
    }

    std::cout << "Model extraction completed." << std::endl;
}

int proccesor() {
    std::string crashBHPath = "iso/CRASH6/CRASH.BH";
    std::string crashBDPath = "iso/CRASH6/CRASH.BD";
    std::string outputDir = "out/CrashTwinsanity/3D";

    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directory(outputDir);
    }

    try {
        extractModels(crashBHPath, crashBDPath, outputDir);
        std::cout << "Model extraction and conversion completed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}