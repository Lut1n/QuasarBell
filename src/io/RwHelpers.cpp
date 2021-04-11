#include "io/RwHelpers.hpp"

//--------------------------------------------------------------
std::string toString(WaveEdit::Wave e)
{
    static const char* items[] = {"Sin", "Saw", "Square", "Noise_1", "Noise_2", "Noise_3", "Noise_4"};
    return items[(unsigned)e];
}
//--------------------------------------------------------------
void stringTo(const std::string& str, WaveEdit::Wave& e)
{
    static const char* items[] = {"Sin", "Saw", "Square", "Noise_1", "Noise_2", "Noise_3", "Noise_4"};
    for(int i=0; i<7; ++i)
        if(str == items[i]) e = (WaveEdit::Wave)i;
}

//--------------------------------------------------------------
void toJson(JsonValue& json, const vec2& v)
{
    json.setPath(0).set(v.x);
    json.setPath(1).set(v.y);
}
//--------------------------------------------------------------
void jsonTo(JsonValue& json, vec2& v)
{
    v.x = json.setPath(0).getNumeric();
    v.y = json.setPath(1).getNumeric();
}