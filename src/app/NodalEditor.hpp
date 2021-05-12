#ifndef QUASAR_BELL_NODAL_EDITOR
#define QUASAR_BELL_NODAL_EDITOR

#include "App.hpp"

#include "font/GlyphRenderer.h"
#include "ui/UiSystem.h"
#include "ui/UiForm.h"
#include "ui/UiNode.h"
#include "ui/UiText.h"
#include "ui/UiInput.h"
#include "ui/UiButton.h"
#include "ui/UiNodeBoard.h"

#include <unordered_map>

#include "gui/NodePropertiesEdit.hpp"
#include "gui/NodeContextMenu.hpp"

#include "gui/nodal/SignalOperationNode.hpp"


struct OperationCollection
{
    std::unordered_map<size_t, std::unique_ptr<SignalOperationNode>> operations;
    size_t getId(SignalOperationNode* operation) const;
    size_t getFreeId() const;
    size_t addOperation(std::unique_ptr<SignalOperationNode>& operation);
    void setOperation(size_t id, std::unique_ptr<SignalOperationNode>& operation);
    SignalOperationNode* getOperation(size_t id);
};

struct OperationConnections
{
    struct Entry
    {
        int src;
        int src_index;
        int dst;
        int dst_index;
    };
    std::vector<Entry> entries;

    void fill(UiConnections* ui, const OperationCollection& coll);
};

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
class NodalEditorWorkSpace : public ToolWorkSpace, public UiConnections::Handler
{
public:
    void init(App* app) override;
    void onEvent(const KeyEvent& event) override;
    void update(float t) override;
    void render() override;
    
    void onConnect(UiPin* a, UiPin* b) override;
    void onDisconnect(UiPin* a, UiPin* b) override;

private:
    NodalEditorComponentGroup _components;
    App* _app = nullptr;
    bool _ready = false;

    std::unique_ptr<UiNodeBoard> nodeboard;
    OperationCollection operations; 
    UiConnections* uiConnections;
};

#endif // QUASAR_BELL_NODAL_EDITOR
