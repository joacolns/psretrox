#ifndef CAPSTONE_WRAPPER_H
#define CAPSTONE_WRAPPER_H

#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void disassembleCode(const std::vector<uint8_t>& binaryCode, const std::string& fileName);
int DecompileMIPS();
void recomp_C();

#endif
