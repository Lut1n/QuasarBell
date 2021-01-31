#include "io/SignalRW.hpp"

#include <chrono>
#include <iomanip> // put_time
#include <ctime> // localtime
#include <sstream>

#include "io/RwHelpers.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, WaveEdit& wave, FrequencyEdit& freq, AmplitudeEdit& ampl)
{
    root.setPath("sfx-edition","signal","wave","type").set(toString(wave.wave));
    
    auto& jsonFreq = root.setPath("sfx-edition","signal","frequency");
    jsonFreq.setPath("pitch").set(freq.pitch);
    jsonFreq.setPath("speed").set(freq.speed);
    jsonFreq.setPath("acc").set(freq.acc);
    jsonFreq.setPath("jerk").set(freq.jerk);
    jsonFreq.setPath("reset").set(freq.resetTime);
    
    auto& jsonAmpl = root.setPath("sfx-edition","signal","amplitude");
    jsonAmpl.setPath("attack").set(ampl.attack);
    jsonAmpl.setPath("decay").set(ampl.decay);
    jsonAmpl.setPath("sustain").set(ampl.sustain);
    jsonAmpl.setPath("release").set(ampl.release);
    jsonAmpl.setPath("duration").set(ampl.duration);
}
//--------------------------------------------------------------
void loadFrom(JsonValue& root, WaveEdit& wave, FrequencyEdit& freq, AmplitudeEdit& ampl)
{
    stringTo(root.setPath("sfx-edition","signal","wave","type").getString(), wave.wave);
    
    auto& jsonFreq = root.setPath("sfx-edition","signal","frequency");
    freq.pitch = jsonFreq.setPath("pitch").getNumeric();
    freq.speed = jsonFreq.setPath("speed").getNumeric();
    freq.acc = jsonFreq.setPath("acc").getNumeric();
    freq.jerk = jsonFreq.setPath("jerk").getNumeric();
    freq.resetTime = jsonFreq.setPath("reset").getNumeric();

    auto& jsonAmpl = root.setPath("sfx-edition","signal","amplitude");
    ampl.attack = jsonAmpl.setPath("attack").getNumeric();
    ampl.decay = jsonAmpl.setPath("decay").getNumeric();
    ampl.sustain = jsonAmpl.setPath("sustain").getNumeric();
    ampl.release = jsonAmpl.setPath("release").getNumeric();
    ampl.duration = jsonAmpl.setPath("duration").getNumeric();
}
//--------------------------------------------------------------
void writeInfo(JsonValue& root)
{
    auto now = std::chrono::system_clock::now();
    auto t_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream date; date << std::put_time(std::localtime(&t_c), "%Y-%m-%d %X");

    std::string app = "QuasarBell (https://www.github.com/Lut1n/QuasarBell)";

    root.setPath("info","app").set(app);
    root.setPath("info","version").set(1.0);
    root.setPath("info","date").set(date.str());
}
