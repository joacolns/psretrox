// mips_mapping.h
#ifndef MIPS_MAPPING_H
#define MIPS_MAPPING_H

#include <string>
#include <fstream>

// Declaración de la función que procesa una instrucción
void procesar_instruccion(const std::string& instruccion, std::ofstream& salida);

// Declaración de la función principal que convierte un archivo de log a C
void convertir_a_C(const std::string& archivo_log, const std::string& archivo_salida);

#endif // MIPS_MAPPING_H
