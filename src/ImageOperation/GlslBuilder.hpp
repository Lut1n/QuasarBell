#ifndef QUASAR_BELL_GLSL_BUILDER_HPP
#define QUASAR_BELL_GLSL_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Math.hpp"
#include "ImageOperation/ImageOperationType.hpp"

//--------------------------------------------------------------
namespace qb
{
    std::string uv(size_t i);
    std::string va(size_t i);
    std::string in(size_t i);
    std::string fu(size_t i);
    std::string glslVec4(const vec4& v4);

    void replaceAll(std::string& glsl, const std::string& key, const std::string& val);
    std::string replaceArgs(const std::string& glslTemplate, const std::vector<std::string>& args);

    struct GlslContext
    {
        std::string code;

        std::list<size_t> vaStack;
        std::list<size_t> uvStack;

        size_t nextUvId = 1;
        size_t nextVaId = 0;

        void pushUv(size_t id);
        void popUv();
        size_t getUvId();
        size_t getNextUv();

        void pushVa(size_t id);
        size_t popVa();
        size_t getVa();
        size_t getNextVa();

        void pushCode(const std::string& toAdd);
    };

    struct GlslFrame
    {
        bool hasUv = false;
        std::vector<vec4> inputs;
        std::unordered_map<ImageOperationType, std::string> functions;

        GlslContext mainContext;
        std::vector<GlslContext> subContexts;
        
        std::list<size_t> contextStack;

        void setFunctions(ImageOperationType operationType, const std::string& functionCode);

        size_t pushInput(const vec4& v4);
        GlslContext& getContext();

        size_t pushContext();
        void popContext();

        std::string compile();
    };

    struct GlslBuilderVisitor
    {
        std::vector<GlslFrame> frames;
        std::list<size_t> frameStack;

        GlslFrame& getCurrentFrame();

        size_t pushFrame();
        void popFrame();
    };
};


#endif // QUASAR_BELL_GLSL_BUILDER_HPP