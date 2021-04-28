#ifndef GUI_SIGNAL_OP_NODE_H
#define GUI_SIGNAL_OP_NODE_H

#include <memory>
#include <unordered_map>

#include "ui/UiNode.h"
#include "signal/operations/SignalOperation.hpp"

struct SignalOperationNode : public UiNode
{
    SignalOperationNode(const std::string& title, size_t nodetypeId);

    void setOperation(SignalOperation* op);
    SignalOperation* getOperation();

    void drawPreview(const Rect& previewArea) override;

    size_t nodeTypeId() const;

protected:
    void dirtyPreview();
    void displayPreview();
    static void s_imgui_sampler_set_count(int count);
    static float s_imgui_sampler(void* data, int idx);
private:
    SignalOperation* _operation = nullptr;
    size_t _nodetypeId;
    std::array<float, 32> _preview;
    float _previewMaxVal = 1.0;
    bool _hasChange = true;
};

#endif // GUI_SIGNAL_OP_NODE_H
