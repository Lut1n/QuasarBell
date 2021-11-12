#ifndef GUI_SIGNAL_OP_NODE_H
#define GUI_SIGNAL_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"
#include "SignalOperation/SignalOperation.hpp"
#include "SignalOperation/OperationType.hpp"
#include "Core/Factory.h"

//--------------------------------------------------------------
struct SignalNode : public BaseOperationNode
{
    SignalNode(qb::OperationType opType, BaseOperation* operation)
        : BaseOperationNode(qb::getOperationName(opType), (size_t)opType, operation)
        {}
    
    void drawPreview(const Rect& previewArea) override;
};
//--------------------------------------------------------------
template<typename OpObject, qb::OperationType OpType>
struct TypedSignalNode : public SignalNode
{
    TypedSignalNode() : SignalNode(OpType, new OpObject()) {}
};
//--------------------------------------------------------------
#define MAKE_SIGNAL_NODE(opclass, optype)\
    struct opclass ## Node : public TypedSignalNode<opclass, optype> {};
//--------------------------------------------------------------
#define MAKE_SIGNAL_NODE_CREATOR(opclass, optype)\
    static TypedCreator<SignalNode, opclass ## Node> opclass ## Creator(optype);


#endif // GUI_SIGNAL_OP_NODE_H
