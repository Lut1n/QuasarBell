#ifndef JSON_MAIN_SIGNAL_H
#define JSON_MAIN_SIGNAL_H

#include "io/json/json.hpp"
#include "gui/SignalEdit.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, WaveEdit& wave, FrequencyEdit& freq, AmplitudeEdit& ampl);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, WaveEdit& wave, FrequencyEdit& freq, AmplitudeEdit& ampl);

//--------------------------------------------------------------
void writeInfo(JsonValue& root);

#endif // JSON_MAIN_SIGNAL_H
