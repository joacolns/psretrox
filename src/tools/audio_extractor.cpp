#include "audio_extractor.h"
#include "file_utils.h" 
#include <iostream>
#include <vector>
#include <filesystem>
#include <stdexcept>


void extractAudioTracks(const std::string& musicMBPath, const std::string& musicMHPath, const std::string& outputDir) {
    std::vector<uint8_t> musicMB = readBinaryFile(musicMBPath);
    std::vector<uint8_t> musicMH = readBinaryFile(musicMHPath);

    std::filesystem::create_directories(outputDir);

    size_t numTracks = musicMH.size() / 8;
    std::cout << "Número de pistas: " << numTracks << std::endl;

    for (size_t i = 0; i < numTracks; ++i) {
        uint32_t offset = *reinterpret_cast<const uint32_t*>(&musicMH[i * 8]);
        uint32_t size = *reinterpret_cast<const uint32_t*>(&musicMH[i * 8 + 4]);

        std::cout << "Extrayendo pista " << i + 1 << " en offset: 0x" << std::hex << offset
            << " con tamaño: " << size << " bytes" << std::endl;

        if (offset + size > musicMB.size()) {
            std::cerr << "Error: La pista " << i + 1 << " excede los límites de MUSIC.MB." << std::endl;
            continue;
        }

        std::vector<uint8_t> trackData(musicMB.begin() + offset, musicMB.begin() + offset + size);

        if (trackData.empty()) {
            std::cerr << "Advertencia: La pista " << i + 1 << " no contiene datos válidos." << std::endl;
            continue;
        }

        std::string trackFileName = outputDir + "/track_" + std::to_string(i + 1) + ".vag"; // Suponiendo formato PS2 VAG
        writeBinaryFile(trackFileName, trackData);

        std::cout << "Pista " << i + 1 << " guardada en: " << trackFileName << std::endl;
    }
}

int audio_extraction_test() {
    try {
        std::string musicMBPath = "iso/CRASH6/MUSIC.MB";
        std::string musicMHPath = "iso/CRASH6/MUSIC.MH";

        std::string outputDir = "out/CrashTwinsanity/audios";

        if (!std::filesystem::exists(outputDir)) {
            std::filesystem::create_directory(outputDir);
        }

        extractAudioTracks(musicMBPath, musicMHPath, outputDir);
        std::cout << "Extracción completada. Los archivos están en: " << outputDir << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void convertVagToAudio(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::string vgmstreamPath = "C:/VisualStudioLibraries/vgmstream/vgmstream-cli.exe";
    std::string command = vgmstreamPath + " -o " + outputFilePath + " " + inputFilePath;

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Conversión exitosa: " << inputFilePath << " a " << outputFilePath << std::endl;
    }
    else {
        std::cerr << "Hubo un error al convertir el archivo: " << inputFilePath << std::endl;
    }
}

int audioOut() {
    std::string inputDirectory = "out/CrashTwinsanity/audios";

    std::string outputDirectory = "out/CrashTwinsanity/audios";

    if (!std::filesystem::exists(outputDirectory)) {
        std::filesystem::create_directory(outputDirectory);
    }

    for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vag") {
            std::string inputFilePath = entry.path().string();

            std::string outputFilePath = outputDirectory + entry.path().stem().string() + ".mp3";

            convertVagToAudio(inputFilePath, outputFilePath);
        }
    }

    return 0;
}
