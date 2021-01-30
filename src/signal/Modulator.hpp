#ifndef QUASAR_BELL_MODULATOR_HPP
#define QUASAR_BELL_MODULATOR_HPP

#include <list>

#include "signal/PcmData.hpp"

//--------------------------------------------------------------
class Modulator
{
public:
    static PcmData silent(float duration);
    static PcmData attack(float freq, float duration, float ampl = 1.0f, float ampl2 = 0.5f, float oft_t = 0.0f);
    static PcmData release(float freq, float duration, float ampl = 1.0f, float oft_t = 0.0f);
    static PcmData generate(float freq, float duration, float ampl = 1.0f, float slope_time = 0.0f, float oft_t = 0.0f);

    static PcmData mix(const std::list<PcmData>& inputStream, float duration);
};

#endif // QUASAR_BELL_MODULATOR_HPP
