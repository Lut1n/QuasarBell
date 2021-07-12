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
    
    vec4()
        : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
        {}

    vec4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w)
        {}

    vec4(float* fv)
    {
        for(size_t i=0;i<4;++i) v[i] = fv[i];
    }

    void normalize()
    {
        float len = length();
        x /= len;
        y /= len;
        z /= len;
    }

    float length()
    {
        return std::sqrt(x*x+y*y+z*z);
    }
};

#endif // QUASAR_BELL_MATH_HPP