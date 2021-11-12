#ifndef GL_QUAD_H
#define GL_QUAD_H

#include "Core/Vec2.h"

struct GlQuad
{
    unsigned vao = 0;
    unsigned vertexBuffer = 0;

    vec2 u_p0 = vec2(0.0,0.0);
    vec2 u_p1 = vec2(1.0,0.0);
    vec2 u_p2 = vec2(1.0,1.0);
    vec2 u_p3 = vec2(0.0,1.0);
    
    GlQuad();
    
    void update(const vec2& position, const vec2& size, const vec2& origin, float rotation, const vec2& viewport);
    
    void draw();
};

#endif // GL_QUAD_H
