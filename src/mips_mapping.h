// mips_mapping.h
#ifndef MIPS_MAPPING_H
#define MIPS_MAPPING_H

#include <string>
#include <fstream>

//Function to process a single instruction
void processInstruction(const std::string& instruction, std::ofstream& output);

//Function to convert a log file to C
void converToC(const std::string& logFile, const std::string& outputFile);

#endif // MIPS_MAPPING_H
