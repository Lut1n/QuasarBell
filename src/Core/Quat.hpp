#ifndef QUASAR_BELL_QUAT_HPP
#define QUASAR_BELL_QUAT_HPP

#include "Core/Math.hpp"

struct quat : public vec4
{
    using vec4::vec4; // inherit ctors

    explicit quat(const vec4& v);

    quat(vec4 axis, float angle);

    quat operator*(const quat& other) const;

    quat inverse() const;

    vec4 rotate(const vec4& v) const;
};

#endif // QUASAR_BELL_QUAT_HPP