#ifndef QUASAR_BELL_RM_BUILDER_HPP
#define QUASAR_BELL_RM_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Vec2.h"
#include "Core/Math.hpp"
#include "SdfOperation/SdfOperationType.hpp"
#include "App/BaseOperationNode.hpp"

//--------------------------------------------------------------
namespace qb
{
    /*std::string uv(size_t i);
    std::string va(size_t i);
    std::string in(size_t i);
    std::string ke(size_t i);
    std::string fu(size_t i);
    std::string sa(size_t i);
    std::string glslVec4(const vec4& v4);

    void replaceAll(std::string& glsl, const std::string& key, const std::string& val);
    std::string replaceArgs(const std::string& glslTemplate, const std::vector<std::string>& args);
*/
    std::string tfmr(size_t i);

    struct RMContext
    {
        std::string code;

        std::list<size_t> vaStack;
        std::list<size_t> uvStack;
        std::list<size_t> tfmrStack;

        size_t nextUvId = 1;
        size_t nextTfmrId = 1;
        size_t nextVaId = 0;

        void pushUv(size_t id);
        void popUv();
        size_t getUvId();
        size_t getNextUv();
        
        void pushTransform(size_t id);
        void popTransform();
        size_t getTransformId();
        size_t getNextTransform();

        void pushVa(size_t id);
        size_t popVa();
        size_t getVa();
        size_t getNextVa();

        void pushCode(const std::string& toAdd);
    };

    struct RMFrame
    {
        int resolution = 256;
        std::vector<vec4> inputs;
        std::vector<Kernel> kernels;
        std::vector<RMFrame> frames;
        std::unordered_map<SdfOperationType, std::string> functions;

        RMContext mainContext;
        std::vector<RMContext> subContexts;
        
        std::list<size_t> contextStack;

        void setFunctions(SdfOperationType operationType, const std::string& functionCode);

        size_t pushInput(const vec4& v4);
        size_t pushKernel(const Kernel& ke);
        RMContext& getContext();

        size_t pushContext();
        void popContext();

        std::string compile();
    };

    struct RMBuilderVisitor : public BaseOperationVisitor
    {
        RMFrame mainFrame;
        std::list<RMFrame*> frameStack;

        RMFrame& getCurrentFrame();

        void pushFrame();
        void popFrame();
    };
};


#endif // QUASAR_BELL_RM_BUILDER_HPP