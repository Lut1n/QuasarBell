#ifndef GUI_SIGNAL_OP_NODE_H
#define GUI_SIGNAL_OP_NODE_H

#include <memory>
#include <unordered_map>

#include "UI/UiNode.h"
#include "signal/operations/SignalOperation.hpp"

struct SignalOperationNode : public UiNode
{
    SignalOperationNode(const std::string& title, size_t nodetypeId);

    void setOperation(SignalOperation* op);
    SignalOperation* getOperation();

    size_t nodeTypeId() const;

protected:
    void displayPreview();
    static float s_imgui_sampler(void* data, int idx);
private:
    SignalOperation* _operation = nullptr;
    size_t _nodetypeId;
};

#endif // GUI_SIGNAL_OP_NODE_H
