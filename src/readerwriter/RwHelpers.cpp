#include "ReaderWriter/RwHelpers.hpp"

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
    struct tm buf;
    auto now = std::chrono::system_clock::now();
    auto t_c = std::chrono::system_clock::to_time_t(now);
    ::localtime_s(&buf, &t_c);
    std::stringstream date; date << std::put_time(&buf, "%Y-%m-%d %X");

    std::string app = "QuasarBell (https://www.github.com/Lut1n/QuasarBell)";

    root.setPath("info","app").set(app);
    root.setPath("info","version").set(3.0);
    root.setPath("info","date").set(date.str());
}