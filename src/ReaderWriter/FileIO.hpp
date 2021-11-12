#ifndef QUASARBELL_FILE_IO_H
#define QUASARBELL_FILE_IO_H

#include "Json/Json.hpp"

//--------------------------------------------------------------
std::string loadTextFile(const std::string& filename);
//--------------------------------------------------------------
void saveTextFile(const std::string& filename, const std::string& text);
//--------------------------------------------------------------
JsonValue loadJsonFile(const std::string& filename);
//--------------------------------------------------------------
void saveJsonFile(const std::string& filename, JsonValue& json);

#endif // QUASARBELL_FILE_IO_H
