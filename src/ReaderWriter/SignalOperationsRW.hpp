#ifndef JSON_SIGNAL_OPERATIONS_H
#define JSON_SIGNAL_OPERATIONS_H

#include "Json/Json.hpp"
#include "Ui/UiPin.h"
#include "App/BaseOperationNode.hpp"


struct OperationCollection;
struct OperationConnections;

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& collection, const OperationConnections& co, std::function<std::string(BaseOperationNode*)> typeIdGetter);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, OperationCollection& collection, OperationConnections& co, std::function<BaseOperationNode*(const std::string&)> nodeCreator);

#endif // JSON_SIGNAL_OPERATIONS_H
