#include "readerwriter/RwHelpers.hpp"

//--------------------------------------------------------------
void toJson(JsonValue& json, const vec2& v)
{
    json.setPath(0).set(v.x);
    json.setPath(1).set(v.y);
}
//--------------------------------------------------------------
void jsonTo(JsonValue& json, vec2& v)
{
    v.x = (float) json.setPath(0).getNumeric();
    v.y = (float) json.setPath(1).getNumeric();
}