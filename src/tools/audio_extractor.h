#pragma once

#ifndef AUDIO_EXTRACTOR_H
#define AUDIO_EXTRACTOR_H

#include <string>

void extractAudioTracks(const std::string& musicMBPath, const std::string& musicMHPath, const std::string& outputDir);
int audio_extraction_test();

#endif // AUDIO_EXTRACTOR_H
