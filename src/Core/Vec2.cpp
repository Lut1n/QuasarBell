#include "Core/Vec2.h"

#include <cmath>

float mix(float a, float b, float f)
{
    return (b-a)*f + a;
}

vec2::vec2()
{
}

vec2::vec2(const vec2& v)
    : x(v.x)
    , y(v.y)
{
}

vec2::vec2(float _x, float _y)
    : x(_x)
    , y(_y)
{
}

const vec2& vec2::operator=(const vec2& v)
{
    x = v.x;
    y = v.y;
    return *this;
}

const vec2& vec2::operator+=(const vec2& v)
{
    *this = *this + v;
    return *this;
}

vec2 vec2::operator+(const vec2& v) const
{
    return vec2(x+v.x, y+v.y);
}

vec2 vec2::operator-(const vec2& v) const
{
    return vec2(x-v.x, y-v.y);
}

vec2 vec2::operator*(const vec2& v) const
{
    return vec2(x*v.x, y*v.y);
}

vec2 vec2::operator*(float s) const
{
    return vec2(x*s, y*s);
}

vec2 vec2::operator/(const vec2& v) const
{
    return vec2(x/v.x, y/v.y);
}

vec2 vec2::operator/(float s) const
{
    return vec2(x/s, y/s);
}

float vec2::operator[](unsigned i) const
{
    const float* ptr = &x;
    return ptr[i];
}

float& vec2::operator[](unsigned i)
{
    float* ptr = &x;
    return ptr[i];
}

float vec2::length() const
{
    return std::sqrt(length2());
}

float vec2::length2() const
{
    return dot(*this);
}

float vec2::dot(const vec2& v) const
{
    return x*v.x + y*v.y;
}

float vec2::angle() const
{
    return std::atan2(y,x);
}

vec2 vec2::fromAngle(float a)
{
    return vec2(std::cos(a),std::sin(a));
}

vec2 vec2::rotate(float a) const
{
    vec2 ret;
    ret.x = std::cos(a)*x + std::sin(a)*y;
    ret.y = -std::sin(a)*x + std::cos(a)*y;
    return ret;
}



Rect Rect::fromPosAndSize(const vec2& p, const vec2& s)
{
    return Rect{p, p+s};
}

Rect Rect::fromAABB(const vec2& p0, const vec2& p1)
{
    return Rect{p0, p1};
}

vec2 Rect::size() const
{
    return p1 - p0;
}

Rect Rect::operator+(const vec2& oft) const
{
    return Rect{p0+oft, p1+oft};
}

Rect Rect::clampTo(const Rect& other) const
{
    Rect ret = *this;
    if (p0.x < other.p0.x) ret.p0.x = other.p0.x;
    if (p0.y < other.p0.y) ret.p0.y = other.p0.y;
    if (p1.x > other.p1.x) ret.p1.x = other.p1.x;
    if (p1.y > other.p1.y) ret.p1.y = other.p1.y;
    return ret;
}

Rect Rect::extends(const Rect& other) const
{
    auto minf = [](float a, float b) -> float{
        return a < b ? a : b; };
    auto maxf = [](float a, float b) -> float{
        return a > b ? a : b; };
    
    Rect ret;
    ret.p0.x = minf(p0.x,other.p0.x);
    ret.p0.y = minf(p0.y,other.p0.y);
    ret.p1.x = maxf(p1.x,other.p1.x);
    ret.p1.y = maxf(p1.y,other.p1.y);
    return ret;
}

// axis-aligned bounding box (AABB)
bool Rect::outside(const vec2& pt) const
{
    return pt.x<p0.x || p1.x<pt.x || pt.y<p0.y || p1.y<pt.y;
}

bool Rect::inside(const vec2& pt) const
{
    return !outside(pt);
}

bool Rect::outside(const Rect& other) const
{
    bool out = other.p0.x > p1.x || other.p1.x < p0.x;
    out = out || other.p0.y > p1.y || other.p1.y < p0.y;
    return out;
}

bool Rect::inside(const Rect& other) const
{
    return inside(other.p0) && inside(other.p1);
}

bool Rect::over(const Rect& other) const
{
    return !outside(other);
}
