#ifndef ISO_READER_H
#define ISO_READER_H

#include <string>
#include <vector>

// Función para leer un archivo binario y devolver su contenido como un vector de bytes
std::vector<uint8_t> readFile(const std::string& filePath);

#endif
