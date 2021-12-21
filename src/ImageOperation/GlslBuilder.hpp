#ifndef QUASAR_BELL_GLSL_BUILDER_HPP
#define QUASAR_BELL_GLSL_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Vec2.h"
#include "Core/Math.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "GeometryOperation/GeometryOperationType.hpp"
//#include "App/BaseOperationNode.hpp"
#include "App/BaseAttributes.hpp"

//--------------------------------------------------------------
namespace qb
{
    template<typename ID, typename T>
    struct Stack
    {
        std::vector<std::pair<ID,T>> data;
        std::list<size_t> idStack;

        void push(ID id, T value)
        {
            idStack.push_back(data.size());
            data.push_back(std::make_pair(id,value));
        }

        void repush(ID id)
        {
            std::vector<T> topush;
            for(auto& item : data)
            {
                if (item.first == id)
                    topush.push_back(item.second);
            }
            for(auto& value : topush)
                push((ID)0, value);
        }

        void pop()
        {
            idStack.pop_back();
        }

        void ipop()
        {
            idStack.pop_front();
        }

        T& get()
        {
            return data[idStack.back()].second;
        }

        T& iget()
        {
            return data[idStack.front()].second;
        }

        size_t size()
        {
            return idStack.size();
        }
    };

    using IdStack = Stack<BaseAttributes*,size_t>;

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
        std::unordered_map<BaseAttributes*, bool> visited;
        std::string code;

        IdStack vaStack;
        IdStack uvStack;
        IdStack tfmrStack;

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

        void repushall();
    };

    struct GlslFrame
    {
        enum class Type
        {
            Texture,
            Sdf,
            VoxelPlan
        };

        int resolution = 256;
        float targetZ = 0.0f;
        float voxelSize = 0.1f;
        bool hasUv = false;
        Type type = Type::Texture;
        std::vector<vec4> inputs;
        std::vector<Kernel> kernels;
        std::vector<GlslFrame> frames;

        std::unordered_map<ImageOperationType, std::string> functions;
        std::unordered_map<GeometryOperationType, std::string> sdfFunctions;
        std::unordered_map<std::string, std::string> optFunctions;

        GlslContext mainContext;
        std::vector<GlslContext> subContexts;
        
        IdStack inputStack;
        IdStack kernelStack;
        IdStack frameStack;
        IdStack contextStack;

        void setFunctions(ImageOperationType operationType, const std::string& functionCode);
        void setFunctions(GeometryOperationType operationType, const std::string& functionCode);
        void setFunctions(const std::string& id, const std::string& functionCode);

        size_t pushInput(const vec4& v4);
        size_t pushKernel(const Kernel& ke);
        GlslContext& getContext();

        size_t pushContext();
        void popContext();

        void raymarcher(std::string& glsl);
        void zProjection(std::string& glsl);
        std::string compile();

        bool needUv() const { return hasUv || type == Type::Sdf || type == Type::VoxelPlan; }

        void repushall();

        size_t totalFrameCount();
    };

    struct GlslBuilderVisitor
    {
        std::unordered_map<BaseAttributes*, bool> visited;

        GlslFrame mainFrame;
        Stack<BaseAttributes*, GlslFrame*> frameStack;

        GlslFrame& getCurrentFrame();

        size_t pushFrame(GlslFrame::Type type);
        void popFrame();

        void setCurrentOperation(BaseAttributes* o);
        void unsetCurrentOperation();
        size_t repushall();
    };
};


#endif // QUASAR_BELL_GLSL_BUILDER_HPP
