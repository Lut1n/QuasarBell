#ifndef QUASAR_BELL_NODAL_EDITOR
#define QUASAR_BELL_NODAL_EDITOR

#include <unordered_map>

#include "Ui/UiNodeBoard.h"

#include "App/AppInterface.hpp"
#include "App/SignalNode.hpp"


//--------------------------------------------------------------
struct OperationCollection
{
    std::unordered_map<size_t, std::unique_ptr<SignalNode>> operations;
    size_t getId(SignalNode* operation) const;
    size_t getFreeId() const;
    size_t addOperation(std::unique_ptr<SignalNode>& operation);
    void setOperation(size_t id, std::unique_ptr<SignalNode>& operation);
    SignalNode* getOperation(size_t id);
};

//--------------------------------------------------------------
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
class UiSignalNodeBoard : public UiConnections::Handler
{
public:
    void update(float t);
    
    void onConnect(UiPin* a, UiPin* b) override;
    void onDisconnect(UiPin* a, UiPin* b) override;

private:
    bool _ready = false;

    std::unique_ptr<UiNodeBoard> nodeboard;
    OperationCollection operations; 
    UiConnections* uiConnections;
};

#endif // QUASAR_BELL_NODAL_EDITOR