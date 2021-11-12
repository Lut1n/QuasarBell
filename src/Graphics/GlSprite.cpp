#include "Graphics/GlSprite.h"

#include "Graphics/GlError.h"

GlSprite::GlSprite()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    // vec2 * 6 = 12 float * 2(uv) = 24
    float vertices[24] = 
    {
        0.0, 0.0,
        0.0, 0.0,
        
        1.0, 0.0,
        1.0, 0.0,
        
        1.0, 1.0,
        1.0, 1.0,
        
        1.0, 0.0,
        1.0, 0.0,
        
        1.0, 1.0,
        1.0, 1.0,
        
        0.0, 1.0,
        0.0, 1.0,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    GL_CHECKERROR("sprite buffer data");
}

void GlSprite::update(const vec2& position, const vec2& size, const vec2& uv0, const vec2& uv1, const vec2& viewport, bool yVertInverted)
{
    vec2 pts[4] = {u_p0, u_p1, u_p2, u_p3};
    for(auto& pt : pts)
    {
        pt = pt * size;
        pt += position;
        
        pt = pt / viewport;
        pt = pt * 2.0 - vec2(1.0,1.0);
        if(yVertInverted)
            pt = pt * vec2(1.0,-1.0);
    }
    
    float vertices[24] = 
    {
        pts[0].x,pts[0].y,
        uv0.x, uv0.y,
        
        pts[1].x,pts[1].y,
        uv1.x, uv0.y,
        
        pts[2].x,pts[2].y,
        uv1.x, uv1.y,
        
        pts[0].x,pts[0].y,
        uv0.x, uv0.y,
        
        pts[2].x,pts[2].y,
        uv1.x, uv1.y,
        
        pts[3].x,pts[3].y,
        uv0.x, uv1.y
    };
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // std::cout << "bind GL_ARRAY_BUFFER " << vertexBuffer << std::endl;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    GL_CHECKERROR("sprite buffer subdata");
}

void GlSprite::draw()
{
    glBindVertexArray(vao);
    GL_CHECKERROR("sprite bind vao");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    GL_CHECKERROR("sprite bind vbo");
    glDrawArrays(GL_TRIANGLES, 0, 6);
    GL_CHECKERROR("sprite draw");
}
