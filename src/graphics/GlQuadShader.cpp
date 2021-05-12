#include "Graphics/GlQuadShader.h"

#include "Graphics/GlError.h"

static const char* vert_quad_shader_text =
"#version 330\n"
"in vec2 pos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(pos, 0.0, 1.0);\n"
"}\n";
 
static const char* frag_quad_shader_text =
"#version 330\n"
"uniform vec4 col;\n"
"void main()\n"
"{\n"
"    gl_FragColor = col;\n"
"}\n";


GlQuadShader::GlQuadShader()
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_quad_shader_text, NULL);
    glCompileShader(vertex_shader);
    testCompilation(vertex_shader, "quad vertex");

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_quad_shader_text, NULL);
    glCompileShader(fragment_shader);
    testCompilation(fragment_shader, "quad fragment");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vpos_location = glGetAttribLocation(program, "pos");
    vCol_location = glGetUniformLocation(program, "col");
    
    GL_CHECKERROR("quad shader link");
}

void GlQuadShader::use()
{
    glUseProgram(program);
    GL_CHECKERROR("quad shader use");
}

void GlQuadShader::bindAttributes()
{
    size_t attribSize = sizeof(float)*2;
    // std::cout << "vpos_location = " << vpos_location << std::endl;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) 0);
    GL_CHECKERROR("quad shader vertex attrib pointer");
    glEnableVertexAttribArray(vpos_location);
    GL_CHECKERROR("quad shader enable vertex attrib array");
}

void GlQuadShader::unbindAttributes()
{
    glDisableVertexAttribArray(vpos_location);
    GL_CHECKERROR("quad shader unbind attrib");
}

void GlQuadShader::setup(float* col)
{
    use();
    glUniform4fv(vCol_location, 1, col);
    GL_CHECKERROR("quad shader uniform");
}
