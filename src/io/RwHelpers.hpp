#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <iostream>

#include "gui/SignalEdit.hpp"

//--------------------------------------------------------------
std::string toString(WaveEdit::Wave e);

//--------------------------------------------------------------
void stringTo(const std::string& str, WaveEdit::Wave& e);

#endif // JSON_HELPERS_H
