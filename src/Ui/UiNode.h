#ifndef UI_NODE_H
#define UI_NODE_H

#include <memory>
#include <unordered_map>
#include <list>

#include "Ui/UiSystem.h"
#include "Ui/UiFrame.h"
#include "Ui/UiList.h"
#include "Ui/UiText.h"
#include "Ui/UiPin.h"

struct UiNode : public UiFrame
{
    UiNode(const std::string& title, const vec2& position, const vec2& size);
    virtual ~UiNode();
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    virtual void drawPreview(const Rect& previewArea);

    void addPin(int id, const std::string& label, bool isOutput, size_t typeFlags);
    UiNode* getSourceNode(int id);
    size_t getSourceCount() const;

    virtual void displayAttributes();

    size_t getIndex(UiPin* pin) const;

    void disconnectAllPins();

protected:
    void startMove(const vec2& mousePos) override;
    void endMove(const vec2& mousePos) override;
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
