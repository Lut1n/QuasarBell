#ifndef QUASAR_BELL_NODAL_EDITOR
#define QUASAR_BELL_NODAL_EDITOR

#include <unordered_map>

#include "Ui/UiNodeBoard.h"

#include "App/AppInterface.hpp"
#include "App/BaseOperationNode.hpp"

//--------------------------------------------------------------
struct OperationCollection
{
    std::unordered_map<size_t, std::unique_ptr<BaseOperationNode>> operations;
    size_t getId(BaseOperationNode* operation) const;
    size_t getFreeId() const;
    size_t addOperation(std::unique_ptr<BaseOperationNode>& operation);
    void setOperation(size_t id, std::unique_ptr<BaseOperationNode>& operation);
    BaseOperationNode* getOperation(size_t id);
    Rect getBoundingBox() const;
    void centerNodes(const Rect& area);
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

class OutputOperation;
class HighResOutput;

//--------------------------------------------------------------
class UiSignalNodeBoard : public UiConnections::Handler
{
public:
    void update(float t);
    void cleanup();
    void load(const std::string& path);
    void save(const std::string& path);
    
    void onConnect(UiPin* a, UiPin* b) override;
    void onDisconnect(UiPin* a, UiPin* b) override;

    void updatePreviews();

private:
    double _lastUpdateTime = 0.0;
    bool _ready = false;

    std::unique_ptr<UiNodeBoard> nodeboard;
    OperationCollection operations;
    UiConnections* uiConnections;
};

#endif // QUASAR_BELL_NODAL_EDITOR
