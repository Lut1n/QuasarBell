#ifndef QUASAR_BELL_NODAL_EDITOR
#define QUASAR_BELL_NODAL_EDITOR

#include "App.hpp"
#include "gui/PostEffect.hpp"
#include "gui/InputEffect.hpp"

#include "Font/GlyphRenderer.h"
#include "UI/UiSystem.h"
#include "UI/UiForm.h"
#include "UI/UiNode.h"
#include "UI/UiText.h"
#include "UI/UiInput.h"
#include "UI/UiButton.h"
#include "UI/UiConnections.h"

#include <unordered_map>

#include "gui/NodePropertiesEdit.hpp"
#include "gui/NodeContextMenu.hpp"

//--------------------------------------------------------------
class NodalEditorComponentGroup : public GuiComponentGroup
{
public:
    NodalEditorComponentGroup();
    virtual ~NodalEditorComponentGroup();
    
public:
    NodePropertiesEdit* nodeProperties;
    NodeContextMenu* nodeContextMenu;
};

//--------------------------------------------------------------
class NodalEditorWorkSpace : public ToolWorkSpace
{
public:
    void init(App* app) override;
    void onEvent(const KeyEvent& event) override;
    void update(double t) override;
    void render() override;

private:
    PcmData generate();

private:
    NodalEditorComponentGroup _components;
    App* _app = nullptr;
    bool _ready = false;

    std::vector<std::unique_ptr<UiNode>> nodes;
    std::unique_ptr<UiText> selected;
    std::unique_ptr<UiButton> test;
    std::unique_ptr<UiConnections> cons;
};

#endif // QUASAR_BELL_NODAL_EDITOR
