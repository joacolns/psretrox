#ifndef MODEL_EXTRACTOR_H
#define MODEL_EXTRACTOR_H

#include <string>
#include <vector>

// Función para escribir un archivo binario
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);

// Función para extraer bloques de CRASH.BD y CRASH.BH
void extractModels(const std::string& crashBHPath, const std::string& crashBDPath, const std::string& outputDir);

#endif // MODEL_EXTRACTOR_H
