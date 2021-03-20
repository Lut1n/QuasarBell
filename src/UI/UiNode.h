#ifndef UI_NODE_H
#define UI_NODE_H

#include <memory>
#include <unordered_map>

#include "UI/UiSystem.h"
#include "UI/UiFrame.h"
#include "UI/UiList.h"
#include "UI/UiText.h"
#include "UI/UiPin.h"

struct UiNode : public UiFrame
{
    UiNode(const std::string& title, const vec2& position, const vec2& size);
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    void addPin(int id, const std::string& label, bool isOutput);
    UiNode* getSourceNode(int id);
    
public:
    std::unique_ptr<UiText> title;

    std::unordered_map<int, std::unique_ptr<UiPin>> inputs;
    std::unordered_map<int, std::unique_ptr<UiPin>> outputs;
};

#endif // UI_NODE_H
