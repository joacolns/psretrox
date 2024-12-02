#pragma once

#include <string>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int convertPSSToMP4(const std::string& inputFilePath, const std::string& outputFilePath);
int PSS_Processor();
void convertVagToAudio(const std::string& inputFilePath, const std::string& outputFilePath);
int audioOut();
