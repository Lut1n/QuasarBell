#ifndef UI_NODEBOARD_H
#define UI_NODEBOARD_H

#include <list>

#include "Ui/UiSystem.h"
#include "Ui/UiFrame.h"
#include "Ui/UiNode.h"
#include "Ui/UiConnections.h"

struct UiNodeBoard : public UiFrame
{
    UiNodeBoard(const vec2& position, const vec2& size);
    virtual ~UiNodeBoard();

    bool onEvent(const UiEvent& event) override;
    void draw() override;

protected:
    void startMove(const vec2& mousePos) override;
    void endMove(const vec2& mousePos) override;
    void onMove(const vec2& delta) override;
    
public:
    // static bool isSelected(UiNode* node);
    // static std::list<UiNode*> selected;
    
    std::unique_ptr<UiConnections> connections;
    bool requestContextMenu = false;
    vec2 contextMenuPosition;

    bool areaSelection = false;
    Rect selectRect;
};

#endif // UI_NODEBOARD_H
