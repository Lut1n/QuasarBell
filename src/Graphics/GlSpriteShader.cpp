#include "Graphics/GlSpriteShader.h"

#include "Graphics/GlError.h"

static const char* vert_spriteShader_text =
"#version 330\n"
"in vec2 pos;\n"
"in vec2 uv;\n"
"out vec2 vUV;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(pos, 0.0, 1.0);\n"
"    vUV = uv;\n"
"}\n";
 
static const char* frag_spriteShader_text =
"#version 330\n"
"uniform vec4 col;\n"
"uniform sampler2D tex;\n"
"in vec2 vUV;\n"
"void main()\n"
"{\n"
"    gl_FragColor = col * texture2D(tex, vUV);\n"
"}\n";

static const char* frag_roundedShader_text =
"#version 330\n"
"uniform vec4 col;\n"
"uniform float radius;\n"
"in vec2 vUV;\n"
"void main()\n"
"{\n"
"   vec2 c = min(max(vUV, vec2(radius)), vec2(1.0-radius));\n"
"   gl_FragColor = col * step(distance(vUV,c), radius);\n"
"}\n";


GlSpriteShader::GlSpriteShader()
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_spriteShader_text, NULL);
    glCompileShader(vertex_shader);
    testCompilation(vertex_shader, "sprite vertex");

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_spriteShader_text, NULL);
    glCompileShader(fragment_shader);
    testCompilation(fragment_shader, "sprite fragment");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vpos_location = glGetAttribLocation(program, "pos");
    uv_location = glGetAttribLocation(program, "uv");
    
    // mvp_location = glGetUniformLocation(program, "transform");
    vCol_location = glGetUniformLocation(program, "col");
    tex_location = glGetUniformLocation(program, "tex");
    GL_CHECKERROR("sprite shader link");
}

void GlSpriteShader::bindAttributes()
{
    size_t v2Size = sizeof(float)*2;
    size_t attribSize = v2Size*2;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) 0);
    GL_CHECKERROR("sprite shader bind attrib (pos ptr)");
    glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) v2Size);
    GL_CHECKERROR("sprite shader bind attrib (uv ptr)");
    glEnableVertexAttribArray(vpos_location);
    GL_CHECKERROR("sprite shader bind attrib (vpos)");
    glEnableVertexAttribArray(uv_location);
    GL_CHECKERROR("sprite shader bind attrib (uv)");
}

void GlSpriteShader::unbindAttributes()
{
    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(uv_location);
    GL_CHECKERROR("sprite shader unbind attrib");
}

void GlSpriteShader::use()
{
    glUseProgram(program);
    GL_CHECKERROR("sprite shader use");
}

void GlSpriteShader::setup(float* col, unsigned /*tex*/)
{
    use();
    glUniform4fv(vCol_location, 1, col);
    glUniform1i(tex_location, 0);
    GL_CHECKERROR("sprite shader uniform");
}


GlRoundedShader::GlRoundedShader()
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_spriteShader_text, NULL);
    glCompileShader(vertex_shader);
    testCompilation(vertex_shader, "rounded vertex");

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_roundedShader_text, NULL);
    glCompileShader(fragment_shader);
    testCompilation(fragment_shader, "rounded fragment");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vpos_location = glGetAttribLocation(program, "pos");
    uv_location = glGetAttribLocation(program, "uv");
    
    vCol_location = glGetUniformLocation(program, "col");
    radius_location =  glGetUniformLocation(program, "radius");
    
    GL_CHECKERROR("rounded shader link");
}

void GlRoundedShader::bindAttributes()
{
    size_t v2Size = sizeof(float)*2;
    size_t attribSize = v2Size*2;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) 0);
    GL_CHECKERROR("rounded shader bind attrib (pos ptr)");
    glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) v2Size);
    GL_CHECKERROR("rounded shader bind attrib (uv ptr)");
    glEnableVertexAttribArray(vpos_location);
    GL_CHECKERROR("rounded shader bind attrib (vpos)");
    glEnableVertexAttribArray(uv_location);
    GL_CHECKERROR("rounded shader bind attrib (uv)");
}

void GlRoundedShader::unbindAttributes()
{
    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(uv_location);
    GL_CHECKERROR("rounded shader unbind attrib");
}

void GlRoundedShader::use()
{
    glUseProgram(program);
    GL_CHECKERROR("rounded shader use");
}

void GlRoundedShader::setup(float* col, float radius)
{
    use();
    glUniform4fv(vCol_location, 1, col);
    glUniform1f(radius_location, radius);
    GL_CHECKERROR("rounded shader uniform");
}
