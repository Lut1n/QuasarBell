#include "io/PostRW.hpp"

#include "io/RwHelpers.hpp"

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
    ringmod.freqSampler.value = jsonRingmod.setPath("frequency","value").getNumeric();
    ringmod.freqSampler.speed = jsonRingmod.setPath("frequency","speed").getNumeric();
    ringmod.freqSampler.resetTime = jsonRingmod.setPath("frequency","reset").getNumeric();
    
    ringmod.amplSampler.value = jsonRingmod.setPath("amplitude","value").getNumeric();
    ringmod.amplSampler.speed = jsonRingmod.setPath("amplitude","speed").getNumeric();
    ringmod.amplSampler.resetTime = jsonRingmod.setPath("amplitude","reset").getNumeric();
    
    
    auto& jsonTremolo = root.setPath("sfx-edition","post-fx","tremolo");
    stringTo(jsonTremolo.setPath("wave","type").getString(), tremolo.wave.wave);
    
    tremolo.isEnabled = jsonTremolo.setPath("enable").getBoolean();
    tremolo.freqSampler.value = jsonTremolo.setPath("frequency","value").getNumeric();
    tremolo.freqSampler.speed = jsonTremolo.setPath("frequency","speed").getNumeric();
    tremolo.freqSampler.resetTime = jsonTremolo.setPath("frequency","reset").getNumeric();
    
    tremolo.amplSampler.value = jsonTremolo.setPath("amplitude","value").getNumeric();
    tremolo.amplSampler.speed = jsonTremolo.setPath("amplitude","speed").getNumeric();
    tremolo.amplSampler.resetTime = jsonTremolo.setPath("amplitude","reset").getNumeric();
}
