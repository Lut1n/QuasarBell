#include "ImageOperation/HighResOutput.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

HighResOutput* HighResOutput::defaultOutput = nullptr;

//--------------------------------------------------------------
ColorInput::ColorInput()
    : ImageOperation(qb::ImageOperationType_Color)
{
    makeProperty("r", ImageDataType_Float, &r);
    makeProperty("g", ImageDataType_Float, &g);
    makeProperty("b", ImageDataType_Float, &b);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool ColorInput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({r,g,b,a});
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void ColorInput::uiProperties()
{
    uiPreview();
    vec4 color = {r,g,b,a};
    if (ImGui::ColorEdit4("color", (float*)&color))
    {
        r = color.x; g = color.y; b = color.z; a = color.w;
        dirty(false);
    }
}
//--------------------------------------------------------------
Construct3f::Construct3f()
    : ImageOperation(qb::ImageOperationType_Construct3f)
{
    makeProperty("c1", ImageDataType_Float, &c1);
    makeProperty("c2", ImageDataType_Float, &c2);
    makeProperty("c3", ImageDataType_Float, &c3);
    makeInput("in1", ImageDataType_Float);
    makeInput("in2", ImageDataType_Float);
    makeInput("in3", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool Construct3f::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1Id = pushOpOrInput(0,t,visitor, {c1,c1,c1,1.0f});
    std::string in2Id = pushOpOrInput(1,t,visitor, {c2,c2,c2,1.0f});
    std::string in3Id = pushOpOrInput(2,t,visitor, {c3,c3,c3,1.0f});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4($2.x,$3.x,$4.x,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1Id, in2Id, in3Id});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
Split3f::Split3f()
    : ImageOperation(qb::ImageOperationType_Split3f)
{
    makeProperty("r", ImageDataType_Float, &r);
    makeProperty("g", ImageDataType_Float, &g);
    makeProperty("b", ImageDataType_Float, &b);
    makeProperty("index", &index, 0, 2);
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool Split3f::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string inId = qb::in(frame.pushInput({r,g,b,(float)this->index}));
    std::string vaId;
    
    std::string glsl = "vec4 $1 = vec4(vec3($3[int($2.w)]),1.0);\n";
    
    if(sampleInput(0, t, visitor))
        vaId = qb::va(context.popVa());
    else
        vaId = inId;

    size_t opId = context.getNextVa();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), inId, vaId});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void Split3f::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    vec4 color = {r,g,b,0.0f};
    if (ImGui::ColorEdit3("color", (float*)&color))
    {
        r = color.x; g = color.y; b = color.z;
        dirty(false);
    }
    uiProperty(3);
    ImGui::EndChild();
}

//--------------------------------------------------------------
DirectionalSignal::DirectionalSignal()
    : ImageOperation(qb::ImageOperationType_DirSignal)
{
    makeProperty("direction x", &directionX, -1.0, 1.0);
    makeProperty("direction y", &directionY, -1.0, 1.0);
    makeProperty("frequency", &frequency, 1.0, 10.0);
    makeProperty("amplitude", &amplitude, 0.0, 1.0);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool DirectionalSignal::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({directionX,directionY,frequency,amplitude});
    size_t uvId = context.getUvId();
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3($3.w * (sin( dot($2.xy,normalize($3.xy)) * $3.z * 6.28 ) * 0.5 + 0.5)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
}
//--------------------------------------------------------------
RadialSignal::RadialSignal()
    : ImageOperation(qb::ImageOperationType_RadSignal)
{
    makeProperty("center x", &centerX, 0.0, 1.0);
    makeProperty("center y", &centerY, 0.0, 1.0);
    makeProperty("frequency", &frequency, 1.0, 10.0);
    makeProperty("amplitude", &amplitude, 0.0, 1.0);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool RadialSignal::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({centerX,centerY,frequency,amplitude});
    size_t uvId = context.getUvId();
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3($3.w * (sin( distance($2.xy,$3.xy) * $3.z * 6.28 ) * 0.5 + 0.5)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
}
//--------------------------------------------------------------
Dynamics::Dynamics()
    : ImageOperation(qb::ImageOperationType_Dynamics)
{
    makeInput("in", ImageDataType_Float);
    makeInput("brightness", ImageDataType_Float);
    makeInput("contrast", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("brightness", &brightness, -1.0f, 1.0f);
    makeProperty("contrast", &contrast, 0.0f, 10.0f);
}
//--------------------------------------------------------------
bool Dynamics::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    if(sampleInput(0, t, visitor))
    {
        std::string inId = qb::va(context.popVa());
        size_t outId = context.getNextVa();
        
        std::string briId = pushOpOrInput(1,t,visitor, {brightness,brightness,brightness,brightness});
        std::string contId = pushOpOrInput(2,t,visitor, {contrast,contrast,contrast,contrast});
        
        std::string glsl = "vec4 $1 = dynamics($2, vec4($3.x,$4.x,0.0,0.0));\n";
        glsl = qb::replaceArgs(glsl, {qb::va(outId), inId, briId, contId});
        context.pushVa(outId);
        context.pushCode(glsl);
        frame.setFunctions(getNodeType(), getOperationCode());
        return true;
    }
    return false;
}
//--------------------------------------------------------------
std::string Dynamics::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 dynamics(vec4 color, vec4 bc){\n"
    "    vec3 c = (color.xyz - 0.5f) * bc.y + 0.5f + bc.x;\n"
    "    return vec4(c,1.0f);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
HighResOutput::HighResOutput()
    : ImageOperation(qb::ImageOperationType_HighResOutput)
{
    preview.resolution = (int)std::pow(2.0,(float)resolution);
    makeInput("in", ImageDataType_Float);
    makeProperty("resolution", ImageDataType_Int, &resolution);

    if (defaultOutput == nullptr) defaultOutput = this;
}
//--------------------------------------------------------------
HighResOutput::~HighResOutput()
{
    if (defaultOutput == this) defaultOutput = nullptr;
}
//--------------------------------------------------------------
bool HighResOutput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    size_t opId;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {1.0f,1.0f,1.0f,1.0f});

    opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void HighResOutput::uiProperties()
{
    uiPreview();
    
    if (defaultOutput == this)
    {
        ImGui::Text("Default output");
    }
    else if (ImGui::Button("Set as default output"))
    {
        defaultOutput = this;
    }

    if (ImGui::SliderInt("power", &resolution, 0, 12))
    {
        preview.resolution = (int)std::pow(2.0,(float)resolution);
        preview.initialized = false;
        dirty(true);
    }
    int res = (int)std::pow(2.0,(float)resolution);
    ImGui::InputInt("resolution", &res);
}

//--------------------------------------------------------------
TimeInput::TimeInput()
    : ImageOperation(qb::ImageOperationType_Time)
{
    makeOutput("time", ImageDataType_Float);
}
//--------------------------------------------------------------
bool TimeInput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    float ti = RenderInterface::getTime();
    size_t inId = frame.pushInput({ti,ti,ti,ti});
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3($2.x),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}