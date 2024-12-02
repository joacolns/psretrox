#ifndef MODEL_EXTRACTOR_H
#define MODEL_EXTRACTOR_H

#include <string>
#include <vector>


void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);
void extractModels(const std::string& crashBHPath, const std::string& crashBDPath, const std::string& outputDir);
int proccesor();

#endif
