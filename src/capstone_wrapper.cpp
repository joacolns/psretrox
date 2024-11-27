// En capstone_wrapper.cpp
#include <sstream>  
#include <iostream>
#include <capstone/capstone.h>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>

// Función para desensamblar el código y guardar el log
void disassembleCode(const std::vector<uint8_t>& binaryCode, const std::string& fileName) {
    // Obtener la hora actual
    std::time_t now = std::time(0);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);  // Usar localtime_s para seguridad

    // Formatear la fecha y hora
    std::stringstream timeStream;
    timeStream << std::put_time(&timeInfo, "%Y-%m-%d_%H-%M-%S");  // Formato de fecha y hora

    std::string timestamp = timeStream.str();
    std::string logFileName = "out/" + fileName + "_" + timestamp + ".log";  // Nombre del archivo log

    // Crear y abrir el archivo de log
    std::ofstream logFile(logFileName);
    if (!logFile) {
        std::cerr << "Error al abrir el archivo de log." << std::endl;
        return;
    }

    // Inicializar Capstone para la arquitectura x86 (ajusta según sea necesario)
    csh handle;
    cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle); // Cambia a CS_MODE_64 si es arquitectura de 64 bits
    if (err != CS_ERR_OK) {
        std::cerr << "Error al inicializar Capstone: " << cs_strerror(err) << std::endl;
        return;
    }

    // Desensamblar el código binario
    cs_insn* insn;
    size_t count = cs_disasm(handle, binaryCode.data(), binaryCode.size(), 0x1000, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            // Escribir la instrucción desensamblada en el archivo de log
            logFile << "0x" << std::hex << insn[i].address << ":\t" << insn[i].mnemonic << "\t" << insn[i].op_str << std::endl;
        }
        cs_free(insn, count);
    }
    else {
        std::cerr << "No se pudo desensamblar el código." << std::endl;
    }

    // Cerrar Capstone
    cs_close(&handle);

    // Cerrar el archivo de log
    logFile.close();
}
