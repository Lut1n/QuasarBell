#include "graphics/GlSpriteShader.h"

#include "graphics/GlError.h"

static const char* vert_spriteShader_text =
"#version 330\n"
// "uniform mat4 MVP;\n"
// "uniform vec3 transform\n"
// "uniform vec3 vCol;\n"
"in vec2 pos;\n"
"in vec2 uv;\n"
//"varying vec3 color;\n"
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
// "FragColor = texture(screenTexture, TexCoords);\n"
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
    GL_CHECKERROR("quad shader uniform");
}
