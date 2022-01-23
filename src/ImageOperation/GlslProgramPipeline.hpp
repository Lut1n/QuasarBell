#ifndef QB_GLSL_PIPELINE_HPP
#define QB_GLSL_PIPELINE_HPP

#include "ImageOperation/GlslBuilder.hpp"

//--------------------------------------------------------------
namespace qb
{
    struct FrameStats
    {
        int width = 1;
        int depth = 0;
    };

    struct GraphStats
    {
        std::unordered_map<GlslFrame*, FrameStats> frameStats;
    };

    std::vector<qb::GlslFrame*> getOrderedFrames(GlslFrame* outputFrame);

    struct GlslProgramPipeline
    {
        struct Descriptor
        {
            std::string glsl;
            size_t programId = 0;
            size_t inputCount = 0;
            size_t kernelCount = 0;
            size_t textureCount = 0;
            bool voxelPlan = false;
        };

        int resolution = 256;

        GlslProgramPipeline() = default;
        ~GlslProgramPipeline();

        std::vector<Descriptor> orderedDescriptors;

        void init(std::vector<GlslFrame*>& orderedFrames);
    };

    struct GlslPipelineData
    {
        std::vector<vec4> inputs;
        std::vector<Kernel> kernels;

        // if voxelPlan only
        float voxelSize = 0.0f;
        float targetZ = 0.0f;
        
        void init(std::vector<GlslFrame*>& orderedFrames);
    };
};


#endif // QB_GLSL_PIPELINE_HPP
