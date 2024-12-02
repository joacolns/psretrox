#pragma once

#include <vector>
#include <string>

std::vector<std::string> getFilesInDirectory(const std::string& directory, const std::string& extension);
std::string readFileContent(const std::string& filePath);
void saveFileContent(const std::string& filePath, const std::string& content);
int renderGUI();