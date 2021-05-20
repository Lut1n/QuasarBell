#include "App/SignalNode.hpp"

//--------------------------------------------------------------
namespace qb::imgui
{
    static float s_sampler_count = 100;
}
//--------------------------------------------------------------
SignalNode::SignalNode(const std::string& title, size_t nodeTypeId)
    : UiNode(title, vec2(0.0,0.0), vec2(70,70))
    , _nodetypeId(nodeTypeId)
{
}
//--------------------------------------------------------------
SignalNode::~SignalNode()
{
    disconnectAllPins();
}
//--------------------------------------------------------------
void SignalNode::setOperation(SignalOperation* op)
{
    _operation = op;
    for(size_t i=0; i<_operation->getInputCount(); ++i)
    {
        auto input = _operation->getInput(i);
        addPin((int)i, input->name, false, UiPin::TYPE_FLOAT1);
    }
    for(size_t i=0; i<_operation->getOutputCount(); ++i)
    {
        auto output = _operation->getOutput(i);
        addPin((int)i, output->name, true, UiPin::TYPE_FLOAT1);
    }
}
//--------------------------------------------------------------
size_t SignalNode::nodeTypeId() const
{
    return _nodetypeId;
}
//--------------------------------------------------------------
SignalOperation* SignalNode::getOperation()
{
    return _operation;
}
//--------------------------------------------------------------
void SignalNode::displayProperties()
{
    _operation->uiProperties();
}
//--------------------------------------------------------------
void SignalNode::drawPreview(const Rect& previewArea)
{
    auto& preview = _operation->preview;
    auto& data = preview.data;
    const int sample_count = (int)data.size();
    RenderInterface::setColor(0x777777FF);
    Rect target = previewArea + parentPosition + position;
    RenderInterface::fill(target.p0, target.p1);
    vec2 barS(previewArea.size().x / (float)sample_count, 0);
    RenderInterface::setColor(0xEEEEEEFF);

    preview.compute(_operation);

    for(int i=0; i< (int)data.size(); ++i)
    {
        vec2 p = target.p0 + vec2((float)i*barS.x,previewArea.size().y*0.5f);
        float y = data[i] / preview.maxVal;
        float h = previewArea.size().y*0.5f * -y;
        RenderInterface::fill(p, p+vec2(barS.x,h));
    }
}