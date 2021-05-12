#ifndef GUI_SIGNAL_OP_NODE_H
#define GUI_SIGNAL_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"
#include "SignalOperation/SignalOperation.hpp"
#include "SignalOperation/OperationType.hpp"
#include "Core/Factory.h"


struct SignalOperationNode : public UiNode
{
    SignalOperationNode(const std::string& title, size_t nodetypeId);

    void setOperation(SignalOperation* op);
    SignalOperation* getOperation();

    void drawPreview(const Rect& previewArea) override;

    size_t nodeTypeId() const;
private:
    SignalOperation* _operation = nullptr;
    size_t _nodetypeId;
};

template<typename OpObject, qb::OperationType OpType>
struct ConcretSignalNode : public SignalOperationNode
{
    ConcretSignalNode()
        : SignalOperationNode("SignalNode", OpType)
    {
        title->text = qb::getOperationName(OpType);
        setOperation(&concretOperation);
    }

    void displayProperties() override
    {
        getOperation()->uiProperties();
    }
    
public:
    OpObject concretOperation;
};


#define MAKE_SIGNAL_NODE(opclass, optype)\
    struct opclass ## Node : public ConcretSignalNode<opclass, optype> {};

#define MAKE_SIGNAL_NODE_CREATOR(opclass, optype)\
    static TypedFactory<SignalOperationNode, opclass ## Node> opclass ## Factory(optype);


#endif // GUI_SIGNAL_OP_NODE_H
