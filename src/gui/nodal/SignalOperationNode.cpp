#include "gui/nodal/SignalOperationNode.hpp"

#include "imgui.h"

namespace qb
{
    namespace imgui
    {
        static float s_sampler_count = 100;
    }
}

//--------------------------------------------------------------
SignalOperationNode::SignalOperationNode(const std::string& title, size_t nodeTypeId)
    : UiNode(title, vec2(0.0,0.0), vec2(70,70))
    , _nodetypeId(nodeTypeId)
{
}

//--------------------------------------------------------------
void SignalOperationNode::setOperation(SignalOperation* op)
{
    _operation = op;
    for(size_t i=0; i<_operation->getInputCount(); ++i)
    {
        auto input = _operation->getInput(i);
        addPin(i, input->name, false);
    }
    for(size_t i=0; i<_operation->getOutputCount(); ++i)
    {
        auto output = _operation->getOutput(i);
        addPin(i, output->name, true);
    }
}

//--------------------------------------------------------------
size_t SignalOperationNode::nodeTypeId() const
{
    return _nodetypeId;
}

//--------------------------------------------------------------
SignalOperation* SignalOperationNode::getOperation()
{
    return _operation;
}

void SignalOperationNode::drawPreview(const Rect& previewArea)
{
    auto& preview = getOperation()->preview;
    auto& data = preview.data;
    const int sample_count = (int)data.size();
    RenderInterface::setColor(0x777777FF);
    Rect target = previewArea + parentPosition + position;
    RenderInterface::fill(target.p0, target.p1);
    vec2 barS(previewArea.size().x / (float)sample_count, 0);
    RenderInterface::setColor(0xEEEEEEFF);

    preview.compute(getOperation());

    for(int i=0; i< (int)data.size(); ++i)
    {
        vec2 p = target.p0 + vec2((float)i*barS.x,previewArea.size().y*0.5f);
        float y = data[i] / preview.maxVal;
        float h = previewArea.size().y*0.5f * -y;
        RenderInterface::fill(p, p+vec2(barS.x,h));
    }
}