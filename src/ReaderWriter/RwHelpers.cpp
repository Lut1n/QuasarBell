#include "ReaderWriter/RwHelpers.hpp"

#include "Core/Version.hpp"

#include <chrono>
#include <iomanip> // put_time
#include <ctime> // localtime
#include <sstream>

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
//--------------------------------------------------------------
void writeInfo(JsonValue& root)
{
    time_t t = time(NULL);
    std::stringstream date; date << std::put_time(localtime(&t), "%Y-%m-%d %X");

    std::string app = "QuasarBell (https://www.github.com/Lut1n/QuasarBell)";

    root.setPath("info","app").set(app);
    root.setPath("info","version").set(QB_VERSION);
    root.setPath("info","date").set(date.str());
}