#ifndef UI_NODE_H
#define UI_NODE_H

#include <memory>
#include <unordered_map>
#include <list>

#include "UI/UiSystem.h"
#include "UI/UiFrame.h"
#include "UI/UiList.h"
#include "UI/UiText.h"
#include "UI/UiPin.h"

struct UiNode : public UiFrame
{
    UiNode(const std::string& title, const vec2& position, const vec2& size);
    virtual ~UiNode();
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    void addPin(int id, const std::string& label, bool isOutput);
    UiNode* getSourceNode(int id);

    virtual void displayProperties();

    size_t getIndex(UiPin* pin) const;

protected:
    void onMove(const vec2& delta) override;
    
public:

    static bool isSelected(UiNode* node);

    std::unique_ptr<UiText> title;

    std::vector<std::unique_ptr<UiPin>> inputs;
    std::vector<std::unique_ptr<UiPin>> outputs;

    bool clicking = false;
    bool toDelete = false;
    static std::list<UiNode*> selected;
};

#endif // UI_NODE_H
