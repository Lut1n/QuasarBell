#ifndef QUASAR_BELL_GLSL_BUILDER_HPP
#define QUASAR_BELL_GLSL_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Vec2.h"
#include "Core/Math.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "SdfOperation/SdfOperationType.hpp"
#include "App/BaseOperationNode.hpp"

//--------------------------------------------------------------
namespace qb
{
    std::string uv(size_t i);
    std::string va(size_t i);
    std::string in(size_t i);
    std::string ke(size_t i);
    std::string fu(size_t i);
    std::string sa(size_t i);
    std::string glslVec4(const vec4& v4);
    std::string tfmr(size_t i);

    void replaceAll(std::string& glsl, const std::string& key, const std::string& val);
    std::string replaceArgs(const std::string& glslTemplate, const std::vector<std::string>& args);

    struct GlslContext
    {
        std::string code;

        std::list<size_t> vaStack;
        std::list<size_t> uvStack;
        std::list<size_t> tfmrStack;

        size_t nextUvId = 1;
        size_t nextVaId = 0;
        size_t nextTfmrId = 1;

        void pushUv(size_t id);
        void popUv();
        size_t getUvId();
        size_t getNextUv();

        void pushVa(size_t id);
        size_t popVa();
        size_t getVa();
        size_t getNextVa();
        
        void pushTransform(size_t id);
        void popTransform();
        size_t getTransformId();
        size_t getNextTransform();

        void pushCode(const std::string& toAdd);
    };

    struct GlslFrame
    {
        enum class Type
        {
            Texture,
            Sdf
        };

        int resolution = 256;
        bool hasUv = false;
        Type type = Type::Texture;
        std::vector<vec4> inputs;
        std::vector<Kernel> kernels;
        std::vector<GlslFrame> frames;
        std::unordered_map<ImageOperationType, std::string> functions;
        std::unordered_map<SdfOperationType, std::string> sdfFunctions;
        std::unordered_map<std::string, std::string> optFunctions;

        GlslContext mainContext;
        std::vector<GlslContext> subContexts;
        
        std::list<size_t> contextStack;

        void setFunctions(ImageOperationType operationType, const std::string& functionCode);
        void setFunctions(SdfOperationType operationType, const std::string& functionCode);
        void setFunctions(const std::string& id, const std::string& functionCode);

        size_t pushInput(const vec4& v4);
        size_t pushKernel(const Kernel& ke);
        GlslContext& getContext();

        size_t pushContext();
        void popContext();

        void raymarcher(std::string& glsl);
        std::string compile();

        bool needUv() const { return hasUv || type == Type::Sdf; }
    };

    struct GlslBuilderVisitor : public BaseOperationVisitor
    {
        GlslFrame mainFrame;
        std::list<GlslFrame*> frameStack;

        GlslFrame& getCurrentFrame();

        size_t pushFrame(GlslFrame::Type type);
        void popFrame();
    };
};


#endif // QUASAR_BELL_GLSL_BUILDER_HPP