#pragma once

#ifndef AUDIO_EXTRACTOR_H
#define AUDIO_EXTRACTOR_H

#include <string>

//Extract tracks from MUSIC.MB y MUSIC.MH
void extractAudioTracks(const std::string& musicMBPath, const std::string& musicMHPath, const std::string& outputDir);

#endif // AUDIO_EXTRACTOR_H
