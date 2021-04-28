#ifndef JSON_MAIN_SIGNAL_H
#define JSON_MAIN_SIGNAL_H

#include "json/json.hpp"
#include "gui/SignalEdit.hpp"
#include "gui/PostEffect.hpp"
#include "gui/InputEffect.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, WaveEdit& wave, FrequencyEdit& freq, AmplitudeEdit& ampl);
//--------------------------------------------------------------
void saveInto(JsonValue& root, RingModFxEdit& ringmod, TremoloFxEdit& tremolo);
//--------------------------------------------------------------
void saveInto(JsonValue& root, VibratoFxEdit& vibrato, ArpeggioFxEdit& arpeggio);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, WaveEdit& wave, FrequencyEdit& freq, AmplitudeEdit& ampl);
//--------------------------------------------------------------
void loadFrom(JsonValue& root, RingModFxEdit& ringmod, TremoloFxEdit& tremolo);
//--------------------------------------------------------------
void loadFrom(JsonValue& root, VibratoFxEdit& vibrato, ArpeggioFxEdit& arpeggio);

//--------------------------------------------------------------
void writeInfo(JsonValue& root);

#endif // JSON_MAIN_SIGNAL_H
