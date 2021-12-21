#ifndef GUI_SIGNAL_OP_NODE_H
#define GUI_SIGNAL_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "App/BaseOperationNode.hpp"
#include "SignalOperation/OperationType.hpp"
#include "Core/Factory.h"

//--------------------------------------------------------------
struct SignalNode : public BaseOperationNode
{
    SignalNode(size_t opType, BaseAttributes* attributes)
        : BaseOperationNode(qb::getOperationName((qb::OperationType)opType), opType, attributes)
        {
        }
};
//--------------------------------------------------------------
template<typename OpObject>
struct TypedSignalNode : public SignalNode
{
    TypedSignalNode() : SignalNode(OpObject::TypeId, new OpObject()) {}
};
//--------------------------------------------------------------
#define MAKE_SIGNAL_NODE(opclass)\
    struct opclass ## Node : public TypedSignalNode<opclass> {};
//--------------------------------------------------------------
#define MAKE_SIGNAL_NODE_CREATOR(opclass)\
    static TypedCreator<SignalNode, opclass ## Node> opclass ## Creator(opclass::TypeId);


#endif // GUI_SIGNAL_OP_NODE_H
