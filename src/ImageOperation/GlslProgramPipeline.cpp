#include "ImageOperation/GlslProgramPipeline.hpp"
#include "Graphics/RenderInterface.h"

#include <functional>


//--------------------------------------------------------------
std::list<qb::GlslFrame*> sortImpl(std::list<qb::GlslFrame*> frameList, qb::GraphStats& stats)
{
    std::list<qb::GlslFrame*> ret;
    while(!frameList.empty())
    {
        qb::GlslFrame* next = nullptr;
        int maxWidth = 0;
        int maxDepth = -1;
        for(auto f : frameList)
        {
            if (stats.frameStats[f].width > maxWidth)
            {
                next = f;
                maxWidth = stats.frameStats[f].width;
                maxDepth = stats.frameStats[f].depth;
            }
            else if (stats.frameStats[f].width == maxWidth)
            {
                if (stats.frameStats[f].depth > maxDepth)
                {
                    next = f;
                    maxWidth = stats.frameStats[f].width;
                    maxDepth = stats.frameStats[f].depth;
                }
            }
        }

        if (next == nullptr)
            std::cout << "GlslProgramPipeline: stats error" << std::endl;
        ret.push_back(next);
        frameList.remove(next);
    }
    return ret;
}

//--------------------------------------------------------------
std::vector<qb::GlslFrame*> qb::getOrderedFrames(GlslFrame* outputFrame)
{
    GraphStats stats;

    // std::list<GlslFrame*> frameList;

    std::function<void(GlslFrame*)> getStats = [&getStats, &stats/*, &frameList*/](GlslFrame* frame)
    {
        int maxDepth = 0;
        int totalWidth = 0;

        for(auto& f : frame->frames)
        {
            getStats(&f);
            maxDepth = std::max(stats.frameStats[&f].depth, maxDepth);
            totalWidth += stats.frameStats[&f].width;
        }

        if (totalWidth == 0) totalWidth = 1;
        stats.frameStats[frame].depth = maxDepth;
        stats.frameStats[frame].width = totalWidth;
        // frameList.push_back(frame);
    };

    getStats(outputFrame);
    // std::cout << "scanning graph: " << frameList.size() << " frame detected" << std::endl;

    std::vector<GlslFrame*> ret;

    std::function<void(GlslFrame*)> orderFrame = [&orderFrame, &stats, &ret](GlslFrame* frame)
    {
        int maxDepth = 0;
        int totalWidth = 0;

        std::list<qb::GlslFrame*> frameList;
        for(auto& f : frame->frames) frameList.push_back(&f);
        
        frameList = sortImpl(frameList, stats);
        for(auto& f : frameList) orderFrame(f);

        ret.push_back(frame);
    };

    orderFrame(outputFrame);

    return ret;
}

//--------------------------------------------------------------
qb::GlslProgramPipeline::~GlslProgramPipeline()
{
    for(auto desc : orderedDescriptors)
        RenderInterface::destroyCustomProgram((unsigned)desc.programId);
}

//--------------------------------------------------------------
void qb::GlslProgramPipeline::init(std::vector<GlslFrame*>& orderedFrames)
{
    std::cout << "init program pipeline with " << orderedFrames.size() << " frame count" << std::endl;

    for(auto desc : orderedDescriptors)
        RenderInterface::destroyCustomProgram((unsigned)desc.programId);
    orderedDescriptors.clear();

    for(auto frame : orderedFrames)
    {
        Descriptor desc;
        desc.glsl = frame->compile();
        desc.voxelPlan = frame->type == qb::GlslFrame::Type::VoxelPlan;
        desc.programId = RenderInterface::createCustomProgram();
        RenderInterface::updateCustomProgram((unsigned)desc.programId, desc.glsl, frame->needUv());
        desc.inputCount = frame->inputs.size();
        desc.kernelCount = frame->kernels.size();
        desc.textureCount = frame->frames.size();
        
        orderedDescriptors.push_back(desc);
    };
}

//--------------------------------------------------------------
void qb::GlslPipelineData::init(std::vector<GlslFrame*>& orderedFrames)
{
    inputs.clear();
    kernels.clear();

    for(auto frame : orderedFrames)
    {
        inputs.insert(inputs.end(), frame->inputs.begin(), frame->inputs.end());
        kernels.insert(kernels.end(), frame->kernels.begin(), frame->kernels.end());
        
        if (frame->type == qb::GlslFrame::Type::VoxelPlan)
        {
            voxelSize = frame->voxelSize;
            targetZ = frame->targetZ;
        }
    };
}
