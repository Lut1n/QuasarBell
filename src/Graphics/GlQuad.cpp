#include "Graphics/GlQuad.h"

#include "Graphics/GlError.h"

GlQuad::GlQuad()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    // vec2 * 6 = 12 float
    float vertices[12] = 
    {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    GL_CHECKERROR("quad buffer data");
}

void GlQuad::update(const vec2& position, const vec2& size, const vec2& origin, float rotation, const vec2& viewport)
{
    vec2 pts[4] = {u_p0, u_p1, u_p2, u_p3};
    for(auto& pt : pts)
    {
        pt = pt * size;
        pt = pt - origin;
        pt = pt.rotate(-rotation);
        pt += position;
        
        pt = pt / viewport;
        pt = pt * 2.0 - vec2(1.0,1.0);
        pt = pt * vec2(1.0,-1.0);
    }
    
    float vertices[12] = 
    {
        pts[0].x,pts[0].y,
        pts[1].x,pts[1].y,
        pts[2].x,pts[2].y,
        pts[0].x,pts[0].y,
        pts[2].x,pts[2].y,
        pts[3].x,pts[3].y
    };
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // std::cout << "quad bind GL_ARRAY_BUFFER " << vertexBuffer << std::endl;
    GL_CHECKERROR("quad bind");
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    GL_CHECKERROR("quad buffer subdata");
}

void GlQuad::draw()
{
    glBindVertexArray(vao);
    GL_CHECKERROR("quad bind vao");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    GL_CHECKERROR("quad bind vbo");
    glDrawArrays(GL_TRIANGLES, 0, 6);
    GL_CHECKERROR("quad draw");
}