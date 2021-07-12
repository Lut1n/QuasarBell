#include "Core/Quat.hpp"

#include <iostream>
#include <cmath>

quat::quat(const vec4& v)
{
    for(size_t i=0;i<4;++i) this->v[i] = v.v[i];
}

quat::quat(vec4 axis, float angle)
{
    float co = std::cos(angle * 0.5f);
    float si = std::sin(angle * 0.5f);
    axis.normalize();
    x = axis.x * si;
    y = axis.y * si;
    z = axis.z * si;
    w = co;
}

quat quat::operator*(const quat& other) const
{
    const quat& q1 = *this;
    const quat& q2 = other;

    return quat(
        q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
        q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
        q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
        q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z
    );
}

quat quat::inverse() const
{
    return quat(-x,-y,-z,w);
}

vec4 quat::rotate(const vec4& v) const
{
    const quat& q = *this;
    const quat qi = inverse();
    quat p(v.x,v.y,v.z,0.0f);
    return q * (p * qi);
}