#ifndef QUASAR_BELL_MATH_HPP
#define QUASAR_BELL_MATH_HPP

#include <cmath>

#undef min
#undef max

//--------------------------------------------------------------
namespace qb
{
    inline float log2(float v)
    {
        return (float)(std::log(v) / std::log(2.0));
    }

    //--------------------------------------------------------------
    inline float pow2(float v)
    {
        return (float)std::pow(2, v);
    }

    //--------------------------------------------------------------
    template<typename Ty>
    inline Ty min(Ty a, Ty b)
    {
        return a < b ? a : b;
    }

    //--------------------------------------------------------------
    template<typename Ty>
    inline Ty max(Ty a, Ty b)
    {
        return a > b ? a : b;
    }

};

struct vec4
{
    union {struct{float x,y,z,w;}; float v[4];};
};

#endif // QUASAR_BELL_MATH_HPP