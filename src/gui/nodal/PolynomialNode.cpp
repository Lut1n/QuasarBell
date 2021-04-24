#include "gui/nodal/PolynomialNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, PolynomialNode> polynomial_node_factory(qb::OperationType_Polynomial);

//--------------------------------------------------------------
PolynomialNode::PolynomialNode()
    : SignalOperationNode("Polynomial", qb::OperationType_Polynomial)
{
    addPin(0, "value", true);
    setOperation(&sampler);
}
//--------------------------------------------------------------
void PolynomialNode::displayProperties()
{
    if (ImGui::InputInt("count", &sampler.count))
    {
        if (sampler.count < 1) sampler.count = 1;
        if (sampler.count > 10) sampler.count = 10;
        sampler.coefs.resize(sampler.count);
    }

    ImGui::InputFloat("reset", &sampler.reset);
    
    ImGui::Text("Coefs");
    ImGui::Separator();
    int index = 0;
    for(auto& coef : sampler.coefs)
    {
        std::string textid = std::string("##") + std::to_string(index);
        ImGui::InputFloat(textid.c_str(), &coef);
        index++;
    }
    displayPreview();
}