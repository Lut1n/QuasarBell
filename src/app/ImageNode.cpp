#include "App/ImageNode.hpp"

//--------------------------------------------------------------
void ImageNode::updatePreview()
{
    // if selected, force update for taking elapsed time in account
    if(UiNode::isSelected(this))
        dynamic_cast<ImageOperation*>(getOperation())->preview.dirty(false);
    
    dynamic_cast<ImageOperation*>(getOperation())->preview.compute(dynamic_cast<ImageOperation*>(getOperation()));
}
//--------------------------------------------------------------
void ImageNode::drawPreview(const Rect& previewArea)
{
    auto& preview = dynamic_cast<ImageOperation*>(getOperation())->preview;

    Rect src = Rect::fromPosAndSize(vec2(0.0f,0.0f), vec2((float)preview.resolution,(float)preview.resolution));
    // Rect target = previewArea + parentPosition + position;
    RenderInterface::setColor(0xFFFFFFFF);
    
    if(preview.renderFrame)
        RenderInterface::copy((unsigned)preview.renderFrame->glResource, src, previewArea, false, true);
}