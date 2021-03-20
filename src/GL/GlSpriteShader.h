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

#endif // GL_SPRITE_SHADER_H
