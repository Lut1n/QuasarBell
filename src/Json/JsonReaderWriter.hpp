#ifndef JSON_IO_H
#define JSON_IO_H

#include "Json.hpp"

//--------------------------------------------------------------
std::string removeSpace(const std::string& str);

//--------------------------------------------------------------
bool check(const std::string& str);

//--------------------------------------------------------------
JsonValue parse(const std::string& str);

//--------------------------------------------------------------
std::string serialize(const JsonValue& json);

#endif // JSON_IO_H
