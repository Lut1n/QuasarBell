#ifndef GL_CUSTOM_PROGRAM_H
#define GL_CUSTOM_PROGRAM_H

#include <string>
#include <vector>

#include "Core/Math.hpp"

struct GlCustomProgram
{
    unsigned vertex_shader;
    unsigned fragment_shader;
    unsigned program;
    
    // std::vector<unsigned> uniformLocations;
    // unsigned mvp_location;
    unsigned vpos_location;
    unsigned uv_location;
    // unsigned tex_location;

    bool bindUv = false;
    std::string fragShaderCode;

    GlCustomProgram();
    ~GlCustomProgram();

    void resetFragCode();
    void setFragCode(const std::string& fragCode);
    void cleanup();
    void compile();
    
    void bindAttributes();
    
    void unbindAttributes();
    
    void use();

    void setUniform(const std::string& id, float v);
    void setUniform(size_t id, const vec4& v4);
    void setUniform(size_t id, const std::vector<float>& fv);
    void setSampler(size_t id, size_t textureUnit);
    
    void setup();
};

#endif // GL_SPRITE_SHADER_H
