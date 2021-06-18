#ifndef JSON_SIGNAL_OPERATIONS_H
#define JSON_SIGNAL_OPERATIONS_H

#include "Json/json.hpp"
#include "Ui/UiPin.h"
#include "App/BaseOperationNode.hpp"


struct OperationCollection;
struct OperationConnections;

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& collection, const OperationConnections& co, const std::string& category, UiPin::Type pinType,
            std::function<std::string(BaseOperationNode*)> typeIdGetter);

//--------------------------------------------------------------
void loadFrom(JsonValue& root, OperationCollection& collection, OperationConnections& co, const std::string& category, UiPin::Type pinType, 
            std::function<BaseOperationNode*(const std::string&)> nodeCreator);

#endif // JSON_SIGNAL_OPERATIONS_H
