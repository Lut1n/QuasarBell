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

struct GlLineShader
{
    unsigned vertex_shader;
    unsigned fragment_shader;
    unsigned program;
    
    unsigned vpos_location;
    unsigned vCol_location;
    unsigned p1p2_location;
    unsigned thickness_location;
    
    GlLineShader();
    
    void use();
    
    void bindAttributes();
    
    void unbindAttributes();
    
    void setup(float* col, float* p1p2, float* thickness);
};

#endif // GL_QUAD_SHADER_H
