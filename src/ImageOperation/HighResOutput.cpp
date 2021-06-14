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
ImageMix::ImageMix()
    : ImageOperation(qb::ImageOperationType_Mix)
{
    makeInput("in1", ImageDataType_Float);
    makeInput("in2", ImageDataType_Float);
    makeInput("delta", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("delta", &delta, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool ImageMix::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();
    if(!sampleInput(1, t, visitor)) return false;
    size_t v2 = context.popVa();

    bool inputValid = sampleInput(2, t, visitor);
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(mix($2.xyz, $3.xyz, $4.xyz),1.0);\n";
    std::vector<std::string> args;

    if(inputValid)
    {
        size_t v3 = context.popVa();
        args = {qb::va(opId), qb::va(v1), qb::va(v2), qb::va(v3)};
    }
    else
    {
        size_t v3 = frame.pushInput({delta,delta,delta,delta});
        args = {qb::va(opId), qb::va(v1), qb::va(v2), qb::in(v3)};
    }
    glsl = qb::replaceArgs(glsl, args);
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageClamp::ImageClamp()
    : ImageOperation(qb::ImageOperationType_Clamp)
{
    makeInput("in", ImageDataType_Float);
    makeInput("edge0", ImageDataType_Float);
    makeInput("edge1", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("edge0", ImageDataType_Float, &edge0);
    makeProperty("edge1", ImageDataType_Float, &edge1);
}
//--------------------------------------------------------------
bool ImageClamp::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();

    std::string edge1Id = pushOpOrInput(1,t,visitor, {edge0,edge0,edge0,edge0});
    std::string edge2Id = pushOpOrInput(2,t,visitor, {edge1,edge1,edge1,edge1});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(clamp($2.xyz, $3.xyz, $4.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::va(v1), edge1Id, edge2Id});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageDot::ImageDot()
    : ImageOperation(qb::ImageOperationType_Dot)
{
    makeInput("in1", ImageDataType_Float);
    makeInput("in2", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool ImageDot::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1Id, in2Id;

    if (!sampleInput(0, t, visitor)) return false;
    in1Id = qb::va(context.popVa());

    if (!sampleInput(1, t, visitor)) return false;
    in2Id = qb::va(context.popVa());

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3(dot($2.xyz, $3.xyz)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1Id, in2Id});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageCross::ImageCross()
    : ImageOperation(qb::ImageOperationType_Cross)
{
    makeInput("in1", ImageDataType_Float);
    makeInput("in2", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool ImageCross::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1Id, in2Id;

    if (!sampleInput(0, t, visitor)) return false;
    in1Id = qb::va(context.popVa());

    if (!sampleInput(1, t, visitor)) return false;
    in2Id = qb::va(context.popVa());

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(cross($2.xyz, $3.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1Id, in2Id});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageStep::ImageStep()
    : ImageOperation(qb::ImageOperationType_Step)
{
    makeInput("edge", ImageDataType_Float);
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("edge", ImageDataType_Float, &edge);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageStep::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string edgeId = pushOpOrInput(0,t,visitor, {edge,edge,edge,edge});
    std::string valueId = pushOpOrInput(1,t,visitor, {value,value,value,value});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(step($2.xyz, $3.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), edgeId, valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImagePow::ImagePow()
    : ImageOperation(qb::ImageOperationType_Pow)
{
    makeInput("in", ImageDataType_Float);
    makeInput("pow", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
    makeProperty("pow", ImageDataType_Float, &powVal);
}
//--------------------------------------------------------------
bool ImagePow::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    std::string powId = pushOpOrInput(1,t,visitor, {powVal,powVal,powVal,powVal});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(pow($2.xyz, $3.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId, powId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageSqrt::ImageSqrt()
    : ImageOperation(qb::ImageOperationType_Sqrt)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageSqrt::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(sqrt($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageAbs::ImageAbs()
    : ImageOperation(qb::ImageOperationType_Abs)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageAbs::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(abs($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageSin::ImageSin()
    : ImageOperation(qb::ImageOperationType_Sin)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageSin::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(sin($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageCos::ImageCos()
    : ImageOperation(qb::ImageOperationType_Cos)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageCos::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(cos($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageTan::ImageTan()
    : ImageOperation(qb::ImageOperationType_Tan)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageTan::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(tan($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageMod::ImageMod()
    : ImageOperation(qb::ImageOperationType_Mod)
{
    makeInput("in", ImageDataType_Float);
    makeInput("mod", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
    makeProperty("mod", ImageDataType_Float, &mod);
}
//--------------------------------------------------------------
bool ImageMod::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    std::string modId = pushOpOrInput(1,t,visitor, {mod,mod,mod,mod});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(mod($2.xyz, $3.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId, modId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageLog::ImageLog()
    : ImageOperation(qb::ImageOperationType_Log)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageLog::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(log($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageExp::ImageExp()
    : ImageOperation(qb::ImageOperationType_Exp)
{
    makeInput("in", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in", ImageDataType_Float, &value);
}
//--------------------------------------------------------------
bool ImageExp::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,t,visitor, {value,value,value,value});
    
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(exp($2.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
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