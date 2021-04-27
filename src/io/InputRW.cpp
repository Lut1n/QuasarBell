#include "io/InputRW.hpp"

#include "io/RwHelpers.hpp"

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
    vibrato.freqSampler.value = jsonVibrato.setPath("frequency","value").getNumeric();
    vibrato.freqSampler.speed = jsonVibrato.setPath("frequency","speed").getNumeric();
    vibrato.freqSampler.resetTime = jsonVibrato.setPath("frequency","reset").getNumeric();
    
    vibrato.amplSampler.value = jsonVibrato.setPath("amplitude","value").getNumeric();
    vibrato.amplSampler.speed = jsonVibrato.setPath("amplitude","speed").getNumeric();
    vibrato.amplSampler.resetTime = jsonVibrato.setPath("amplitude","reset").getNumeric();
    
    
    auto& jsonArpeggio = root.setPath("sfx-edition","input-fx","arpeggio");
    arpeggio.isEnabled = jsonArpeggio.setPath("enable").getBoolean();
    arpeggio.rateSampler.value = jsonArpeggio.setPath("rate","value").getNumeric();
    arpeggio.rateSampler.speed = jsonArpeggio.setPath("rate","speed").getNumeric();
    arpeggio.rateSampler.resetTime = jsonArpeggio.setPath("rate","reset").getNumeric();
    
    auto& jsonNotes = jsonArpeggio.setPath("notes");
    unsigned count = jsonNotes.count();
    arpeggio.notes.resize(count);
    for(unsigned i = 0; i<count; ++i)
        arpeggio.notes[i] = jsonNotes[i].getNumeric();
}
