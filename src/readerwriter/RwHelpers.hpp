#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <iostream>

#include "json/json.hpp"
#include "core/Vec2.h"

//--------------------------------------------------------------
void toJson(JsonValue& json, const vec2& v);
//--------------------------------------------------------------
void jsonTo(JsonValue& json, vec2& v);

#endif // JSON_HELPERS_H
