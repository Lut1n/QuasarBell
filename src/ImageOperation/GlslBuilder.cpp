#include "ImageOperation/GlslBuilder.hpp"


//--------------------------------------------------------------
std::string qb::uv(size_t i)
{
    return std::string("uv") + std::to_string(i);
}
//--------------------------------------------------------------
std::string qb::va(size_t i)
{
    return std::string("v") + std::to_string(i);
}
//--------------------------------------------------------------
std::string qb::in(size_t i)
{
    return std::string("u") + std::to_string(i);
}
//--------------------------------------------------------------
std::string qb::fu(size_t i)
{
    return std::string("func_") + std::to_string(i);
}

std::string qb::glslVec4(const vec4& v4)
{
    std::string ret = "vec4(" + std::to_string(v4.x);
    for(size_t i=1;i<4;++i) ret += ',' + std::to_string(v4.v[i]);
    return ret + ")";
}
//--------------------------------------------------------------
void qb::replaceAll(std::string& glsl, const std::string& key, const std::string& val)
{
    size_t pos = 0;
    while((pos = glsl.find(key, pos)) != std::string::npos)
    {
        glsl.replace(pos, key.length(), val);
        pos += val.length();
    }
}
//--------------------------------------------------------------
std::string qb::replaceArgs(const std::string& glslTemplate, const std::vector<std::string>& args)
{
    std::string glsl = glslTemplate;
    size_t i=0;
    for(auto& arg : args)
    {
        std::string key = "$" + std::to_string(i+1);
        replaceAll(glsl, key, arg);
        ++i;
    }
    return glsl;
}
//--------------------------------------------------------------
size_t qb::GlslContext::getUvId()
{
    if(uvStack.size() > 0)
        return uvStack.back();
    return 0;
}
//--------------------------------------------------------------
void qb::GlslContext::pushUv(size_t id)
{
    uvStack.push_back(id);
    nextUvId++;
}
//--------------------------------------------------------------
size_t qb::GlslContext::getNextUv()
{
    return nextUvId;
}
//--------------------------------------------------------------
void qb::GlslContext::popUv()
{
    uvStack.pop_back();
}
//--------------------------------------------------------------
void qb::GlslContext::pushVa(size_t id)
{
    vaStack.push_back(id);
    nextVaId++;
}
//--------------------------------------------------------------
size_t qb::GlslContext::popVa()
{
    size_t id = vaStack.front(); vaStack.pop_front();
    return id;
}
//--------------------------------------------------------------
size_t qb::GlslContext::getVa()
{
    return vaStack.front();
}
//--------------------------------------------------------------
size_t qb::GlslContext::getNextVa()
{
    return nextVaId;
}
//--------------------------------------------------------------
void qb::GlslContext::pushCode(const std::string& toAdd)
{
    code += toAdd;
}
//--------------------------------------------------------------
void qb::GlslFrame::setFunctions(ImageOperationType operationType, const std::string& functionCode)
{
    if(functions.find(operationType) == functions.end())
        functions[operationType] = functionCode;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::pushInput(const vec4& v4)
{
    size_t id = inputs.size();
    inputs.push_back(v4);
    return id;
}
//--------------------------------------------------------------
qb::GlslContext& qb::GlslFrame::getContext()
{
    if(contextStack.size() > 0)
        return subContexts[contextStack.back()];
    return mainContext;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::pushContext()
{
    size_t idx = subContexts.size();
    subContexts.push_back(GlslContext());
    contextStack.push_back(idx);
    return idx;
}
//--------------------------------------------------------------
void qb::GlslFrame::popContext()
{
    contextStack.pop_back();
}
//--------------------------------------------------------------
std::string qb::GlslFrame::compile()
{
    std::string glsl;
    glsl += "#version 330\n";

    // uniforms
    for(size_t i=0; i<inputs.size(); ++i)
        glsl += replaceArgs("uniform vec4 $1 = $2;\n", {in(i), glslVec4(inputs[i])});
    
    // in/out
    if (hasUv) glsl += "in vec2 uv0;\n";
    glsl += "out vec4 fragColor;\n";

    // functions
    for(auto& func : functions) glsl += func.second;

    // sub contexts
    int contextId = subContexts.size() - 1;
    for(auto& context : subContexts)
    {
        std::string contextTemplate = "vec4 $1(vec2 uv0){\n$2return $3;\n}\n";
        glsl += replaceArgs(contextTemplate, {fu(contextId), context.code, va(context.getVa())});
        contextId--;
    }

    // main context
    glsl += "void main(){\n";
    glsl += mainContext.code;
    glsl += replaceArgs("fragColor = $1;\n};\n", {va(mainContext.getVa())});
    
    return glsl;
}
//--------------------------------------------------------------
qb::GlslFrame& qb::GlslBuilderVisitor::getCurrentFrame()
{
    if(frameStack.size() == 0) pushFrame();
    return frames[frameStack.back()];
}
//--------------------------------------------------------------
size_t qb::GlslBuilderVisitor::pushFrame()
{
    size_t idx = frames.size();
    frames.push_back(GlslFrame());
    frameStack.push_back(idx);
    return idx;
}
//--------------------------------------------------------------
void qb::GlslBuilderVisitor::popFrame()
{
    frameStack.pop_back();
}