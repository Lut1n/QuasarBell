#ifndef JSON_POSTFX_H
#define JSON_POST_H

#include "io/json/json.hpp"
#include "gui/PostEffect.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, RingModFxEdit& ringmod, TremoloFxEdit& tremolo);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, RingModFxEdit& ringmod, TremoloFxEdit& tremolo);

#endif // JSON_POSTFX_H
