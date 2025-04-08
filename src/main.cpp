#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <string>

#include "iso_reader.h"
#include "capstone_wrapper.h"
#include "mips_mapping.h"
#include "audio_extractor.h"
#include "3d_proccesor.h"

namespace fs = std::filesystem;
void processPSSFiles(const std::string& folderPath, const std::string& outputDir);

void displayLogo() {
    //Red color escape code
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
    std::cout << reset; //Restore terminal color
}



void menu() {
        std::cout << "1 - Decompile files and recompile to C" << std::endl;
        std::cout << "2 - Decompile to MIPS assembly" << std::endl;
        std::cout << "3 - Recompile to C" << std::endl;
        std::cout << "4 - Decompile 3D models" << std::endl;
        std::cout << "5 - Decompile cutscenes" << std::endl;
        std::cout << "6 - Decompile audios" << std::endl;
        std::cout << "q - Quit" << std::endl;
        std::cout << "\n";
}

int convertPSSToMP4(const std::string& inputFilePath, const std::string& outputFilePath) {
    //.PSS to .MP4 conversion preprocessing with FFmpeg
    std::string command = "ffmpeg -i " + inputFilePath + " -vcodec libx264 -acodec aac " + outputFilePath;

    //Execute ffmpeg command
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error when converting the file." << std::endl;
        return -1;
    }

    std::cout << "Converting done." << outputFilePath << std::endl;

    return 0;
}

void recomp_C() {
    std::cout << "Recompiling..." << std::endl;

    std::string OUTFolder = "out/CrashTwinsanity";
    std::string RECOMPFolder = "out/CrashTwinsanity/C_recomp/";

    //Check if the folder exists.
    if (!fs::exists(OUTFolder) || !fs::is_directory(OUTFolder)) {
        std::cerr << "The folder " << OUTFolder << " does not exist or it's not a valid directory." << std::endl;
        return;
    }

    if (!fs::exists(RECOMPFolder)) {
        std::cout << "The folder " << RECOMPFolder << " does not exist. Creating it..." << std::endl;
        if (!fs::create_directory(RECOMPFolder)) {
            std::cerr << "Could not create the folder... " << RECOMPFolder << std::endl;
            return;
        }
    }

    for (const auto& entry : fs::directory_iterator(OUTFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".asm") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); //filename (without the extension)

            std::cout << "Processing file: " << fileName << ".asm" << std::endl;

            //Make a new output file with the same name as the .asm file
            std::string archivo_salida = RECOMPFolder + fileName + ".c";

            //Read and disassemble the .asm (log) file
            std::vector<uint8_t> assemblyfile = readFile(filePath);
            if (!assemblyfile.empty()) {
                std::cout << "Recompiling " << fileName << " to C..." << std::endl;
                convertir_a_C(filePath, archivo_salida);  //Dinamic mode activated
                std::cout << "Finishing in... " << fileName << std::endl;
            }
            else {
                std::cerr << "Could not read the file " << fileName << ".log" << std::endl;
            }
        }
    }
}

int DecompileMIPS() {

    std::cout << "Disassembling Crash Twinsanity" << std::endl;

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

    //Read binary files
    std::vector<uint8_t> binaryCodeBD = readFile(filePathBD);
    std::vector<uint8_t> binaryCodeSLUS = readFile(filePathSLUS);
    std::vector<uint8_t> binaryCodeMUSIC_BD = readFile(filePathMUSIC_BD);
    std::vector<uint8_t> binaryCodeMUSIC_MH = readFile(filePathMUSIC_MH);
    std::vector<uint8_t> binaryCodeAMERICAN_MB = readFile(filePathAMERICAN_MB);
    std::vector<uint8_t> binaryCodeAMERICAN_MH = readFile(filePathAMERICAN_MH);
    std::vector<uint8_t> binaryCodeSYS = readFile(filePathSYS);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty() && binaryCodeMUSIC_BD.empty() && binaryCodeMUSIC_MH.empty() && binaryCodeAMERICAN_MB.empty() && binaryCodeAMERICAN_MH.empty() && binaryCodeSYS.empty()) {
        std::cerr << "No binary file could be read." << std::endl;
        return 1;
    }

    // Desensamblar los archivos
    if (!binaryCodeBD.empty()) {
        std::cout << "Disassembling CRASH.BD." << std::endl;
        disassembleCode(binaryCodeBD, "CRASH.BD");
        std::cout << "CRASH.BD disassembling completed!" << std::endl;
    }

    if (!binaryCodeSLUS.empty()) {
        std::cout << "Disassembling SLUS_209.09..." << std::endl;
        disassembleCode(binaryCodeSLUS, "SLUS_209.09");
        std::cout << "SLUS_209.09 disassembling completed!" << std::endl;
    }

    if (!binaryCodeMUSIC_BD.empty()) {
        std::cout << "Disassembling MUSIC.MB..." << std::endl;
        disassembleCode(binaryCodeMUSIC_BD, "MUSIC.MB");
        std::cout << "MUSIC.MB disassembling completed!" << std::endl;
    }

    if (!binaryCodeMUSIC_MH.empty()) {
        std::cout << "Disassembling MUSIC.MH...." << std::endl;
        disassembleCode(binaryCodeMUSIC_MH, "MUSIC.MH");
        std::cout << "MUSIC.MH disassembling completed!" << std::endl;
    }

    if (!binaryCodeAMERICAN_MB.empty()) {
        std::cout << "Disassembling AMERICAN.MB..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MB, "AMERICAN.MB");
        std::cout << "AMERICAN.MB disassembling completed!" << std::endl;
    }

    if (!binaryCodeAMERICAN_MH.empty()) {
        std::cout << "Disassembling AMERICAN.MH..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MH, "AMERICAN.MH");
        std::cout << "AMERICAN.MH disassembling completed!" << std::endl;
    }

    if (!binaryCodeSYS.empty()) {
        std::cout << "Disassembling SYSTEM.CNF..." << std::endl;
        disassembleCode(binaryCodeSYS, "SYSTEM.CNF");
        std::cout << "SYSTEM.CNF disassembling completed!" << std::endl;
    }

    std::cout << "Processing files in SYS (IRX)" << std::endl;
    for (const auto& entry : fs::directory_iterator(IRXFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".IRX") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); //filename without the extension

            //Read and disassemble the .PSS file
            std::vector<uint8_t> binaryCodeIRX = readFile(filePath);
            if (!binaryCodeIRX.empty()) {
                std::cout << "Disassembling " << fileName << ".IRX..." << std::endl;
                disassembleCode(binaryCodeIRX, fileName + ".IRX");
                std::cout << fileName << ".IRX disassembling completed!" << std::endl;
            }
            else {
                std::cerr << "Could not read" << fileName << ".IRX" << std::endl;
            }

        }
    }


    return 0;
}

int PSS_Processor() {
    std::string fmvFolder = "iso/FMV"; // Save .PSS path
    std::string fmvFolderBONS = "iso/FMV/BONUS"; // <-------- ONLY FOR CRASH BANDICOOT TWINSANITY
    std::string outputDir = "out/CrashTwinsanity/videos";

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Process .PSS files in the FMV folder
    std::cout << "Processing FMV files... " << std::endl;
    processPSSFiles(fmvFolder, outputDir);

    // Process .PSS files in the BONUS folder
    std::cout << "Processing BONUS folder" << std::endl;
    processPSSFiles(fmvFolderBONS, outputDir);

    return 0;
}

void processPSSFiles(const std::string& folderPath, const std::string& outputDir) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".PSS") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string(); // filename without extension

            // Read and disassemble the .PSS file
            std::vector<uint8_t> binaryCodePSS = readFile(filePath);
            if (!binaryCodePSS.empty()) {
                std::cout << "Disassembling " << fileName << ".PSS..." << std::endl;
                disassembleCode(binaryCodePSS, fileName + ".PSS");
                std::cout << fileName << ".PSS disassembling completed!" << std::endl;
            } else {
                std::cerr << "Could not read " << fileName << ".PSS" << std::endl;
            }

            std::string inputFilePath = folderPath + "/" + fileName + ".PSS";  // Path to the .PSS file
            std::string outputFilePathMP4 = outputDir + "/" + fileName + ".mp4";  // Path to the .MP4 file

            convertPSSToMP4(inputFilePath, outputFilePathMP4);
        }
    }
}

int audio_extraction_test() {
    try {
        //Music.MB and Music.MH paths
        std::string musicMBPath = "iso/CRASH6/MUSIC.MB";
        std::string musicMHPath = "iso/CRASH6/MUSIC.MH";

        //Output directory
        std::string outputDir = "out/CrashTwinsanity/audios";

        if (!std::filesystem::exists(outputDir)) {
            std::filesystem::create_directory(outputDir);
        }

        //Extract tracks
        extractAudioTracks(musicMBPath, musicMHPath, outputDir);
        std::cout << "Extraction complete! Files saved to: " << outputDir << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void convertVagToAudio(const std::string& inputFilePath, const std::string& outputFilePath) {
    //vgmstream-cli path
    std::string vgmstreamPath = "C:/VisualStudioLibraries/vgmstream/vgmstream-cli.exe";
    std::string command = vgmstreamPath + " -o " + outputFilePath + " " + inputFilePath;

    //Execute command
    int result = std::system(command.c_str());

    //Check if the command was successful
    if (result == 0) {
        std::cout << "Converted files " << inputFilePath << " saved in " << outputFilePath << std::endl;
    }
    else {
        std::cerr << "There was an error converting the files. " << inputFilePath << std::endl;
    }
}

int audioOut() {
    //.vag files input path
    std::string inputDirectory = "out/CrashTwinsanity/audios";

    //.vag files output path
    std::string outputDirectory = "out/CrashTwinsanity/audios";

    //Check if the output directory exists or if it needs to be created.
    if (!std::filesystem::exists(outputDirectory)) {
        std::filesystem::create_directory(outputDirectory);
    }

    //Loop through the input directory and convert each .vag file to .mp3
    for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vag") {
            //path to .vag file
            std::string inputFilePath = entry.path().string();

            //output path to .mp3 file
            std::string outputFilePath = outputDirectory + entry.path().stem().string() + ".mp3";

            //convert .vag to .mp3
            convertVagToAudio(inputFilePath, outputFilePath);
        }
    }

    return 0;
}

int tdproccesor() {
    std::string crashBHPath = "iso/CRASH6/CRASH.BH";
    std::string crashBDPath = "iso/CRASH6/CRASH.BD";
    std::string outputDir = "out/CrashTwinsanity/3D";

    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directory(outputDir);
    }

    try {
        extractModels(crashBHPath, crashBDPath, outputDir);
        std::cout << "Extracción y conversión de modelos completada." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
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
		tdproccesor();
        break;
    case '2':
        DecompileMIPS();
        break;
    case '3':
        recomp_C();
        break;
    case '4':
        tdproccesor();
        break;
    case '5':
        PSS_Processor();
        break;
    case '6':
        audio_extraction_test();
        audioOut();
        break;
    case 'q':
        break;
    default:
        std::cout << "Invalid option. Please try again.\n";
        break;
    }


    std::cout << "Finished processing." << std::endl;

    return 0;

}

