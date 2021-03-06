#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <iostream>

#include "Json/json.hpp"
#include "Core/Vec2.h"

//--------------------------------------------------------------
void toJson(JsonValue& json, const vec2& v);
//--------------------------------------------------------------
void jsonTo(JsonValue& json, vec2& v);
//--------------------------------------------------------------
void writeInfo(JsonValue& root);

#endif // JSON_HELPERS_H
