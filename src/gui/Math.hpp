#ifndef QUASAR_BELL_MATH_HPP
#define QUASAR_BELL_MATH_HPP

#include <cmath>

//--------------------------------------------------------------
inline float log2(float v)
{
    return std::log(v) / std::log(2.0);
}

//--------------------------------------------------------------
inline float pow2(float v)
{
    return std::pow(2, v);
}

#endif // QUASAR_BELL_MATH_HPP