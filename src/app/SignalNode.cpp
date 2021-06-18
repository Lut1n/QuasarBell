#include "App/SignalNode.hpp"

//--------------------------------------------------------------
void SignalNode::drawPreview(const Rect& previewArea)
{
    auto& preview = dynamic_cast<SignalOperation*>(getOperation())->preview;
    auto& data = preview.data;
    const int sample_count = (int)data.size();
    RenderInterface::setColor(0x777777FF);
    Rect localArea = previewArea + (parentPosition + position)*1.0;
    RenderInterface::fill(previewArea.p0, previewArea.p1);
    vec2 barS(localArea.size().x / (float)sample_count, 0);
    RenderInterface::setColor(0xEEEEEEFF);

    preview.compute(dynamic_cast<SignalOperation*>(getOperation()));

    for(int i=0; i< (int)data.size(); ++i)
    {
        vec2 p = previewArea.p0 + vec2((float)i*barS.x,localArea.size().y*0.5f);
        float y = data[i] / preview.maxVal;
        float h = localArea.size().y*0.5f * -y;
        RenderInterface::fill(p, p+vec2(barS.x,h));
    }
}