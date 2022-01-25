#ifndef QB_TEXTURE_BUILDER_HPP
#define QB_TEXTURE_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Vec2.h"
#include "Core/Math.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "GeometryOperation/GeometryOperationType.hpp"
#include "ImageOperation/ImageOperations.hpp"
#include "SdfOperation/SdfOperations.hpp"
#include "App/BaseOperationNode.hpp"

//--------------------------------------------------------------
struct ImageDataCollector : public BaseNodeVisitor
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

struct FrameRenderer;

//--------------------------------------------------------------
struct TextureGenerator
{
    struct State
    {
        int defaultResolution = 256;
        int resolution = 256;
        float time = 0.0f;

        std::string path;
        BaseOperationNode* selectedNode = nullptr;
        bool toExport = false;
        bool toCancel = false;
        bool sdfMode = false;
        bool voxelOutput = false;
        bool recompile = false;
    };

    ImageDataCollector collector;
    std::unordered_map<size_t, std::unique_ptr<AbstractImageOperation>> imageOperations;
    std::unordered_map<size_t, std::unique_ptr<AbstractImageOperation>> sdfOperations;

    State state;
    std::unordered_map<BaseOperationNode*, TextureOperationInfo> infos;
    std::unordered_map<BaseOperationNode*, std::unique_ptr<BasePreview>> previews;
    BaseOperationNode* currentNode = nullptr;

    static std::unique_ptr<FrameRenderer> frameRenderer;

    template<typename Ty>
    void registerImageOperation()
    {
        imageOperations[Ty::TypeId] = std::make_unique<Ty>();
    }
    
    template<typename Ty>
    void registerSdfOperation()
    {
        sdfOperations[Ty::TypeId] = std::make_unique<Ty>();
    }

    TextureGenerator();

    void prepare(BaseOperationNode* node);
    void prepareInputInfos();

    TexturePreview* createPreview(int resolution);

    bool hasChanged();

    void compute(BaseOperationNode* outputNode);

    void computeResult(BaseOperationNode* node, TexturePreview* preview);
    void computeVoxels(BaseOperationNode* node);

    void computePreviews(BaseOperationNode* node);

    void generateTexture(BaseOperationNode* node);
};


#endif // QB_TEXTURE_BUILDER_HPP
