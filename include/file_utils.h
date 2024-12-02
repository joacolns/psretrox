#pragma once

#include <string>
#include <vector>

std::vector<uint8_t> readBinaryFile(const std::string& filePath);

void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);
