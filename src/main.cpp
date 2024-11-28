#include <iostream>
#include <filesystem>
#include "iso_reader.h"
#include "capstone_wrapper.h"
#include <cstdlib>
#include "mips_mapping.h"
#include "audio_extractor.h"

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

void menu() {

        std::cout << "1 - Decompile files and recompile to C" << std::endl;
        std::cout << "2 - Decompile to MIPS assembly" << std::endl;
        std::cout << "3 - Recompile to C" << std::endl;
        std::cout << "4 - Decompile cutscenes" << std::endl;
        std::cout << "5 - Decompile audios" << std::endl;
        std::cout << "q - Quit" << std::endl;
        std::cout << "\n";
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

void recomp_C() {
    std::cout << "Recompilando..." << std::endl;

    std::string OUTFolder = "out/CrashTwinsanity";
    std::string RECOMPFolder = "out/CrashTwinsanity/C_recomp/";

    // Asegúrate de que la carpeta existe
    if (!fs::exists(OUTFolder) || !fs::is_directory(OUTFolder)) {
        std::cerr << "La carpeta " << OUTFolder << " no existe o no es un directorio válido." << std::endl;
        return;
    }

    if (!fs::exists(RECOMPFolder)) {
        std::cout << "La carpeta " << RECOMPFolder << " no existe. Creándola..." << std::endl;
        if (!fs::create_directory(RECOMPFolder)) {
            std::cerr << "Error al crear la carpeta " << RECOMPFolder << std::endl;
            return;
        }
    }

    for (const auto& entry : fs::directory_iterator(OUTFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".asm") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            std::cout << "Procesando archivo: " << fileName << ".asm" << std::endl;

            // Generar un archivo de salida único con el mismo nombre del archivo .log
            std::string archivo_salida = RECOMPFolder + fileName + ".c";

            // Leer y desensamblar el archivo .log
            std::vector<uint8_t> assemblyfile = readFile(filePath);
            if (!assemblyfile.empty()) {
                std::cout << "Recompilando " << fileName << " a C..." << std::endl;
                convertir_a_C(filePath, archivo_salida);  // Usar el nombre dinámico
                std::cout << "Recompilado Completado para " << fileName << std::endl;
            }
            else {
                std::cerr << "No se pudo leer el archivo " << fileName << ".log" << std::endl;
            }
        }
    }
}

int DecompileMIPS() {

    std::cout << "Desensamblando Crash Twinsanity" << std::endl;

    std::string filePathSYS = "iso/SYSTEM.CNF";

    std::string filePathBD = "iso/CRASH6/CRASH.BD";
    std::string filePathBH = "iso/CRASH6/CRASH.MH";
    std::string filePathSLUS = "iso/SLUS_209.09";
    std::string filePathMUSIC_BD = "iso/CRASH6/MUSIC.MB";
    std::string filePathMUSIC_MH = "iso/CRASH6/MUSIC.MH";
    std::string filePathAMERICAN_MB = "iso/CRASH6/AMERICAN.MB";
    std::string filePathAMERICAN_MH = "iso/CRASH6/AMERICAN.MH";

    std::string IRXFolder = "iso/CRASH6/SYS";

    std::string outputDir = "out/CrashTwinsanity/videos";

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
    std::vector<uint8_t> binaryCodeSYS = readFile(filePathSYS);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty() && binaryCodeMUSIC_BD.empty() && binaryCodeMUSIC_MH.empty() && binaryCodeAMERICAN_MB.empty() && binaryCodeAMERICAN_MH.empty() && binaryCodeSYS.empty()) {
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

    if (!binaryCodeSYS.empty()) {
        std::cout << "Desensamblando SYSTEM.CNF..." << std::endl;
        disassembleCode(binaryCodeSYS, "SYSTEM.CNF");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    std::cout << "Procesando archivos en la carpeta SYS (IRX)..." << std::endl;
    for (const auto& entry : fs::directory_iterator(IRXFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".IRX") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // Nombre del archivo sin extensión

            // Leer y desensamblar el archivo .PSS
            std::vector<uint8_t> binaryCodeIRX = readFile(filePath);
            if (!binaryCodeIRX.empty()) {
                std::cout << "Desensamblando " << fileName << ".IRX..." << std::endl;
                disassembleCode(binaryCodeIRX, fileName + ".IRX");
                std::cout << "Desensamblado Completado" << std::endl;
            }
            else {
                std::cerr << "No se pudo leer " << fileName << ".IRX" << std::endl;
            }

        }
    }


    return 0;
}

int PSS_Processor() {

    std::string fmvFolder = "iso/FMV"; // Carpeta donde están los archivos .PSS
    std::string fmvFolderBONS = "iso/FMV/BONUS"; // <-------- ONLY FOR CRASH BANDICOOT TWINSANITY

    std::string outputDir = "out/CrashTwinsanity/videos";

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
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
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4";  // Ruta completa al archivo .PSS

            convertPSSToMP4(inputFilePath, outputFilePathMP4);

        }
    }

    std::cout << "Procesando archivos en la carpeta BONUS..." << std::endl;
    for (const auto& entry : fs::directory_iterator(fmvFolderBONS)) {
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

            std::string inputFilePath = fmvFolderBONS + "/" + fileName + ".PSS";  // Ruta completa al archivo .PSS
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4";  // Ruta completa al archivo .PSS

            convertPSSToMP4(inputFilePath, outputFilePathMP4);

        }
    }

    return 0;
}

int audio_extraction_test() {
    try {
        // Ruta de los archivos de entrada
        std::string musicMBPath = "iso/CRASH6/MUSIC.MB";
        std::string musicMHPath = "iso/CRASH6/MUSIC.MH";

        // Directorio de salida
        std::string outputDir = "out/CrashTwinsanity/audios";

        if (!std::filesystem::exists(outputDir)) {
            std::filesystem::create_directory(outputDir);
        }

        // Extraer las pistas
        extractAudioTracks(musicMBPath, musicMHPath, outputDir);
        std::cout << "Extracción completada. Los archivos están en: " << outputDir << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void convertVagToAudio(const std::string& inputFilePath, const std::string& outputFilePath) {
    // Ruta completa a vgmstream-cli
    std::string vgmstreamPath = "C:/VisualStudioLibraries/vgmstream/vgmstream-cli.exe";
    std::string command = vgmstreamPath + " -o " + outputFilePath + " " + inputFilePath;

    // Ejecutar el comando en la terminal
    int result = std::system(command.c_str());

    // Verificar si la conversión fue exitosa
    if (result == 0) {
        std::cout << "Conversión exitosa: " << inputFilePath << " a " << outputFilePath << std::endl;
    }
    else {
        std::cerr << "Hubo un error al convertir el archivo: " << inputFilePath << std::endl;
    }
}

int audioOut() {
    // Directorio donde están los archivos .vag extraídos
    std::string inputDirectory = "out/CrashTwinsanity/audios";

    // Directorio donde guardar los archivos convertidos
    std::string outputDirectory = "out/CrashTwinsanity/audios";

    // Verificar que el directorio de salida existe, si no, crearlo
    if (!std::filesystem::exists(outputDirectory)) {
        std::filesystem::create_directory(outputDirectory);
    }

    // Iterar sobre los archivos .vag en el directorio de entrada
    for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vag") {
            // Obtener la ruta completa del archivo .vag
            std::string inputFilePath = entry.path().string();

            // Crear la ruta de salida para el archivo convertido (en .mp3)
            std::string outputFilePath = outputDirectory + entry.path().stem().string() + ".mp3";

            // Llamar a la función para convertir el archivo
            convertVagToAudio(inputFilePath, outputFilePath);
        }
    }

    return 0;
}


int main() {

    char selection;

    displayLogo();

    menu();

    std::cin >> selection;

    switch (selection) {
    case '1':
        DecompileMIPS();
        PSS_Processor();
        recomp_C();
        audio_extraction_test();
        audioOut();
        break;
    case '2':
        DecompileMIPS();
        break;
    case '3':
        recomp_C();
        break;
    case '4':
        PSS_Processor();
        break;
    case '5':
        audio_extraction_test();
        audioOut();
        break;
    case 'q':
        break;
    default:
        std::cout << "Invalid option. Please try again.\n";
        break;
    }

    std::cout << "Proceso finalizado." << std::endl;

    return 0;

}

