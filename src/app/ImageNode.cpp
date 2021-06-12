#include "App/ImageNode.hpp"

//--------------------------------------------------------------
/*namespace qb::imgui
{
    static int s_sampler2d_size = 256;
}*/
//--------------------------------------------------------------
ImageNode::ImageNode(const std::string& title, size_t nodeTypeId)
    : UiNode(title, vec2(0.0,0.0), vec2(70,70))
    , _nodetypeId(nodeTypeId)
{
}
//--------------------------------------------------------------
ImageNode::~ImageNode()
{
    disconnectAllPins();
}
//--------------------------------------------------------------
void ImageNode::setOperation(ImageOperation* op)
{
    _operation = op;
    if (!_operation) return;

    for(size_t i=0; i<_operation->getInputCount(); ++i)
    {
        auto input = _operation->getInput(i);
        addPin((int)i, input->name, false, UiPin::TYPE_FLOAT2);
    }
    for(size_t i=0; i<_operation->getOutputCount(); ++i)
    {
        auto output = _operation->getOutput(i);
        addPin((int)i, output->name, true, UiPin::TYPE_FLOAT2);
    }
}
//--------------------------------------------------------------
size_t ImageNode::nodeTypeId() const
{
    return _nodetypeId;
}
//--------------------------------------------------------------
ImageOperation* ImageNode::getOperation()
{
    return _operation;
}
//--------------------------------------------------------------
void ImageNode::displayProperties()
{
    _operation->uiProperties();
}
//--------------------------------------------------------------
void ImageNode::initializePreview()
{
    //_operation->preview.initialize(_operation);
}
//--------------------------------------------------------------
void ImageNode::updatePreview()
{
    // if selected, force update for taking elapsed time in account
    if(UiNode::isSelected(this))
        _operation->preview.dirty(false);
    
    _operation->preview.compute(_operation);
}
//--------------------------------------------------------------
void ImageNode::drawPreview(const Rect& previewArea)
{
    auto& preview = _operation->preview;

    Rect src = Rect::fromPosAndSize(vec2(0.0f,0.0f), vec2(preview.resolution,preview.resolution));
    // Rect target = previewArea + parentPosition + position;
    RenderInterface::setColor(0xFFFFFFFF);
    
    if(preview.renderFrame)
        RenderInterface::copy(preview.renderFrame->glResource, src, previewArea);
}