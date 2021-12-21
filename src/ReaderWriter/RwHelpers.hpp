#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <iostream>

#include "Json/Json.hpp"
#include "Core/Vec2.h"

//--------------------------------------------------------------
void toJson(JsonValue& json, const vec2& v);
//--------------------------------------------------------------
void toJson(JsonValue& json, const float* fv, size_t sz);
//--------------------------------------------------------------
void jsonTo(JsonValue& json, vec2& v);
//--------------------------------------------------------------
void jsonTo(JsonValue& json, float* fv, size_t sz);
//--------------------------------------------------------------
void writeInfo(JsonValue& root);

#endif // JSON_HELPERS_H
