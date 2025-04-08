#include "audio_extractor.h"
#include "file_utils.h" // Include new header file
#include <iostream>
#include <vector>
#include <filesystem>
#include <stdexcept>

/*
//Read a binary file from a bytes vector
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Write a binary file
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo crear el archivo: " + filePath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}
*/

//Extract audio tracks from MUSIC.MB y MUSIC.MH
void extractAudioTracks(const std::string& musicMBPath, const std::string& musicMHPath, const std::string& outputDir) {
    //Read MUSIC.MB and MUSIC.MH files
    std::vector<uint8_t> musicMB = readBinaryFile(musicMBPath);
    std::vector<uint8_t> musicMH = readBinaryFile(musicMHPath);

    //Make an output directory if it doesn't exist
    std::filesystem::create_directories(outputDir);

    //Get MUSIC.MH track index
    size_t numTracks = musicMH.size() / 8; //Each entry has 8 bytes.
    std::cout << "Total tracks: " << numTracks << std::endl;

    for (size_t i = 0; i < numTracks; ++i) {
        //Read offset and size from MUSIC.MH for each track
        uint32_t offset = *reinterpret_cast<const uint32_t*>(&musicMH[i * 8]);
        uint32_t size = *reinterpret_cast<const uint32_t*>(&musicMH[i * 8 + 4]);

        std::cout << "Extracting track " << i + 1 << " offset: 0x" << std::hex << offset
            << " size: " << size << " bytes" << std::endl;

        //Check if offset and size are within the limits of MUSIC.MB
        if (offset + size > musicMB.size()) {
            std::cerr << "Error: Track " << i + 1 << " excedes MUSIC.MB limits" << std::endl;
            continue;
        }
        //Extract trackData
        std::vector<uint8_t> trackData(musicMB.begin() + offset, musicMB.begin() + offset + size);

        //Check if trackData is valid by checking either structure or header
        if (trackData.empty()) {
            std::cerr << "WARNING: track " << i + 1 << " does not have valid data." << std::endl;
            continue;
        }

        //Save track in a file
        std::string trackFileName = outputDir + "/track_" + std::to_string(i + 1) + ".vag"; // Suponiendo formato PS2 VAG
        writeBinaryFile(trackFileName, trackData);

        //sout to confirm that the track has been extracted
        std::cout << "Track " << i + 1 << " has been saved in: " << trackFileName << std::endl;
    }
}
