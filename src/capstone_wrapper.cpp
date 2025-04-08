// En capstone_wrapper.cpp
#include <sstream>  
#include <iostream>
#include <capstone/capstone.h>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>

//Disassemble code and save a log
void disassembleCode(const std::vector<uint8_t>& binaryCode, const std::string& fileName) {
    //Get current time
    std::time_t now = std::time(0);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);  //Use localtime_s for thread safety

    /*
 
    TIME STREAM

    
    std::stringstream timeStream;
    timeStream << std::put_time(&timeInfo, "%Y-%m-%d_%H-%M-%S");  // Date adn time format

    std::string timestamp = timeStream.str();

    */

    std::string logFileName = "out/CrashTwinsanity/" + fileName + ".asm";  //Log filename

    //Create and open the asm file
    std::ofstream logFile(logFileName);
    if (!logFile) {
        std::cerr << "Could not open log." << std::endl;
        return;
    }

    //Init x86 architecture Capstone (adjust if needed)
    csh handle;
    cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle); //if the arch is 64bit, change it to CS_MODE_64
    if (err != CS_ERR_OK) {
        std::cerr << "Error when initializing Capstone" << cs_strerror(err) << std::endl;
        return;
    }

    //Binary code disassembly
    cs_insn* insn;
    size_t count = cs_disasm(handle, binaryCode.data(), binaryCode.size(), 0x1000, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            //log the disassembly instructions
            //logFile << "0x" << std::hex << insn[i].address << ":\t" << insn[i].mnemonic << "\t" << insn[i].op_str << std::endl;
            logFile << insn[i].mnemonic << "\t" << insn[i].op_str << std::endl;
        }
        cs_free(insn, count);
    }
    else {
        std::cerr << "Code was not disassembled." << std::endl;
    }

    //Close capstone handler
    cs_close(&handle);

    //Close the log file
    logFile.close();
}
