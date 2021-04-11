#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <iostream>

#include "io/json/json.hpp"
#include "Core/Vec2.h"
#include "gui/SignalEdit.hpp"

//--------------------------------------------------------------
std::string toString(WaveEdit::Wave e);
//--------------------------------------------------------------
void stringTo(const std::string& str, WaveEdit::Wave& e);

//--------------------------------------------------------------
void toJson(JsonValue& json, const vec2& v);
//--------------------------------------------------------------
void jsonTo(JsonValue& json, vec2& v);

#endif // JSON_HELPERS_H
