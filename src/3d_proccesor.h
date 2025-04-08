#pragma once

#ifndef MODEL_EXTRACTOR_H
#define MODEL_EXTRACTOR_H

#include <string>
#include <vector>

//read a binary file in a byte vector
//std::vector<uint8_t> readBinaryFile(const std::string& filePath);

//Write a binary file from a byte vector
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);

//Block extraction using CRASH.BD and CRASH.BH
void extractModels(const std::string& crashBHPath, const std::string& crashBDPath, const std::string& outputDir);

#endif // MODEL_EXTRACTOR_H
