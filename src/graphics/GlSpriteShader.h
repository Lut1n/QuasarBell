#ifndef GL_SPRITE_SHADER_H
#define GL_SPRITE_SHADER_H

struct GlSpriteShader
{
    unsigned vertex_shader;
    unsigned fragment_shader;
    unsigned program;
    
    // unsigned mvp_location;
    unsigned vpos_location;
    unsigned uv_location;
    unsigned tex_location;
    unsigned vCol_location;
    
    GlSpriteShader();
    
    void bindAttributes();
    
    void unbindAttributes();
    
    void use();
    
    void setup(float* col, unsigned /*tex*/);
};

struct GlRoundedShader
{
    unsigned vertex_shader;
    unsigned fragment_shader;
    unsigned program;
    
    // unsigned mvp_location;
    unsigned vpos_location;
    unsigned uv_location;
    unsigned uv1_location;
    unsigned uv2_location;
    unsigned radius_location;
    unsigned vCol_location;
    
    GlRoundedShader();
    
    void bindAttributes();
    
    void unbindAttributes();
    
    void use();
    
    void setup(float* col, float radius);
};

#endif // GL_SPRITE_SHADER_H
