#pragma once

#include <string>
#include <vector>

// Función para leer un archivo binario en un vector de bytes
std::vector<uint8_t> readBinaryFile(const std::string& filePath);

// Función para escribir un archivo binario
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);
