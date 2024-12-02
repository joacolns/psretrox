#ifndef MIPS_MAPPING_H
#define MIPS_MAPPING_H

#include <string>
#include <fstream>

void procesar_instruccion(const std::string& instruccion, std::ofstream& salida);

void convertir_a_C(const std::string& archivo_log, const std::string& archivo_salida);

#endif // MIPS_MAPPING_H
