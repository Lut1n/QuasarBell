#ifndef GL_SPRITE_H
#define GL_SPRITE_H

#include "Core/Vec2.h"

struct GlSprite
{
    unsigned vao = 0;
    unsigned vertexBuffer = 0;

    vec2 u_p0 = vec2(0.0,0.0);
    vec2 u_p1 = vec2(1.0,0.0);
    vec2 u_p2 = vec2(1.0,1.0);
    vec2 u_p3 = vec2(0.0,1.0);
    
    GlSprite();
    
    void update(const vec2& position, const vec2& size, const vec2& uv0, const vec2& uv1, const vec2& viewport);
    
    void draw();
};

#endif // GL_SPRITE_H
