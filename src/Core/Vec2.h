#ifndef VEC2_H
#define VEC2_H

#include <vector>
#include <initializer_list>

float mix(float a, float b, float f);

struct vec2
{
    float x = 0.0f;
    float y = 0.0f;
    
    vec2();
    vec2(const vec2& v);
    vec2(float _x, float _y);
    
    const vec2& operator=(const vec2& v);
    
    const vec2& operator+=(const vec2& v);
    const vec2& operator-=(const vec2& v);
    
    vec2 operator+(const vec2& v) const;
    vec2 operator-(const vec2& v) const;
    
    vec2 operator*(const vec2& v) const;
    vec2 operator*(float s) const;
    
    vec2 operator/(const vec2& v) const;
    vec2 operator/(float s) const;
    
    float operator[](unsigned i) const;
    float& operator[](unsigned i);
    
    float length() const;
    float length2() const;
    
    float dot(const vec2& v) const;
    float angle() const;
    
    static vec2 fromAngle(float a);
    
    vec2 rotate(float a) const;
};

struct Rect
{
    vec2 p0, p1;
    
    static Rect fromPosAndSize(const vec2& p, const vec2& s);
    static Rect fromAABB(const vec2& p0, const vec2& p1);
    
    vec2 size() const;
    
    Rect operator+(const vec2& oft) const;
    Rect clampTo(const Rect& other) const;
    Rect extends(const Rect& other) const;

    bool outside(const vec2& pt) const;
    bool inside(const vec2& pt) const;
    bool outside(const Rect& other) const;
    bool inside(const Rect& other) const;
    bool over(const Rect& other) const;
};





struct Kernel
{
    int w=0,h=0;
    std::vector<float> data;

    Kernel(int w, int h);
    Kernel(std::initializer_list<float> initData);

    float& operator()(size_t x, size_t y);
    float operator()(size_t x, size_t y) const;

    void operator=(std::initializer_list<float> initData);

    Kernel resize(size_t nw, size_t nh);

    static void display(const Kernel& k);
    static Kernel convProduct(const Kernel& k1, const Kernel& k2);
};

#endif // VEC2_H
