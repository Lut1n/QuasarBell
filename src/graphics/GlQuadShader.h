#ifndef GL_QUAD_SHADER_H
#define GL_QUAD_SHADER_H

struct GlQuadShader
{
    unsigned vertex_shader;
    unsigned fragment_shader;
    unsigned program;
    
    unsigned vpos_location;
    unsigned vCol_location;
    
    GlQuadShader();
    
    void use();
    
    void bindAttributes();
    
    void unbindAttributes();
    
    void setup(float* col);
};

#endif // GL_QUAD_SHADER_H
