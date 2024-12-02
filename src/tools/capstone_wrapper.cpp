#include <sstream>  
#include <iostream>
#include <capstone/capstone.h>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <mips_mapping.h>
#include <iso_reader.h>

namespace fs = std::filesystem;

void disassembleCode(const std::vector<uint8_t>& binaryCode, const std::string& fileName) {
    /*
    std::time_t now = std::time(0);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);
    */

    std::string logFileName = "out/CrashTwinsanity/" + fileName + ".asm";

    std::ofstream logFile(logFileName);
    if (!logFile) {
        std::cerr << "Error opening assembly file." << std::endl;
        return;
    }

    // CAPSTONE ARQUITECTURE X86
    csh handle;
    cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle); // CS_MODE_64 IF 64BITS
    if (err != CS_ERR_OK) {
        std::cerr << "Error starting Capstone: " << cs_strerror(err) << std::endl;
        return;
    }

    cs_insn* insn;
    size_t count = cs_disasm(handle, binaryCode.data(), binaryCode.size(), 0x1000, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            //logFile << "0x" << std::hex << insn[i].address << ":\t" << insn[i].mnemonic << "\t" << insn[i].op_str << std::endl;
            logFile << insn[i].mnemonic << "\t" << insn[i].op_str << std::endl;
        }
        cs_free(insn, count);
    }
    else {
        std::cerr << "Can't disassemble the code." << std::endl;
    }

    cs_close(&handle);

    logFile.close();
}

int DecompileMIPS() {

    std::cout << "Disassembling Crash Twinsanity" << std::endl;  //<---- CRASH TWINSANITY TEST GAME

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

    std::vector<uint8_t> binaryCodeBD = readFile(filePathBD);
    std::vector<uint8_t> binaryCodeSLUS = readFile(filePathSLUS);
    std::vector<uint8_t> binaryCodeMUSIC_BD = readFile(filePathMUSIC_BD);
    std::vector<uint8_t> binaryCodeMUSIC_MH = readFile(filePathMUSIC_MH);
    std::vector<uint8_t> binaryCodeAMERICAN_MB = readFile(filePathAMERICAN_MB);
    std::vector<uint8_t> binaryCodeAMERICAN_MH = readFile(filePathAMERICAN_MH);
    std::vector<uint8_t> binaryCodeSYS = readFile(filePathSYS);

    if (binaryCodeBD.empty() && binaryCodeSLUS.empty() && binaryCodeMUSIC_BD.empty() && binaryCodeMUSIC_MH.empty() && binaryCodeAMERICAN_MB.empty() && binaryCodeAMERICAN_MH.empty() && binaryCodeSYS.empty()) {
        std::cerr << "Could not read any of the files." << std::endl;
        return 1;
    }

    if (!binaryCodeBD.empty()) {
        std::cout << "Disassembling .BD..." << std::endl;
        disassembleCode(binaryCodeBD, "CRASH.BD");
        std::cout << "Desensamblado Completado" << std::endl;
    }

    if (!binaryCodeSLUS.empty()) {
        std::cout << "Disassembling SLUS_XXX" << std::endl;
        disassembleCode(binaryCodeSLUS, "SLUS_209.09");
        std::cout << "Disassembly Completed" << std::endl;
    }

    if (!binaryCodeMUSIC_BD.empty()) {
        std::cout << "Disassembling MUSIC.MB..." << std::endl;
        disassembleCode(binaryCodeMUSIC_BD, "MUSIC.MB");
        std::cout << "Disassembly Completado" << std::endl;
    }

    if (!binaryCodeMUSIC_MH.empty()) {
        std::cout << "Disassembling MUSIC.MH..." << std::endl;
        disassembleCode(binaryCodeMUSIC_MH, "MUSIC.MH");
        std::cout << "Disassembly Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MB.empty()) {
        std::cout << "Disassembling AMERICAN.MB..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MB, "AMERICAN.MB");
        std::cout << "Disassembly Completado" << std::endl;
    }

    if (!binaryCodeAMERICAN_MH.empty()) {
        std::cout << "Disassembling AMERICAN.MH..." << std::endl;
        disassembleCode(binaryCodeAMERICAN_MH, "AMERICAN.MH");
        std::cout << "Disassembly Completado" << std::endl;
    }

    if (!binaryCodeSYS.empty()) {
        std::cout << "Disassembling SYSTEM.CNF..." << std::endl;
        disassembleCode(binaryCodeSYS, "SYSTEM.CNF");
        std::cout << "Disassembly Completado" << std::endl;
    }

    std::cout << "Processing files in the SYS folder (IRX)..." << std::endl;
    for (const auto& entry : fs::directory_iterator(IRXFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".IRX") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string();

            std::vector<uint8_t> binaryCodeIRX = readFile(filePath);
            if (!binaryCodeIRX.empty()) {
                std::cout << "Disassembling " << fileName << ".IRX..." << std::endl;
                disassembleCode(binaryCodeIRX, fileName + ".IRX");
                std::cout << "Disassembly Completado" << std::endl;
            }
            else {
                std::cerr << "Can't read file " << fileName << ".IRX" << std::endl;
            }

        }
    }


    return 0;
}

void recomp_C() {
    std::cout << "Recompiling..." << std::endl;

    std::string OUTFolder = "out/CrashTwinsanity";
    std::string RECOMPFolder = "out/CrashTwinsanity/C_recomp/";

    if (!fs::exists(OUTFolder) || !fs::is_directory(OUTFolder)) {
        std::cerr << "The folder " << OUTFolder << " doesn't exist." << std::endl;
        return;
    }

    if (!fs::exists(RECOMPFolder)) {
        std::cout << "The folder " << RECOMPFolder << " doesn't exist.  Creating it..." << std::endl;
        if (!fs::create_directory(RECOMPFolder)) {
            std::cerr << "Error creating the folder. " << RECOMPFolder << std::endl;
            return;
        }
    }

    for (const auto& entry : fs::directory_iterator(OUTFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".asm") {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().stem().string();

            std::cout << "Processing file: " << fileName << ".asm" << std::endl;

            std::string archivo_salida = RECOMPFolder + fileName + ".c";

            // Leer y desensamblar el archivo .log
            std::vector<uint8_t> assemblyfile = readFile(filePath);
            if (!assemblyfile.empty()) {
                std::cout << "Recompiling " << fileName << " to C..." << std::endl;
                convertir_a_C(filePath, archivo_salida); 
                std::cout << "Recompiled Completed for " << fileName << std::endl;
            }
            else {
                std::cerr << "Can't read the file " << fileName << ".asm" << std::endl;
            }
        }
    }
}
