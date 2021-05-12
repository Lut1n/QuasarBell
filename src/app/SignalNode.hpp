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
struct SignalNode : public UiNode
{
    SignalNode(const std::string& title, size_t nodetypeId);

    void setOperation(SignalOperation* op);
    SignalOperation* getOperation();

    void displayProperties() override;
    void drawPreview(const Rect& previewArea) override;

    size_t nodeTypeId() const;
private:
    SignalOperation* _operation = nullptr;
    size_t _nodetypeId;
};
//--------------------------------------------------------------
template<typename OpObject, qb::OperationType OpType>
struct TypedSignalNode : public SignalNode
{
    TypedSignalNode()
        : SignalNode("SignalNode", OpType)
    {
        title->text = qb::getOperationName(OpType);
        setOperation(&signalOperation);
    }
private:
    OpObject signalOperation;
};
//--------------------------------------------------------------
#define MAKE_SIGNAL_NODE(opclass, optype)\
    struct opclass ## Node : public TypedSignalNode<opclass, optype> {};
//--------------------------------------------------------------
#define MAKE_SIGNAL_NODE_CREATOR(opclass, optype)\
    static TypedCreator<SignalNode, opclass ## Node> opclass ## Creator(optype);


#endif // GUI_SIGNAL_OP_NODE_H
