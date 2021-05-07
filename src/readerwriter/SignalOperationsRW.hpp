#ifndef JSON_SIGNAL_OPERATIONS_H
#define JSON_SIGNAL_OPERATIONS_H

#include "json/json.hpp"
// #include "signal/operations/SignalOperation.hpp"

struct OperationCollection;
struct OperationConnections;

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& operations, const OperationConnections& co);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, OperationCollection& operations, OperationConnections& co);

#endif // JSON_SIGNAL_OPERATIONS_H