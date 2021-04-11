#ifndef GUI_SIGNAL_OP_NODE_H
#define GUI_SIGNAL_OP_NODE_H

#include <memory>
#include <unordered_map>

#include "UI/UiNode.h"
#include "signal/operations/SignalOperation.hpp"

struct SignalOperationNode : public UiNode
{
    SignalOperationNode(const std::string& title, const vec2& position);

    void setOperation(SignalOperation* op);
    SignalOperation* getOperation();

protected:
    void displayPreview();
    static float s_imgui_sampler(void* data, int idx);
private:
    SignalOperation* operation = nullptr;
};

#endif // GUI_SIGNAL_OP_NODE_H
