#include "readerwriter/SignalRW.hpp"

#include <chrono>
#include <iomanip> // put_time
#include <ctime> // localtime
#include <sstream>

#include "readerwriter/RwHelpers.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, VibratoFxEdit& vibrato, ArpeggioFxEdit& arpeggio)
{
    auto& jsonVibrato = root.setPath("sfx-edition","input-fx","vibrato");
    jsonVibrato.setPath("enable").set(vibrato.isEnabled);
    jsonVibrato.setPath("wave","type").set(toString(vibrato.wave.wave));
    jsonVibrato.setPath("frequency","value").set(vibrato.freqSampler.value);
    jsonVibrato.setPath("frequency","speed").set(vibrato.freqSampler.speed);
    jsonVibrato.setPath("frequency","reset").set(vibrato.freqSampler.resetTime);
    
    jsonVibrato.setPath("amplitude","value").set(vibrato.amplSampler.value);
    jsonVibrato.setPath("amplitude","speed").set(vibrato.amplSampler.speed);
    jsonVibrato.setPath("amplitude","reset").set(vibrato.amplSampler.resetTime);
    
    auto& jsonArpeggio = root.setPath("sfx-edition","input-fx","arpeggio");
    jsonArpeggio.setPath("enable").set(arpeggio.isEnabled);
    jsonArpeggio.setPath("rate","value").set(arpeggio.rateSampler.value);
    jsonArpeggio.setPath("rate","speed").set(arpeggio.rateSampler.speed);
    jsonArpeggio.setPath("rate","reset").set(arpeggio.rateSampler.resetTime);
    auto& jsonNotes = jsonArpeggio.setPath("notes");
    
    jsonNotes.clear();
    for(unsigned i = 0; i<arpeggio.notes.size(); ++i)
        jsonNotes.setPath(i).set((float)arpeggio.notes[i]);
}
//--------------------------------------------------------------
void loadFrom(JsonValue& root, VibratoFxEdit& vibrato, ArpeggioFxEdit& arpeggio)
{
    auto& jsonVibrato = root.setPath("sfx-edition","input-fx","vibrato");
    stringTo(jsonVibrato.setPath("wave","type").getString(), vibrato.wave.wave);
    
    vibrato.isEnabled = jsonVibrato.setPath("enable").getBoolean();
    vibrato.freqSampler.value = (float)jsonVibrato.setPath("frequency","value").getNumeric();
    vibrato.freqSampler.speed = (float) jsonVibrato.setPath("frequency","speed").getNumeric();
    vibrato.freqSampler.resetTime = (float) jsonVibrato.setPath("frequency","reset").getNumeric();
    
    vibrato.amplSampler.value = (float) jsonVibrato.setPath("amplitude","value").getNumeric();
    vibrato.amplSampler.speed = (float) jsonVibrato.setPath("amplitude","speed").getNumeric();
    vibrato.amplSampler.resetTime = (float) jsonVibrato.setPath("amplitude","reset").getNumeric();
    
    
    auto& jsonArpeggio = root.setPath("sfx-edition","input-fx","arpeggio");
    arpeggio.isEnabled = jsonArpeggio.setPath("enable").getBoolean();
    arpeggio.rateSampler.value = (float) jsonArpeggio.setPath("rate","value").getNumeric();
    arpeggio.rateSampler.speed = (float) jsonArpeggio.setPath("rate","speed").getNumeric();
    arpeggio.rateSampler.resetTime = (float) jsonArpeggio.setPath("rate","reset").getNumeric();
    
    auto& jsonNotes = jsonArpeggio.setPath("notes");
    size_t count = jsonNotes.count();
    arpeggio.notes.resize(count);
    for(size_t i = 0; i<count; ++i)
        arpeggio.notes[i] = (int)jsonNotes[(int)i].getNumeric();
}
//--------------------------------------------------------------
void saveInto(JsonValue& root, RingModFxEdit& ringmod, TremoloFxEdit& tremolo)
{
    auto& jsonRingmod = root.setPath("sfx-edition","post-fx","ringmod");
    jsonRingmod.setPath("enable").set(ringmod.isEnabled);
    jsonRingmod.setPath("wave","type").set(toString(ringmod.wave.wave));
    jsonRingmod.setPath("frequency","value").set(ringmod.freqSampler.value);
    jsonRingmod.setPath("frequency","speed").set(ringmod.freqSampler.speed);
    jsonRingmod.setPath("frequency","reset").set(ringmod.freqSampler.resetTime);
    
    jsonRingmod.setPath("amplitude","value").set(ringmod.amplSampler.value);
    jsonRingmod.setPath("amplitude","speed").set(ringmod.amplSampler.speed);
    jsonRingmod.setPath("amplitude","reset").set(ringmod.amplSampler.resetTime);
    
    auto& jsonTremolo = root.setPath("sfx-edition","post-fx","tremolo");
    jsonTremolo.setPath("enable").set(tremolo.isEnabled);
    jsonTremolo.setPath("wave","type").set(toString(tremolo.wave.wave));
    jsonTremolo.setPath("frequency","value").set(tremolo.freqSampler.value);
    jsonTremolo.setPath("frequency","speed").set(tremolo.freqSampler.speed);
    jsonTremolo.setPath("frequency","reset").set(tremolo.freqSampler.resetTime);
    
    jsonTremolo.setPath("amplitude","value").set(tremolo.amplSampler.value);
    jsonTremolo.setPath("amplitude","speed").set(tremolo.amplSampler.speed);
    jsonTremolo.setPath("amplitude","reset").set(tremolo.amplSampler.resetTime);
}
//--------------------------------------------------------------
void loadFrom(JsonValue& root, RingModFxEdit& ringmod, TremoloFxEdit& tremolo)
{
    auto& jsonRingmod = root.setPath("sfx-edition","post-fx","ringmod");
    stringTo(jsonRingmod.setPath("wave","type").getString(), ringmod.wave.wave);
    
    ringmod.isEnabled = jsonRingmod.setPath("enable").getBoolean();
    ringmod.freqSampler.value = (float) jsonRingmod.setPath("frequency","value").getNumeric();
    ringmod.freqSampler.speed = (float) jsonRingmod.setPath("frequency","speed").getNumeric();
    ringmod.freqSampler.resetTime = (float) jsonRingmod.setPath("frequency","reset").getNumeric();
    
    ringmod.amplSampler.value = (float) jsonRingmod.setPath("amplitude","value").getNumeric();
    ringmod.amplSampler.speed = (float) jsonRingmod.setPath("amplitude","speed").getNumeric();
    ringmod.amplSampler.resetTime = (float) jsonRingmod.setPath("amplitude","reset").getNumeric();
    
    
    auto& jsonTremolo = root.setPath("sfx-edition","post-fx","tremolo");
    stringTo(jsonTremolo.setPath("wave","type").getString(), tremolo.wave.wave);
    
    tremolo.isEnabled = jsonTremolo.setPath("enable").getBoolean();
    tremolo.freqSampler.value = (float) jsonTremolo.setPath("frequency","value").getNumeric();
    tremolo.freqSampler.speed = (float) jsonTremolo.setPath("frequency","speed").getNumeric();
    tremolo.freqSampler.resetTime = (float) jsonTremolo.setPath("frequency","reset").getNumeric();
    
    tremolo.amplSampler.value = (float) jsonTremolo.setPath("amplitude","value").getNumeric();
    tremolo.amplSampler.speed = (float) jsonTremolo.setPath("amplitude","speed").getNumeric();
    tremolo.amplSampler.resetTime = (float) jsonTremolo.setPath("amplitude","reset").getNumeric();
}
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
    freq.pitch = (float) jsonFreq.setPath("pitch").getNumeric();
    freq.speed = (float) jsonFreq.setPath("speed").getNumeric();
    freq.acc = (float) jsonFreq.setPath("acc").getNumeric();
    freq.jerk = (float) jsonFreq.setPath("jerk").getNumeric();
    freq.resetTime = (float) jsonFreq.setPath("reset").getNumeric();

    auto& jsonAmpl = root.setPath("sfx-edition","signal","amplitude");
    ampl.attack = (float) jsonAmpl.setPath("attack").getNumeric();
    ampl.decay = (float) jsonAmpl.setPath("decay").getNumeric();
    ampl.sustain = (float) jsonAmpl.setPath("sustain").getNumeric();
    ampl.release = (float) jsonAmpl.setPath("release").getNumeric();
    ampl.duration = (float) jsonAmpl.setPath("duration").getNumeric();
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
    root.setPath("info","version").set(2.0);
    root.setPath("info","date").set(date.str());
}
