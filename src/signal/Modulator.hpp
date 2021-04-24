#ifndef QUASAR_BELL_MODULATOR_HPP
#define QUASAR_BELL_MODULATOR_HPP

#include <list>

#include "signal/PcmData.hpp"

//--------------------------------------------------------------
class Modulator
{
public:
    static qb::Pcm16 silent(float duration);
    static qb::Pcm16 attack(float freq, float duration, float ampl = 1.0f, float ampl2 = 0.5f, float oft_t = 0.0f);
    static qb::Pcm16 release(float freq, float duration, float ampl = 1.0f, float oft_t = 0.0f);
    static qb::Pcm16 generate(float freq, float duration, float ampl = 1.0f, float slope_time = 0.0f, float oft_t = 0.0f);

    static qb::Pcm16 mix(const std::list<qb::Pcm16>& inputStream, float duration);
};

#endif // QUASAR_BELL_MODULATOR_HPP
