#ifndef QUASAR_BELL_PCM_BUILDER_HPP
#define QUASAR_BELL_PCM_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Vec2.h"
#include "Core/Math.hpp"
#include "SignalOperation/SignalOperations.hpp"
#include "App/BaseOperation.hpp"


//--------------------------------------------------------------
struct DataCollector : public BaseNodeVisitor
{
    std::vector<BaseOperationNode*> nodes;
    
    void visit(BaseOperationNode& node) override
    {
        nodes.push_back(&node);
        for(size_t i=0; i<node.getSourceCount(); ++i)
        {
            auto source = dynamic_cast<BaseOperationNode*>(node.getSourceNode(i));
            if (source)
                source->accept(*this);
        }
    }
};


struct PcmGenerator
{
    struct State
    {
        float t;
        float sampleRate;
        float duration;
        float timeStep;
        int sampleBits;
        std::string path;
        bool toPlay = false;
        bool toStop = false;
        bool toExport = false;
    };

    DataCollector collector;
    std::unordered_map<size_t, std::unique_ptr<AbstractSignalOperation>> signalOperations;

    State state;
    std::unordered_map<BaseOperationNode*, OperationInfo> infos;
    std::unordered_map<BaseOperationNode*, std::unique_ptr<BasePreview>> previews;
    BaseOperationNode* currentNode = nullptr;

    template<typename Ty>
    void registerSignalOperation()
    {
        signalOperations[Ty::TypeId] = std::make_unique<Ty>();
    }

    PcmGenerator();

    void reset();

    void prepare(BaseOperationNode* node);

    PcmPreview* createPreview();

    bool hasChanged();

    void compute(BaseOperationNode* outputNode);

    void computeResult(BaseOperationNode* node, PcmPreview* preview, Time time);

    void computePreviews(BaseOperationNode* node);

    void generateAudio(BaseOperationNode* node);
};


#endif // QUASAR_BELL_PCM_BUILDER_HPP
