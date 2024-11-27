#include <iostream>
#include <filesystem>
#include "iso_reader.h"
#include "capstone_wrapper.h"
#include <iostream>
#include <cstdlib>

namespace fs = std::filesystem;

void displayLogo() {
    // Códigos de escape ANSI para texto rojo
    const std::string red = "\033[31m";
    const std::string reset = "\033[0m";

    std::cout << red;
    std::cout << R"(
                         _                       ___    __ 
                        | |                     / _ \  /_ |
  _ __   ___  _ __  ___ | |_  _ __  ___ __  __ | | | |  | |
 | '_ \ / __|| '__|/ _ \| __|| '__|/ _ \\ \/ / | | | |  | |
 | |_) |\__ \| |  |  __/| |_ | |  | (_) |>  <  | |_| |_ | |
 | .__/ |___/|_|   \___| \__||_|   \___//_/\_\  \___/(_)|_|   
 | |                                                       
 |_|                       
    )" << std::endl;
    std::cout << reset; // Restablecer el color de la terminal
}

int convertPSSToMP4(const std::string& inputFilePath, const std::string& outputFilePath) {
    // Construir el comando de FFmpeg para convertir el archivo .PSS a .MP4
    std::string command = "ffmpeg -i " + inputFilePath + " -vcodec libx264 -acodec aac " + outputFilePath;

    // Ejecutar el comando de FFmpeg
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error al convertir el archivo." << std::endl;
        return -1;
    }

    std::cout << "Conversión completada: " << outputFilePath << std::endl;

    return 0;
}

int main() {

    displayLogo();

    std::string filePathBD = "iso/CRASH6/CRASH.BD";
    std::string filePathBH = "iso/CRASH6/CRASH.MH";
    std::string filePathSLUS = "iso/SLUS_209.09";
    std::string filePathMUSIC_BD = "iso/CRASH6/MUSIC.MB";
    std::string filePathMUSIC_MH = "iso/CRASH6/MUSIC.MH";
    std::string filePathAMERICAN_MB = "iso/CRASH6/AMERICAN.MB";
    std::string filePathAMERICAN_MH = "iso/CRASH6/AMERICAN.MH";

    std::string fmvFolder = "iso/FMV"; // Carpeta donde están los archivos .PSS
    std::string outputDir = "out/videos";
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Leer los archivos binarios
    std::vector<uint8_t> binaryCodeBD = readFile(filePathBD);
    std::vector<uint8_t> binaryCodeSLUS = readFile(filePathSLUS);
    std::vector<uint8_t> binaryCodeMUSIC_BD = readFile(filePathMUSIC_BD);
    std::vector<uint8_t> binaryCodeMUSIC_MH = readFile(filePathMUSIC_MH);
    std::vector<uint8_t> binaryCodeAMERICAN_MB = readFile(filePathAMERICAN_MB);
    std::vector<uint8_t> binaryCodeAMERICAN_MH = readFile(filePathAMERICAN_MH);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty() && binaryCodeMUSIC_BD.empty() && binaryCodeMUSIC_MH.empty() && binaryCodeAMERICAN_MB.empty() && binaryCodeAMERICAN_MH.empty()) {
        std::cerr << "No se pudo leer ninguno de los archivos." << std::endl;
        return 1;
    }

    // Desensamblar los archivos
    if (!binaryCodeBD.empty()) {
        std::cout << "Desensamblando CRASH.BD..." << std::endl;
        disassembleCode(binaryCodeBD, "CRASH.BD");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeSLUS.empty()) {
        std::cout << "Desensamblando SLUS_209.09..." << std::endl;
        disassembleCode(binaryCodeSLUS, "SLUS_209.09");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_BD.empty()) {
        std::cout << "Desensamblando MUSIC.MB..." << std::endl;
        disassembleCode(binaryCodeMUSIC_BD, "MUSIC.MB");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_MH.empty()) {
        std::cout << "Desensamblando MUSIC.MH..." << std::endl;
        disassembleCode(binaryCodeMUSIC_MH, "MUSIC.MH");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MB.empty()) {
        std::cout << "Desensamblando AMERICAN.MB..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MB, "AMERICAN.MB");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MH.empty()) {
        std::cout << "Desensamblando AMERICAN.MH..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MH, "AMERICAN.MH");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    // Procesar archivos .PSS en la carpeta FMV
    std::cout << "Procesando archivos en la carpeta FMV..." << std::endl;
    for (const auto& entry : fs::directory_iterator(fmvFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".PSS") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodePSS = readFile(filePath);
            if (!binaryCodePSS.empty()) {
                std::cout << "Desensamblando " << fileName << ".PSS..." << std::endl;
                disassembleCode(binaryCodePSS, fileName + ".PSS");
                std::cout << "Desensamblado Completado" << std::endl;
            }
            else {
                std::cerr << "No se pudo leer " << fileName << ".PSS" << std::endl;
            }

            std::string inputFilePath = fmvFolder + "/" + fileName + ".PSS";  // Ruta completa al archivo .PSS
            std::string outputFilePath = outputDir + "/" + fileName + ".mp4";  // Ruta completa al archivo .PSS

            convertPSSToMP4(inputFilePath, outputFilePath);

        }
    }

    std::cout << "Proceso finalizado." << std::endl;
    return 0;
}

