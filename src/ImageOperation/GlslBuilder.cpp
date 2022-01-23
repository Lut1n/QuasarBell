#include "ImageOperation/GlslBuilder.hpp"

#include "ImageOperation/ImageOperations.hpp"
#include <functional>

namespace qb
{
    static Stack<void*, BaseAttributes*> s_currentOperation;
}

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
std::string qb::ke(size_t i)
{
    return std::string("k") + std::to_string(i);
}
//--------------------------------------------------------------
std::string qb::fu(size_t i)
{
    return std::string("func_") + std::to_string(i);
}
//--------------------------------------------------------------
std::string qb::sa(size_t i)
{
    return std::string("frame") + std::to_string(i);
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
std::string qb::tfmr(size_t i)
{
    return std::string("tfmr_") + std::to_string(i);
}
//--------------------------------------------------------------
size_t qb::GlslContext::getUvId()
{
    if(uvStack.size() > 0)
        return uvStack.get();
    return 0;
}
//--------------------------------------------------------------
void qb::GlslContext::pushUv(size_t id)
{
    uvStack.push(s_currentOperation.get(), id);
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
    uvStack.pop();
}
//--------------------------------------------------------------
void qb::GlslContext::pushTransform(size_t id)
{
    tfmrStack.push(s_currentOperation.get(), id);
    nextTfmrId++;
}
//--------------------------------------------------------------
void qb::GlslContext::popTransform()
{
    tfmrStack.pop();
}
//--------------------------------------------------------------
size_t qb::GlslContext::getTransformId()
{
    if(tfmrStack.size() > 0)
        return tfmrStack.get();
    return 0;
}
//--------------------------------------------------------------
size_t qb::GlslContext::getNextTransform()
{
    return nextTfmrId;
}
//--------------------------------------------------------------
void qb::GlslContext::pushVa(size_t id)
{
    vaStack.push(s_currentOperation.get(), id);
    nextVaId++;
}
//--------------------------------------------------------------
size_t qb::GlslContext::popVa()
{
    size_t id = vaStack.iget(); vaStack.ipop();
    return id;
}
//--------------------------------------------------------------
size_t qb::GlslContext::getVa()
{
    return vaStack.iget();
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
void qb::GlslContext::repushall()
{
    vaStack.repush(s_currentOperation.get());
    // uvStack.repush(s_currentOperation);
    // tfmrStack.repush(s_currentOperation);
}
//--------------------------------------------------------------
void qb::GlslFrame::setFunctions(ImageOperationType operationType, const std::string& functionCode)
{
    if(functions.find(operationType) == functions.end())
        functions[operationType] = functionCode;
}
//--------------------------------------------------------------
void qb::GlslFrame::setFunctions(GeometryOperationType operationType, const std::string& functionCode)
{
    if(sdfFunctions.find(operationType) == sdfFunctions.end())
        sdfFunctions[operationType] = functionCode;
}
//--------------------------------------------------------------
void qb::GlslFrame::setFunctions(const std::string& id, const std::string& functionCode)
{
    if(optFunctions.find(id) == optFunctions.end())
        optFunctions[id] = functionCode;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::uniformPlaceholder()
{
    size_t id = inputs.size();
    inputs.push_back(vec4(0,0,0,0));
    inputStack.push(s_currentOperation.get(), id);
    return id;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::kernelPlaceholder()
{
    size_t id = kernels.size();
    kernels.push_back(Kernel());
    kernelStack.push(s_currentOperation.get(), id);
    return id;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::pushInput(const vec4& v4)
{
    size_t id = inputs.size();
    inputs.push_back(v4);
    inputStack.push(s_currentOperation.get(), id);
    return id;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::pushKernel(const Kernel& ke)
{
    size_t id = kernels.size();
    kernels.push_back(ke);
    kernelStack.push(s_currentOperation.get(), id);
    return id;
}
//--------------------------------------------------------------
qb::GlslContext& qb::GlslFrame::getContext()
{
    if(contextStack.size() > 0)
        return subContexts[contextStack.get()];
    return mainContext;
}
//--------------------------------------------------------------
size_t qb::GlslFrame::pushContext()
{
    size_t idx = subContexts.size();
    subContexts.push_back(GlslContext());
    contextStack.push(s_currentOperation.get(), idx);
    return idx;
}
//--------------------------------------------------------------
void qb::GlslFrame::popContext()
{
    contextStack.pop();
}

//--------------------------------------------------------------
void qb::GlslFrame::raymarcher(std::string& glsl)
{
    glsl +=
    "vec4 sdMain(vec3 tfmr_0){\n";
    glsl += mainContext.code;
    glsl += replaceArgs("return $1;\n};\n", {va(mainContext.getVa())});

    glsl +=
    "vec3 unproj(vec2 uv, float focalLength){\n"
    "    return normalize(vec3(uv-vec2(0.5),focalLength));\n"
    "}\n"
    "vec3 calcNormal(vec3 p){\n"
    "    const float eps = 0.0001;\n"
    "    const vec2 h = vec2(eps,0);\n"
    "    return normalize(vec3(sdMain(p+h.xyy).x - sdMain(p-h.xyy).x,\n"
    "                     sdMain(p+h.yxy).x - sdMain(p-h.yxy).x,\n"
    "                     sdMain(p+h.yyx).x - sdMain(p-h.yyx).x));\n"
    "}\n"
    "vec4 raycast(vec3 ro, vec3 rd){\n"
    "    float res = 1e10;\n"
    "    float t = 1.0;\n"
    "    vec3 c = vec3(1.0);"
    "    for(int i=0; i<32; ++i){\n"
    "        vec3 pos = ro + t*rd;\n"
    "        vec4 hitc = sdMain(pos);\n"
    "        float hit = hitc.x; c=hitc.yzw;\n"
    "        if (abs(hit) < 0.001){\n"
    "           res = t;\n"
    "           break;\n"
    "        }\n"
    "        t += hit;\n"
    "    }\n"
    "    return vec4(res,c);\n"
    "}\n"
    "vec4 render(vec2 uv){\n"
    "    vec3 ro = vec3(0.0,0.0,-2.0);\n"
    "    vec3 rd = unproj(uv, 2.5);\n"
    "    vec4 res = raycast(ro, rd);\n"
    "    \n"
    "    vec3 col = vec3(0.0);\n"
    "    vec3 pos = ro + res.x*rd;\n"
    "    vec3 nor = calcNormal(pos);\n"
    "    {\n"
    "        vec3  lig = normalize( vec3(-0.5, -0.4, -0.6) );\n"
    "        vec3  hal = normalize( lig-rd );\n"
    "        float dif = clamp( dot( nor, lig ), 0.0, 1.0 );\n"
    "        float spe = pow( clamp( dot( nor, hal ), 0.0, 1.0 ),16.0);\n"
    "        col += dif;\n"
    "        col += spe;\n"
    "    }\n"
    "    return vec4(col*res.yzw,1.0);\n"
    "}\n";
}

//--------------------------------------------------------------
void qb::GlslFrame::zProjection(std::string& glsl)
{
    glsl +=
    "vec4 sdMain(vec3 tfmr_0){\n";
    glsl += mainContext.code;
    glsl += replaceArgs("return $1;\n};\n", {va(mainContext.getVa())});

    glsl +=
    "vec4 zProject(vec3 pos){\n"
    "    float res = 0.0;\n"
    "    vec3 c = vec3(0.0,0.0,0.0);\n"
    "    vec4 hitc = sdMain(pos);\n"
    "    if (abs(hitc.x) < u_zEpsilon*0.5){\n"
    "       c=hitc.yzw;\n"
    "       res = 1.0;\n"
    "    }\n"
    "    return vec4(res,c);\n"
    "}\n"
    "vec4 render(vec2 uv){\n"
    "    vec2 xy = uv * 4.0 - 2.0;\n"
    "    vec3 pos = vec3(floor(xy/u_zEpsilon + 0.5)*u_zEpsilon, u_zTargetPlan);\n"
    "    vec4 res = zProject(pos);\n"
    "    return vec4(res.yzw,res.x);\n"
    "}\n";
}

void qb::GlslFrame::repushall()
{
    // inputStack.repush(s_currentOperation);
    // kernelStack.repush(s_currentOperation);
    // frameStack.repush(s_currentOperation);
    contextStack.repush(s_currentOperation.get());
}

//--------------------------------------------------------------
std::string qb::GlslFrame::compile()
{
    std::string glsl;
    glsl += "#version 330\n";

    // uniforms
    for(size_t i=0; i<inputs.size(); ++i)
        glsl += replaceArgs("uniform vec4 $1;\n", {in(i)});//, glslVec4(inputs[i])});
        
    for(size_t i=0; i<kernels.size(); ++i)
        glsl += replaceArgs("uniform float $1[512];\n", {ke(i)});

    for(size_t i=0; i<frames.size(); ++i)
        glsl += replaceArgs("uniform sampler2D $1;\n", {sa(i)});

    if (type == Type::VoxelPlan)
    {
        glsl += "uniform float u_zTargetPlan;\n";
        glsl += "uniform float u_zEpsilon;\n";
    }
    
    // in/out
    if (needUv()) glsl += "in vec2 uv0;\n";
    glsl += "out vec4 fragColor;\n";

    glsl += "int resolution = " + std::to_string(resolution) + ";\n";

    // functions
    for(auto& func : optFunctions) glsl += func.second;
    for(auto& func : functions) glsl += func.second;
    for(auto& func : sdfFunctions) glsl += func.second;

    // sub contexts
    int contextId = (int)subContexts.size() - 1;
    for(auto rit=subContexts.rbegin(); rit!=subContexts.rend(); rit++)
    {
        auto& context = *rit;
        std::string contextTemplate = "vec4 $1(vec2 uv0){\n$2return $3;\n}\n";
        glsl += replaceArgs(contextTemplate, {fu(contextId), context.code, va(context.getVa())});
        contextId--;
    }
    
    // main context
    if (type == Type::Sdf)
    {
        raymarcher(glsl);
        glsl += "void main(){\n";
        glsl += "fragColor = render(uv0);\n};\n";
    }
    else if (type == Type::VoxelPlan)
    {
        zProjection(glsl);
        glsl += "void main(){\n";
        glsl += "fragColor = render(uv0);\n};\n";
    }
    else if (type == Type::Texture)
    {
        glsl += "void main(){\n";
        glsl += mainContext.code;
        glsl += replaceArgs("fragColor = $1;\n};\n", {va(mainContext.getVa())});
    }
    
    return glsl;
}

size_t qb::GlslFrame::totalFrameCount()
{
    size_t ret = 1;
    for(size_t i=0; i<frames.size(); ++i)
        ret += frames[i].totalFrameCount();
    return ret;
}

//--------------------------------------------------------------
qb::GlslFrame& qb::GlslBuilderVisitor::getCurrentFrame()
{
    if(frameStack.size() > 0)
        return *frameStack.get();
    return mainFrame;
}
//--------------------------------------------------------------
size_t qb::GlslBuilderVisitor::pushFrame(GlslFrame::Type type)
{
    auto& frames = getCurrentFrame().frames;
    size_t idx = frames.size();
    frames.push_back(GlslFrame());
    frames.back().type = type;
    frames.back().resolution = mainFrame.resolution;
    frameStack.push(s_currentOperation.get(), &frames.back());
    return idx;
}
//--------------------------------------------------------------
void qb::GlslBuilderVisitor::popFrame()
{
    frameStack.pop();
}

void qb::GlslBuilderVisitor::setCurrentOperation(BaseAttributes* o)
{
    s_currentOperation.push(0,o);
}

void qb::GlslBuilderVisitor::unsetCurrentOperation()
{
    s_currentOperation.pop();
}

size_t qb::GlslBuilderVisitor::repushall()
{
    size_t id = 0;
    for(auto& item : frameStack.data)
    {
        if (item.first == s_currentOperation.get())
            break;
        id++;
    }

    frameStack.repush(s_currentOperation.get());
    return id;
}

//--------------------------------------------------------------
bool qb::GlslBuilderVisitor::sampleInput(TextureOperationResult& result, TextureOperationInfo* input, bool uniformRequest)
{
    if (input == nullptr)
        return false;

    setCurrentOperation(input->attributes);

    auto& context = getCurrentFrame().getContext();
    auto& visited = context.visited;
    auto it = visited.find(input->attributes);

    bool ret = false;

    if (it != visited.end())
    {
        context.repushall();
        ret = it->second;
    }
    else
    {
        ret = false;
        if (input->attributes)
        {
            if (uniformRequest)
                input->operation->setUniforms(result, input->attributes, input->inputs);
            else
                input->operation->buildProgram(result, input->attributes, input->inputs);
            ret = true;
        }
        visited.emplace(input->attributes, ret);
    }

    unsetCurrentOperation();
    return ret;
}

//--------------------------------------------------------------
bool qb::GlslBuilderVisitor::sampleInContext(TextureOperationResult& result, TextureOperationInfo* input, size_t& ctxId, bool uniformRequest)
{
    if (input == nullptr)
        return false;

    auto& frame = getCurrentFrame();

    ctxId = frame.pushContext();
    bool ret = sampleInput(result, input, uniformRequest);
    frame.popContext();
    return ret;
}

//--------------------------------------------------------------
bool qb::GlslBuilderVisitor::sampleInFrame(TextureOperationResult& result, TextureOperationInfo* input, size_t& frameId, bool uniformRequest)
{
    if (input == nullptr)
        return false;

    auto& frame = getCurrentFrame();

    auto it = visited.end();

    auto target = input->attributes;
    it = visited.find(target);

    bool ret = false;
    setCurrentOperation(target);
    if (it != visited.end())
    {
        frameId = repushall();
        ret = true;
    }
    else
    {
        frameId = pushFrame(input->isSdf ? qb::GlslFrame::Type::Sdf : qb::GlslFrame::Type::Texture);
        ret = sampleInput(result, input, uniformRequest);
        visited.emplace(target, ret);
    }
    popFrame();
    unsetCurrentOperation();

    return ret;
}

//--------------------------------------------------------------
void qb::GlslBuilderVisitor::inputOrUniform(TextureOperationResult& result, TextureOperationInfo* input, const vec4 v, bool isSdf)
{
    auto& frame = getCurrentFrame();
    if (input && input->isSdf != isSdf)
    {
        size_t frameId;
        if (sampleInFrame(result, input, frameId, true))
            return;
    }
    if (!sampleInput(result, input, true))
        frame.pushInput(v);
}
std::string qb::GlslBuilderVisitor::inputOrUniformPlaceholder(TextureOperationResult& result, TextureOperationInfo* input, bool isSdf)
{
    auto& frame = getCurrentFrame();
    auto& context = frame.getContext();

    // do a projection
    if (input && input->isSdf != isSdf)
    {
        size_t frameId;
        if (sampleInFrame(result, input, frameId, false))
        {
            frame.hasUv = true;
            return "texture2D(" + qb::sa(frameId) + "," + qb::uv(context.getUvId()) + ")";
        }
    }

    if (sampleInput(result, input, false))
        return qb::va(context.popVa());
    else
        return qb::in(frame.uniformPlaceholder());
}
//--------------------------------------------------------------
void qb::GlslBuilderVisitor::pushFallback(TextureOperationResult& result)
{
    auto& frame = getCurrentFrame();
    auto& context = frame.getContext();


    size_t opId = context.getNextVa();
    std::string glsl = "vec4 v$1 = vec4(0,0,0,1);";
    glsl = qb::replaceArgs(glsl, {std::to_string(opId)});
    context.pushVa(opId);
    context.pushCode(glsl);
}
void qb::GlslBuilderVisitor::startUniforms()
{
    visited.clear();
    frameStack.clear();
    mainFrame.clear();
}

void qb::GlslFrame::clear()
{
    targetZ = 0.0f;
    voxelSize = 0.1f;
    hasUv = false;
    inputs.clear();
    kernels.clear();
    frames.clear();

    mainContext.clear();
    subContexts.clear();
    
    inputStack.clear();
    kernelStack.clear();
    frameStack.clear();
    contextStack.clear();
}

void qb::GlslContext::clear()
{
    visited.clear();
    
    vaStack.clear();
    uvStack.clear();
    tfmrStack.clear();

    nextUvId = 1;
    nextVaId = 0;
    nextTfmrId = 1;
}