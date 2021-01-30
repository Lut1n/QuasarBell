#ifndef JSON_INPUTFX_H
#define JSON_INPUTFX_H

#include "json/json.hpp"
#include "gui/InputEffect.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, VibratoFxEdit& vibrato, ArpeggioFxEdit& arpeggio);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, VibratoFxEdit& vibrato, ArpeggioFxEdit& arpeggio);

#endif // JSON_INPUTFX_H
