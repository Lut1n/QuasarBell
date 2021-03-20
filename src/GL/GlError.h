#ifndef GLCOMMON_H_INCLUDED
#define GLCOMMON_H_INCLUDED

#include <iostream>
#include <vector>

#include <glad/glad.h>

#define GL_CHECKERROR(msg) GLcheckError(msg, __FILE__, __LINE__)

#define ENUM_TO_STR(enumVar, testVal, str) if(enumVar == testVal) str = #testVal ;

inline void glErrToString(GLenum error, std::string& string)
{
    string = "opengl unknown error.";

    ENUM_TO_STR(error, GL_NO_ERROR, string)
    ENUM_TO_STR(error, GL_INVALID_ENUM, string)
    ENUM_TO_STR(error, GL_INVALID_VALUE, string)
    ENUM_TO_STR(error, GL_INVALID_OPERATION, string)
    ENUM_TO_STR(error, GL_INVALID_FRAMEBUFFER_OPERATION, string)
    ENUM_TO_STR(error, GL_OUT_OF_MEMORY, string)
    //ENUM_TO_STR(error, GL_STACK_UNDERFLOW, string)
    //ENUM_TO_STR(error, GL_STACK_OVERFLOW, string)
}


inline bool GLcheckError(const std::string& debugMsg, const char* file, int line)
{
    bool res = true;
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        res = false;
        std::string strErr;
        glErrToString(error, strErr);
        std::cout << "[impError] " << debugMsg << " - GL error in " << file << " at line " << line << std::endl;
        std::cout << strErr << std::endl;
    }
    return res;
}


inline void testCompilation(unsigned shader, const std::string& msg)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        std::cout << "compilation error (" << msg << ")" << std::endl;
        GLint maxLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
        std::vector<char> errorLog(maxLen);
        glGetShaderInfoLog(shader, maxLen, &maxLen, &errorLog[0]);
        
        std::cout << std::string(&errorLog[0]) << std::endl;
    }
    else
    {
        std::cout << "compilation ok" << std::endl;
    }
}


#endif // GLCOMMON_H_INCLUDED
