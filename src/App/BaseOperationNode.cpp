#include "App/BaseOperationNode.hpp"

#include "imgui.h"


//--------------------------------------------------------------
BaseOperationNode::BaseOperationNode(const std::string& title, size_t nodeTypeId, BaseAttributes* properties)
    : UiNode(title, vec2(0.0,0.0), vec2(70,70))
    , _nodetypeId(nodeTypeId)
{
    unsigned basecolor = 0x001b1bFF;
    color = basecolor;
    colorOnOver = 0x505050FF;
    colorOnIdle = basecolor;
    rounded = true;
    
    _attributes.reset(properties);
    
    int index = 0;
    for(auto& def : _attributes->inputs)
        addPin(index++, def.first, false, (size_t)def.second);
    
    index = 0;
    for(auto& def : _attributes->outputs)
        addPin(index++, def.first, true, (size_t)def.second);
}
//--------------------------------------------------------------
size_t BaseOperationNode::nodeTypeId() const
{
    return _nodetypeId;
}
void BaseOperationNode::drawPreview(const Rect& previewArea)
{
    if (_preview)
        _preview->nodePreview(previewArea);
}
