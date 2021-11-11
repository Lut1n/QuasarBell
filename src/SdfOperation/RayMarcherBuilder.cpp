#include "SdfOperation/RayMarcherBuilder.hpp"

#include "ImageOperation/GlslBuilder.hpp"

/*
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
}*/
/*std::string qb::tfmr(size_t i)
{
    return std::string("tfmr_") + std::to_string(i);
}

//--------------------------------------------------------------
size_t qb::RMContext::getUvId()
{
    if(uvStack.size() > 0)
        return uvStack.back();
    return 0;
}
//--------------------------------------------------------------
void qb::RMContext::pushUv(size_t id)
{
    uvStack.push_back(id);
    nextUvId++;
}
//--------------------------------------------------------------
size_t qb::RMContext::getNextUv()
{
    return nextUvId;
}
//--------------------------------------------------------------
void qb::RMContext::popUv()
{
    uvStack.pop_back();
}
//--------------------------------------------------------------
void qb::RMContext::pushTransform(size_t id)
{
    tfmrStack.push_back(id);
    nextTfmrId++;
}
//--------------------------------------------------------------
void qb::RMContext::popTransform()
{
    tfmrStack.pop_back();
}
//--------------------------------------------------------------
size_t qb::RMContext::getTransformId()
{
    if(tfmrStack.size() > 0)
        return tfmrStack.back();
    return 0;
}
//--------------------------------------------------------------
size_t qb::RMContext::getNextTransform()
{
    return nextTfmrId;
}
//--------------------------------------------------------------
void qb::RMContext::pushVa(size_t id)
{
    vaStack.push_back(id);
    nextVaId++;
}
//--------------------------------------------------------------
size_t qb::RMContext::popVa()
{
    size_t id = vaStack.front(); vaStack.pop_front();
    return id;
}
//--------------------------------------------------------------
size_t qb::RMContext::getVa()
{
    return vaStack.front();
}
//--------------------------------------------------------------
size_t qb::RMContext::getNextVa()
{
    return nextVaId;
}
//--------------------------------------------------------------
void qb::RMContext::pushCode(const std::string& toAdd)
{
    code += toAdd;
}
//--------------------------------------------------------------
void qb::RMFrame::setFunctions(SdfOperationType operationType, const std::string& functionCode)
{
    if(functions.find(operationType) == functions.end())
        functions[operationType] = functionCode;
}
//--------------------------------------------------------------
size_t qb::RMFrame::pushInput(const vec4& v4)
{
    size_t id = inputs.size();
    inputs.push_back(v4);
    return id;
}
//--------------------------------------------------------------
size_t qb::RMFrame::pushKernel(const Kernel& ke)
{
    size_t id = kernels.size();
    kernels.push_back(ke);
    return id;
}
//--------------------------------------------------------------
qb::RMContext& qb::RMFrame::getContext()
{
    if(contextStack.size() > 0)
        return subContexts[contextStack.back()];
    return mainContext;
}
//--------------------------------------------------------------
size_t qb::RMFrame::pushContext()
{
    size_t idx = subContexts.size();
    subContexts.push_back(RMContext());
    contextStack.push_back(idx);
    return idx;
}
//--------------------------------------------------------------
void qb::RMFrame::popContext()
{
    contextStack.pop_back();
}*/
//--------------------------------------------------------------
/*std::string qb::RMFrame::compile()
{
    std::string glsl;
    glsl += "#version 330\n";

    // uniforms
    for(size_t i=0; i<inputs.size(); ++i)
        glsl += replaceArgs("uniform vec4 $1 = $2;\n", {in(i), glslVec4(inputs[i])});
        
    for(size_t i=0; i<kernels.size(); ++i)
        glsl += replaceArgs("uniform float $1[512];\n", {ke(i)});

    for(size_t i=0; i<frames.size(); ++i)
        glsl += replaceArgs("uniform sampler2D $1;\n", {sa(i)});
    
    // in/out
    glsl += "in vec2 uv0;\n";
    glsl += "out vec4 fragColor;\n";

    glsl += "int resolution = " + std::to_string(resolution) + ";\n";

    // functions
    for(auto& func : functions) glsl += func.second;

    // sub contexts
    int contextId = (int)subContexts.size() - 1;
    for(auto rit=subContexts.rbegin(); rit!=subContexts.rend(); rit++)
    {
        auto& context = *rit;
        std::string contextTemplate = "vec4 $1(vec2 uv0){\n$2return $3;\n}\n";
        glsl += replaceArgs(contextTemplate, {fu(contextId), context.code, va(context.getVa())});
        contextId--;
    }

    
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

    // main context
    glsl += "void main(){\n";
    glsl += "fragColor = render(uv0);\n};\n";
    
    return glsl;
}*/
//--------------------------------------------------------------
/*qb::RMFrame& qb::RMBuilderVisitor::getCurrentFrame()
{
    if(frameStack.size() > 0)
        return *dynamic_cast<RMFrame*>(frameStack.back().get());
    return mainFrame;
}
//--------------------------------------------------------------
void qb::RMBuilderVisitor::pushFrame(bool glsl)
{
    auto& frames = getCurrentFrame().frames;
    if (glsl)
        frames.push_back(std::unique_ptr<GlslFrame>());
    else
        frames.push_back(std::unique_ptr<RMFrame>());
    frames.back()->resolution = mainFrame.resolution;
    frameStack.push_back(frames.back());
}
//--------------------------------------------------------------
void qb::RMBuilderVisitor::popFrame()
{
    frameStack.pop_back();
}*/