#include "Graphics/CustomProgram.h"

#include "Graphics/GlError.h"

static const char* vert_custom_shader_text =
"#version 330\n"
"in vec2 pos;\n"
"in vec2 uv;\n"
"out vec2 vUV;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(pos, 0.0, 1.0);\n"
"    vUV = uv;\n"
"}\n";
 
static const char* default_frag_shader_text =
"#version 330\n"
"in vec2 vUV;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);\n"
"}\n";


GlCustomProgram::GlCustomProgram()
{
    fragShaderCode = default_frag_shader_text;
    compile();
}

GlCustomProgram::~GlCustomProgram()
{
    cleanup();
}

void GlCustomProgram::setFragCode(const std::string& fragCode)
{
    fragShaderCode = fragCode;
    cleanup();
    compile();
}

void GlCustomProgram::compile()
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_custom_shader_text, NULL);
    glCompileShader(vertex_shader);
    testCompilation(vertex_shader, "custom vertex");

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* glsl = fragShaderCode.c_str();
    glShaderSource(fragment_shader, 1, &glsl, NULL);
    glCompileShader(fragment_shader);
    testCompilation(fragment_shader, "custom fragment");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vpos_location = glGetAttribLocation(program, "pos");
    uv_location = glGetAttribLocation(program, "uv");
    
    GL_CHECKERROR("quad shader link");
}

void GlCustomProgram::cleanup()
{
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    program = 0;
    vertex_shader = 0;
    fragment_shader = 0;
}

void GlCustomProgram::use()
{
    glUseProgram(program);
    GL_CHECKERROR("custom program use");
}

void GlCustomProgram::bindAttributes()
{
    size_t v2Size = sizeof(float)*2;
    size_t attribSize = v2Size*2;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) 0);
    GL_CHECKERROR("custom program bind attrib (pos ptr)");
    glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) v2Size);
    GL_CHECKERROR("custom program bind attrib (uv ptr)");
    glEnableVertexAttribArray(vpos_location);
    GL_CHECKERROR("custom program bind attrib (vpos)");
    glEnableVertexAttribArray(uv_location);
    GL_CHECKERROR("custom program bind attrib (uv)");
}

void GlCustomProgram::unbindAttributes()
{
    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(uv_location);
    GL_CHECKERROR("custom program unbind attrib");
}

void GlCustomProgram::setup()
{
    use();
    GL_CHECKERROR("custom program uniform");
}
