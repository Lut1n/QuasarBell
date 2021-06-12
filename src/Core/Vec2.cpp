#include "Core/Vec2.h"

#include <cmath>
#include <iostream>
#include <cassert>

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

const vec2& vec2::operator-=(const vec2& v)
{
    *this = *this - v;
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

Rect Rect::extends(const vec2& pt) const
{
    auto minf = [](float a, float b) -> float{
        return a < b ? a : b; };
    auto maxf = [](float a, float b) -> float{
        return a > b ? a : b; };
    
    Rect ret;
    ret.p0.x = minf(p0.x,pt.x);
    ret.p0.y = minf(p0.y,pt.y);
    ret.p1.x = maxf(p1.x,pt.x);
    ret.p1.y = maxf(p1.y,pt.y);
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




Kernel::Kernel(int w, int h) : w(w), h(h)
{
    data.resize(w*h);
}

Kernel::Kernel(std::initializer_list<float> initData)
{
    size_t size = std::sqrt(initData.size());
    w = size;
    h = size;
    data.resize(w*h);
    assert(data.size() == initData.size());
    data = initData;
}

float& Kernel::operator()(size_t x, size_t y)
{
    assert(x<w && y <h);
    size_t index = h*x + y;
    return data[index];
}

float Kernel::operator()(size_t x, size_t y) const
{
    assert(x<w && y <h);
    size_t index = h*x + y;
    return data[index];
}

void Kernel::operator=(std::initializer_list<float> initData)
{
    assert(data.size() == initData.size());
    data = initData;
}

Kernel Kernel::resize(size_t nw, size_t nh)
{
    auto& src = *this;
    Kernel ret(nw,nh);
    int oft_x = (nw-w)/2;
    int oft_y = (nh-h)/2;

    for(size_t y=0; y<h; ++y)
    {
        for(size_t x=0; x<w; ++x)
        {
            int yy = y+oft_y;
            int xx = x+oft_x;
            if(xx>=0 && xx<nw && yy>=0 && yy<nh)
                ret(xx,yy) = src(x,y);
        }
    }
    return ret;
}


void Kernel::display(const Kernel& k)
{
    float total = 0.0;
    std::cout << "kernel[" << k.w << ";" << k.h << "]" << std::endl;
    for(size_t y=0; y<k.h; ++y)
    {
        for(size_t x=0; x<k.w; ++x)
        {
            std::cout << k(x,y) << ";";
            total += k(x,y);
        }
        std::cout << std::endl;
    }
    std::cout << "total: " << total << std::endl;
}

Kernel Kernel::convProduct(const Kernel& k1, const Kernel& k2)
{
    size_t w = k2.w + k1.w - 1;
    size_t h = k2.h + k1.h - 1;
    Kernel ret(w,h);

    size_t oft_x1 = (k1.w-1)/2;
    size_t oft_y1 = (k1.h-1)/2;
    size_t oft_x2 = k1.w-1;
    size_t oft_y2 = k1.h-1;
    for(size_t y=0; y<ret.h; ++y)
    {
        for(size_t x=0; x<ret.w; ++x)
        {
            ret(x,y) = 0.0f;
            for(size_t y1=0; y1<k1.h; ++y1)
            {
                for(size_t x1=0; x1<k1.w; ++x1)
                {
                    float v2 = 0.0;
                    float v1 = 0.0;
                    int yy = y+y1-oft_y1;
                    int xx = x+x1-oft_x1;
                    int y2 = y+y1-oft_y2;
                    int x2 = x+x1-oft_x2;

                    if(y2 >= 0 && y2 < k2.h && x2 >= 0 && x2 < k2.w)
                        v2 = k2(x2,y2);
                    if(yy >= 0 && yy < ret.h && xx >= 0 && xx < ret.w)
                        v1 = k1(x1,y1);
                    
                    ret(x,y) += v1*v2;
                }
            }
        }
    }
    return ret;
}