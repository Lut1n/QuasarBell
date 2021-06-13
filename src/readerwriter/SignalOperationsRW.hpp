#ifndef JSON_SIGNAL_OPERATIONS_H
#define JSON_SIGNAL_OPERATIONS_H

#include "Json/json.hpp"

struct OperationCollection;
struct OperationConnections;

struct ImageOperationCollection;
struct ImageOperationConnections;

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& operations, const OperationConnections& co);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, OperationCollection& operations, OperationConnections& co);

//--------------------------------------------------------------
void saveInto(JsonValue& root, ImageOperationCollection& operations, const ImageOperationConnections& co);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, ImageOperationCollection& operations, ImageOperationConnections& co);

#endif // JSON_SIGNAL_OPERATIONS_H
