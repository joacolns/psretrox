#include "iso_reader.h"
#include <iostream>
#include <fstream>

std::vector<uint8_t> readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::vector<uint8_t> buffer;

    if (file) {
        file.seekg(0, std::ios::end);  //Find and go to the end of the file
        size_t fileSize = file.tellg(); //Get file size
        file.seekg(0, std::ios::beg);   //Find and go to the beginning of the file

        buffer.resize(fileSize); //Resize the buffer to the file size
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize); //Read the file into the buffer
    }
    else {
        std::cerr << "Error when opening the file: " << filePath << std::endl;
    }

    return buffer;
}