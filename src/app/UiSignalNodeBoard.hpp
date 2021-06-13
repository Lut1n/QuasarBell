#ifndef QUASAR_BELL_NODAL_EDITOR
#define QUASAR_BELL_NODAL_EDITOR

#include <unordered_map>

#include "Ui/UiNodeBoard.h"

#include "App/AppInterface.hpp"
#include "App/SignalNode.hpp"
#include "App/ImageNode.hpp"


//--------------------------------------------------------------
struct ImageOperationCollection
{
    std::unordered_map<size_t, std::unique_ptr<ImageNode>> operations;
    size_t getId(ImageNode* operation) const;
    size_t getFreeId() const;
    size_t addOperation(std::unique_ptr<ImageNode>& operation);
    void setOperation(size_t id, std::unique_ptr<ImageNode>& operation);
    ImageNode* getOperation(size_t id);
    Rect getBoundingBox() const;
    void centerNodes(const Rect& area);
};

//--------------------------------------------------------------
struct OperationCollection
{
    std::unordered_map<size_t, std::unique_ptr<SignalNode>> operations;
    size_t getId(SignalNode* operation) const;
    size_t getFreeId() const;
    size_t addOperation(std::unique_ptr<SignalNode>& operation);
    void setOperation(size_t id, std::unique_ptr<SignalNode>& operation);
    SignalNode* getOperation(size_t id);
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

//--------------------------------------------------------------
struct ImageOperationConnections
{
    struct Entry
    {
        int src;
        int src_index;
        int dst;
        int dst_index;
    };
    std::vector<Entry> entries;

    void fill(UiConnections* ui, const ImageOperationCollection& coll);
};

//--------------------------------------------------------------
class UiSignalNodeBoard : public UiConnections::Handler
{
public:
    void update(float t);
    
    void onConnect(UiPin* a, UiPin* b) override;
    void onDisconnect(UiPin* a, UiPin* b) override;

    void initializePreviews();
    void updatePreviews();

private:
    double _lastUpdateTime = 0.0;
    bool _ready = false;

    std::unique_ptr<UiNodeBoard> nodeboard;
    OperationCollection operations; 
    ImageOperationCollection imageOperations; 
    UiConnections* uiConnections;
};

#endif // QUASAR_BELL_NODAL_EDITOR
