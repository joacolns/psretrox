#include <string>
#include <filesystem>
#include <iostream>
#include "capstone_wrapper.h"
#include "iso_reader.h"

namespace fs = std::filesystem;

int convertPSSToMP4(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::string command = "ffmpeg -i " + inputFilePath + " -vcodec libx264 -acodec aac " + outputFilePath;

    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error converting file." << std::endl;
        return -1;
    }

    std::cout << "Conversion Completed: " << outputFilePath << std::endl;

    return 0;
}

int PSS_Processor() {

    std::string fmvFolder = "iso/FMV";
    std::string fmvFolderBONS = "iso/FMV/BONUS"; // <-------- ONLY FOR CRASH BANDICOOT TWINSANITY

    std::string outputDir = "out/CrashTwinsanity/videos";

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    std::cout << "Procesando archivos en la carpeta FMV..." << std::endl;
    for (const auto& entry : fs::directory_iterator(fmvFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".PSS") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string();

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodePSS = readFile(filePath);
            if (!binaryCodePSS.empty()) {
                std::cout << "Disassembling " << fileName << ".PSS..." << std::endl;
                disassembleCode(binaryCodePSS, fileName + ".PSS");
                std::cout << "Disassembled Completed" << std::endl;
            }
            else {
                std::cerr << "Can't read " << fileName << ".PSS" << std::endl;
            }

            std::string inputFilePath = fmvFolder + "/" + fileName + ".PSS";
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4"; 

            convertPSSToMP4(inputFilePath, outputFilePathMP4);

        }
    }

    std::cout << "Processing files in the BONUS folder..." << std::endl;
    for (const auto& entry : fs::directory_iterator(fmvFolderBONS)) {
        if (entry.is_regular_file() && entry.path().extension() == ".PSS") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string();

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodePSS = readFile(filePath);
            if (!binaryCodePSS.empty()) {
                std::cout << "Disassembling " << fileName << ".PSS..." << std::endl;
                disassembleCode(binaryCodePSS, fileName + ".PSS");
                std::cout << "Disassembled Completed" << std::endl;
            }
            else {
                std::cerr << "Can't read  " << fileName << ".PSS" << std::endl;
            }

            std::string inputFilePath = fmvFolderBONS + "/" + fileName + ".PSS"; 
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4";

            convertPSSToMP4(inputFilePath, outputFilePathMP4);

        }
    }

    return 0;
}