#ifndef QUASAR_BELL_WAVEXPORTER_HPP
#define QUASAR_BELL_WAVEXPORTER_HPP

#include <iostream>

#include "signal/PcmData.hpp"

//--------------------------------------------------------------
class WavExporter
{
public:
    static bool exportAsWAV(const std::string& filename, const PcmDataBase& pcm);
};

#endif // QUASAR_BELL_WAVEXPORTER_HPP



