#ifndef QUASAR_BELL_ERROR_H
#define QUASAR_BELL_ERROR_H

#include <iostream>

//--------------------------------------------------------------
#define ENUM_TO_STR(enumVar, testVal, str) if(enumVar == testVal) str = #testVal ;

//--------------------------------------------------------------
inline void ALerrToString(ALenum error, std::string& string)
{
    string = "openal unknown error.";

    ENUM_TO_STR(error, AL_NO_ERROR, string)
    ENUM_TO_STR(error, AL_INVALID_NAME, string)
    ENUM_TO_STR(error, AL_INVALID_ENUM, string)
    ENUM_TO_STR(error, AL_INVALID_VALUE, string)
    ENUM_TO_STR(error, AL_INVALID_OPERATION, string)
    ENUM_TO_STR(error, AL_OUT_OF_MEMORY, string)
}
//--------------------------------------------------------------
void displayALError(const std::string& text, ALenum error)
{
    std::string errorName, errorDesc;
    ALerrToString(error, errorName);
    switch(error)
    {
        case AL_NO_ERROR:
        errorDesc = "there is not currently an error";
        break;
        case AL_INVALID_NAME:
        errorDesc = "a bad name (ID) was passed to an OpenAL function";
        break;
        case AL_INVALID_ENUM:
        errorDesc = "an invalid enum value was passed to an OpenAL function";
        break;
        case AL_INVALID_VALUE:
        errorDesc = "an invalid value was passed to an OpenAL function";
        break;
        case AL_INVALID_OPERATION:
        errorDesc = "the requested operation is not valid";
        break;
        case AL_OUT_OF_MEMORY:
        errorDesc = "the requested operation resulted in OpenAL running out of memory";
        break;
    }
    std::cerr << "[quasarbell-err]: " << text << "[" << (unsigned)error << "] " << errorName << " - " << errorDesc <<  std::endl;
}
//--------------------------------------------------------------
void ackErr(const std::string& msg)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        displayALError(msg, error);
        exit(-1);
    }
}

#endif // QUASAR_BELL_ERROR_H