#include "Graphics/GlQuadShader.h"

#include "Graphics/GlError.h"

static const char* vert_quad_shader_text =
"#version 330\n"
"in vec2 pos;\n"
"out vec2 fragpos;\n"
"void main()\n"
"{\n"
"    fragpos = pos;\n"
"    gl_Position = vec4(pos, 0.0, 1.0);\n"
"}\n";
 
static const char* frag_quad_shader_text =
"#version 330\n"
"uniform vec4 col;\n"
"void main()\n"
"{\n"
"    gl_FragColor = col;\n"
"}\n";

static const char* frag_line_shader_text =
"#version 330\n"
"uniform vec4 col;\n"
"uniform vec4 p1p2;\n"
"uniform vec4 thickness;\n"
"in vec2 fragpos;\n"
"void main()\n"
"{\n"
"    vec2 p1 = p1p2.xy;\n"
"    vec2 p2 = p1p2.zw;\n"
"    vec3 dir = vec3(normalize(p2-p1),0.0);\n"
"    float th = mix(thickness.x,thickness.y,abs(dir.x));\n"
"    vec2 n = cross(vec3(0.0,0.0,1.0), dir).xy;\n"
"    float d = abs(dot(n,fragpos-p1)) / (th*0.5);\n"
"    float a = 1.0 - clamp(d, 0.0, 1.0);\n"
"    gl_FragColor = col * vec4(col.xyz,a);\n"
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






GlLineShader::GlLineShader()
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_quad_shader_text, NULL);
    glCompileShader(vertex_shader);
    testCompilation(vertex_shader, "line vertex");

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_line_shader_text, NULL);
    glCompileShader(fragment_shader);
    testCompilation(fragment_shader, "line fragment");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vpos_location = glGetAttribLocation(program, "pos");
    vCol_location = glGetUniformLocation(program, "col");
    p1p2_location = glGetUniformLocation(program, "p1p2");
    thickness_location = glGetUniformLocation(program, "thickness");
    
    GL_CHECKERROR("line shader link");
}

void GlLineShader::use()
{
    glUseProgram(program);
    GL_CHECKERROR("line shader use");
}

void GlLineShader::bindAttributes()
{
    size_t attribSize = sizeof(float)*2;
    // std::cout << "vpos_location = " << vpos_location << std::endl;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, (GLsizei) attribSize, (void*) 0);
    GL_CHECKERROR("line shader vertex attrib pointer");
    glEnableVertexAttribArray(vpos_location);
    GL_CHECKERROR("line shader enable vertex attrib array");
}

void GlLineShader::unbindAttributes()
{
    glDisableVertexAttribArray(vpos_location);
    GL_CHECKERROR("line shader unbind attrib");
}

void GlLineShader::setup(float* col, float* p1p2, float* thickness)
{
    use();
    glUniform4fv(vCol_location, 1, col);
    glUniform4fv(p1p2_location, 1, p1p2);
    glUniform4fv(thickness_location, 1, thickness);
    GL_CHECKERROR("line shader uniform");
}
