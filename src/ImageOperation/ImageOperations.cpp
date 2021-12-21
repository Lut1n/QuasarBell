#include "ImageOperation/ImageOperations.hpp"

//--------------------------------------------------------------
namespace qb
{
    static constexpr std::string_view dynamicsCode =
    "vec4 dynamics(vec4 color, vec4 bc){\n"
    "    vec3 c = (color.xyz - 0.5) * bc.y + 0.5 + bc.x;\n"
    "    return vec4(c,1.0);\n"
    "}\n";
}


//--------------------------------------------------------------
bool sampleInput(TextureOperationResult& result, TextureOperationInfo* input)
{
    if (input == nullptr)
        return false;

    auto& glslBuilder = result.visitor;
    glslBuilder.setCurrentOperation(input->attributes);

    auto& context = glslBuilder.getCurrentFrame().getContext();
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
            input->operation->apply(result, input->attributes, input->inputs);
            ret = true;
        }
        visited.emplace(input->attributes, ret);
    }

    glslBuilder.unsetCurrentOperation();
    return ret;
}

//--------------------------------------------------------------
bool sampleInContext(TextureOperationResult& result, TextureOperationInfo* input, size_t& ctxId)
{
    if (input == nullptr)
        return false;

    auto& frame = result.visitor.getCurrentFrame();

    ctxId = frame.pushContext();
    bool ret = sampleInput(result, input);
    frame.popContext();
    return ret;
}

//--------------------------------------------------------------
bool sampleInFrame(TextureOperationResult& result, TextureOperationInfo* input, size_t& frameId)
{
    if (input == nullptr)
        return false;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    auto& visited = visitor.visited;
    auto it = visited.end();

    auto target = input->attributes;
    it = visited.find(target);

    bool ret = false;
    visitor.setCurrentOperation(target);
    if (it != visited.end())
    {
        frameId = visitor.repushall();
        ret = true;
    }
    else
    {
        frameId = visitor.pushFrame(input->isSdf ? qb::GlslFrame::Type::Sdf : qb::GlslFrame::Type::Texture);
        ret = sampleInput(result, input);
        visited.emplace(target, ret);
    }
    visitor.popFrame();
    visitor.unsetCurrentOperation();

    return ret;
}

//--------------------------------------------------------------
std::string pushOpOrInput(TextureOperationResult& result, TextureOperationInfo* input, const vec4 v)
{
    auto& frame = result.visitor.getCurrentFrame();
    auto& context = frame.getContext();

    // do a projection
    if (input && input->isSdf)
    {
        size_t frameId;
        if (sampleInFrame(result, input, frameId))
        {
            frame.hasUv = true;
            return "texture2D(" + qb::sa(frameId) + "," + qb::uv(context.getUvId()) + ")";
        }
    }

    if (sampleInput(result, input))
        return qb::va(context.popVa());
    else
        return qb::in(frame.pushInput(v));
}
//--------------------------------------------------------------
void pushFallback(TextureOperationResult& result)
{
    auto& frame = result.visitor.getCurrentFrame();
    auto& context = frame.getContext();


    size_t opId = context.getNextVa();
    std::string glsl = "vec4 v$1 = vec4(0,0,0,1);";
    glsl = qb::replaceArgs(glsl, {std::to_string(opId)});
    context.pushVa(opId);
    context.pushCode(glsl);
}




//--------------------------------------------------------------
void ColorInput::sample(TextureOperationResult& result, ColorData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({attributes->r,attributes->g,attributes->b,attributes->a});
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void Construct3f::sample(TextureOperationResult& result, Construct3fData* attributes, InputInfos& inputs)
{
    auto c1 = attributes->c1;
    auto c2 = attributes->c2;
    auto c3 = attributes->c3;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1Id = pushOpOrInput(result, inputs[0], {c1,c1,c1,1.0f});
    std::string in2Id = pushOpOrInput(result, inputs[1], {c2,c2,c2,1.0f});
    std::string in3Id = pushOpOrInput(result, inputs[2], {c3,c3,c3,1.0f});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4($2.x,$3.x,$4.x,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1Id, in2Id, in3Id});

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void Split3f::sample(TextureOperationResult& result, Split3fData* attributes, InputInfos& inputs)
{
    auto index = attributes->index;
    auto r = attributes->r;
    auto g = attributes->g;
    auto b = attributes->b;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string inId = qb::in(frame.pushInput({r,g,b,(float)index}));
    std::string vaId;

    std::string glsl = "vec4 $1 = vec4(vec3($3[int($2.w)]),1.0);\n";

    if(sampleInput(result, inputs[0]))
        vaId = qb::va(context.popVa());
    else
        vaId = inId;

    size_t opId = context.getNextVa();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), inId, vaId});

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void DirectionalSignal::sample(TextureOperationResult& result, DirectionalSignalData* attributes, InputInfos& inputs)
{
    auto directionX = attributes->directionX;
    auto directionY = attributes->directionY;
    auto frequency = attributes->frequency;
    auto amplitude = attributes->amplitude;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({directionX,directionY,frequency,amplitude});
    size_t uvId = context.getUvId();
    size_t opId = context.getNextVa();

    std::string glsl =
    "vec4 $1 = vec4(vec3($3.w * (sin( dot($2.xy,normalize($3.xy)) * $3.z * 6.28 ) * 0.5 + 0.5)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;
}
//--------------------------------------------------------------
void RadialSignal::sample(TextureOperationResult& result, RadialSignalData* attributes, InputInfos& inputs)
{
    auto centerX = attributes->centerX;
    auto centerY = attributes->centerY;
    auto frequency = attributes->frequency;
    auto amplitude = attributes->amplitude;

    auto& visitor = result.visitor;

    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({centerX,centerY,frequency,amplitude});
    size_t uvId = context.getUvId();
    size_t opId = context.getNextVa();

    std::string glsl =
    "vec4 $1 = vec4(vec3($3.w * (sin( distance($2.xy,$3.xy) * $3.z * 6.28 ) * 0.5 + 0.5)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;
}
//--------------------------------------------------------------
void Dynamics::sample(TextureOperationResult& result, DynamicsData* attributes, InputInfos& inputs)
{
    auto brightness = attributes->brightness;
    auto contrast = attributes->contrast;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string inId = pushOpOrInput(result, inputs[0], {1.0f,1.0f,1.0f,1.0f});
    std::string briId = pushOpOrInput(result, inputs[1], {brightness,brightness,brightness,brightness});
    std::string contId = pushOpOrInput(result, inputs[2], {contrast,contrast,contrast,contrast});

    size_t outId = context.getNextVa();

    std::string glsl = "vec4 $1 = dynamics($2, vec4($3.x,$4.x,0.0,0.0));\n";
    glsl = qb::replaceArgs(glsl, {qb::va(outId), inId, briId, contId});
    context.pushVa(outId);
    context.pushCode(glsl);
    frame.setFunctions((qb::ImageOperationType)DynamicsData::TypeId, std::string(qb::dynamicsCode));
}
//--------------------------------------------------------------
void HighResOutput::sample(TextureOperationResult& result, ImageOutput* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    size_t opId;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(result, inputs[0], {1.0f,1.0f,1.0f,1.0f});

    opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void TimeInput::sample(TextureOperationResult& result, TimeData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    float ti = (float)RenderInterface::getTime();
    size_t inId = frame.pushInput({ti,ti,ti,ti});
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3($2.x),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void UvInput::sample(TextureOperationResult& result, UvInputData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4(uv0,0.0,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;
}
//--------------------------------------------------------------
void SphericalCoord::sample(TextureOperationResult& result, SphericalCoordData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4(uv_to_sphericalcoord(uv0),0.0,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions((qb::ImageOperationType)SphericalCoordData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
std::string SphericalCoord::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec2 uv_to_sphericalcoord(vec2 uv){\n"
    "    float b = (uv.y - 0.5) * 2.0;\n"
    "    return vec2(uv.x, asin(b) / 3.141592 + 0.5);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void UvDistortion::sample(TextureOperationResult& result, UvDistortionData* attributes, InputInfos& inputs)
{
    auto force = attributes->force;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string inputUv, inputNormals;

    if(sampleInput(result, inputs[0])) inputUv = qb::va( context.popVa() );
    else inputUv = "uv0";
    if(sampleInput(result, inputs[1])) inputNormals = qb::va( context.popVa() );
    else inputNormals = "vec4(0.5,0.5,1.0,1.0)";

    std::string forceIn = qb::in( frame.pushInput({force,force,force,force}) );

    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4($2.xy + ($3.xy - vec2(0.5)) * $4.x,0.0,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), inputUv, inputNormals, forceIn});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;
}
//--------------------------------------------------------------
void UvMapping::sample(TextureOperationResult& result, UvMappingData* attributes, InputInfos& inputs)
{
    auto uOft = attributes->uOft;
    auto vOft = attributes->vOft;
    auto uFct = attributes->uFct;
    auto vFct = attributes->vFct;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string glsl;

    bool inputValid = sampleInput(result, inputs[1]); // uv map

    size_t uvId2 = context.getNextUv();
    size_t uvId = context.getUvId();

    if(inputValid)
    {
        size_t v1 = context.popVa();
        glsl = "vec2 $1 = uv_mapping($2,vec4($3.xy-uv0, 1.0,1.0));\n";
        glsl = qb::replaceArgs(glsl, {qb::uv(uvId2), qb::uv(uvId), qb::va(v1)});
    }
    else
    {
        size_t in1 = frame.pushInput({uOft, vOft, uFct, vFct});
        glsl = "vec2 $1 = uv_mapping($2,$3);\n";
        glsl = qb::replaceArgs(glsl, {qb::uv(uvId2), qb::uv(uvId), qb::in(in1)});
    }
    frame.setFunctions((qb::ImageOperationType)UvMappingData::TypeId, getOperationCode());
    frame.hasUv = true;

    context.pushCode(glsl);
    context.pushUv(uvId2);
    bool ret = sampleInput(result, inputs[0]);
    context.popUv();
    if (!ret)
        pushFallback(result);
}
//--------------------------------------------------------------
std::string UvMapping::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec2 uv_mapping(vec2 uv, vec4 oft){\n"
    "    return uv * oft.zw + oft.xy;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void BlurFilter::sample(TextureOperationResult& result, BlurFilterData* attributes, InputInfos& inputs)
{
    auto radius = attributes->radius;

    updateKernel(radius);

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    size_t frameId = 0;
    if(sampleInFrame(result, inputs[0], frameId))
    {
        auto& context = frame.getContext();

        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t ke1 = frame.pushKernel(kernel);
        size_t uvId = context.getUvId();

        std::string glsl = "vec4 v$1 = image_filter($3, $4, $5, int($2.x));\n";
        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::sa(frameId), qb::uv(uvId), qb::ke(ke1)});
        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions((qb::ImageOperationType)BlurFilterData::TypeId, getOperationCode());
        frame.hasUv = true;
    }
    else
    {
        pushFallback(result);
    }
}
//--------------------------------------------------------------
void SharpenFilter::sample(TextureOperationResult& result, SharpenFilterData* attributes, InputInfos& inputs)
{
    auto radius = attributes->radius;

    updateKernel(radius);

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    size_t frameId = 0;
    if(sampleInFrame(result, inputs[0], frameId))
    {
        auto& context = frame.getContext();

        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t ke1 = frame.pushKernel(kernel);
        size_t uvId = context.getUvId();

        std::string glsl = "vec4 v$1 = image_filter2($3, $4, $5, int($2.x));\n";
        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::sa(frameId), qb::uv(uvId), qb::ke(ke1)});
        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions((qb::ImageOperationType)SharpenFilterData::TypeId, getOperationCode());
        frame.hasUv = true;
    }
    else
    {
        pushFallback(result);
    }
}
//--------------------------------------------------------------
std::string BlurFilter::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 image_filter(sampler2D src, vec2 uv, const in float kernel[512], int radius){\n"
    "    vec4 ret = vec4(vec3(0.0),1.0);\n"
    "    float ps = 1.0/resolution;\n"
    "    for(int i=-radius; i<=radius; ++i){\n"
    "    for(int j=-radius; j<=radius; ++j){\n"
    "        vec2 kuv = uv + vec2(i,j)*ps;\n"
    "        ret.xyz += vec3(kernel[(i+radius)*(radius*2+1)+(j+radius)]) * texture2D(src,kuv).xyz;\n"
    "    }}\n"
    "    return ret;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string SharpenFilter::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 image_filter2(sampler2D src, vec2 uv, const in float kernel[512], int radius){\n"
    "    vec4 ret = vec4(vec3(0.0),1.0);\n"
    "    float ps = 1.0/resolution;\n"
    "    for(int i=-radius; i<=radius; ++i){\n"
    "    for(int j=-radius; j<=radius; ++j){\n"
    "        vec2 kuv = uv + vec2(i,j)*ps;\n"
    "        ret.xyz += vec3(kernel[(i+radius)*(radius*2+1)+(j+radius)]) * texture2D(src,kuv).xyz;\n"
    "    }}\n"
    "    return ret;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void BlurFilter::updateKernel(int radius)
{
    // gaussian
    Kernel k0(3,3);
    for(int i=0;i<9;++i)k0.data[i] = (float)(1.0/9.0);
    kernel = k0;
    for(int i=1; i<radius; ++i)
        kernel = Kernel::convProduct(kernel, k0);
}
//--------------------------------------------------------------
void SharpenFilter::updateKernel(int radius)
{
    Kernel k0(3,3);
    k0 = { 0,-1, 0,
          -1, 5,-1,
           0,-1, 0};
    kernel = k0;
    for(int i=1; i<radius; ++i)
        kernel = Kernel::convProduct(kernel, k0);
}
//--------------------------------------------------------------
void MorphoFilter::sample(TextureOperationResult& result, MorphoFilterData* attributes, InputInfos& inputs)
{
    auto radius = attributes->radius;
    auto mode = attributes->mode;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    size_t frameId = 0;
    if(sampleInFrame(result, inputs[0], frameId))
    {
        auto& context = frame.getContext();

        float radiusf = (float)radius;
        std::string radiusId = pushOpOrInput(result, inputs[1], {radiusf,radiusf,radiusf,radiusf});

        size_t in1 = frame.pushInput({(float)mode, 0.0, 0.0, 0.0});

        size_t opId = context.getNextVa();
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(vec3(1.0-$2.x),1.0);\n"
        "float s$1 = 1.0/resolution;\n"
        "int r$1 = int($5.x);\n"
        "for(int i=-r$1; i<=r$1; ++i){\n"
        "for(int j=-r$1; j<=r$1; ++j){\n"
        "    if(i*i + j*j > r$1*r$1) continue;\n"
        "    vec2 kuv$1 = $4 + vec2(i,j)*s$1;\n"
        "    if ($2.x == 0.0)\n"
        "        v$1.xyz = min(v$1.xyz, texture2D($3,kuv$1).xyz);\n"
        "    else\n"
        "        v$1.xyz = max(v$1.xyz, texture2D($3,kuv$1).xyz);\n"
        "}}\n";

        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::sa(frameId), qb::uv(uvId), radiusId});
        context.pushVa(opId);
        context.pushCode(glsl);
        frame.hasUv = true;
    }
    else
    {
        pushFallback(result);
    }
}
//--------------------------------------------------------------
void BumpToNormal::sample(TextureOperationResult& result, BumpToNormalData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t ctx = 0;
    bool inputValid = sampleInContext(result, inputs[0], ctx);

    if(inputValid)
    {
        size_t opId = context.getNextVa();
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(0,0,0,1);\n"
        "float s$1 = 1.0/resolution;\n"
        "float a$1 = $2($3 + vec2(-1,0)*s$1).x - $2($3 + vec2(1,0)*s$1).x;\n"
        "float b$1 = $2($3 + vec2(0,-1)*s$1).x - $2($3 + vec2(0,1)*s$1).x;\n"
        "vec3 va$1 = normalize(vec3(2,0,a$1 * resolution));\n"
        "vec3 vb$1 = normalize(vec3(0,2,b$1 * resolution));\n"
        "v$1.xyz = cross(va$1,vb$1) * 0.5 + 0.5;\n";
        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::fu(ctx), qb::uv(uvId)});

        context.pushVa(opId);
        context.pushCode(glsl);
        frame.hasUv = true;
    }
    else
    {
        pushFallback(result);
    }
}
//--------------------------------------------------------------
void WhiteNoise::sample(TextureOperationResult& result, WhiteNoiseData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = white_noise($2);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions((qb::ImageOperationType)WhiteNoiseData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
std::string WhiteNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_wnoise(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec4 white_noise(vec2 uv){\n"
    "    float r = rand_wnoise(uv);\n"
    "    return vec4(r,r,r,1.0);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void ValueNoise::sample(TextureOperationResult& result, ValueNoiseData* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto smoothness = attributes->smoothness;
    auto octaves = attributes->octaves;

    std::string freqId = pushOpOrInput(result, inputs[0], {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(result, inputs[1], {persistance,persistance,persistance,persistance});
    std::string smoothId = pushOpOrInput(result, inputs[2], {smoothness,smoothness,smoothness,smoothness});

    std::string octaveId = qb::in(frame.pushInput({(float)octaves, 0.0, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = perlin($2.x, $3.x, $4.x, $5.x, $6);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveId, freqId, persId, smoothId, qb::uv(uvId)});
    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions((qb::ImageOperationType)ValueNoiseData::TypeId, getOperationCode());
    frame.hasUv = true;
}
//--------------------------------------------------------------
std::string ValueNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_perlin(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec4 perlin(float octs, float freq, float persist, float smth, vec2 uv){\n"
    "    int octaves = int(octs);\n"
    "    vec2 f = vec2(freq);\n"
    "    float per = 0.5;\n"
    "    float w = 0.0;\n"
    "    float ret = 0.0;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 oft = vec2(0.0, 1.0);\n"
    "        vec2 iuv = floor(uv * f);\n"
    "        vec2 fuv = fract(uv * f);\n"
    "        fuv = smth * fuv * fuv * (3.0 - 2.0 * fuv);\n"
    "        float s1 = rand_perlin(iuv + oft.xx);\n"
    "        float s2 = rand_perlin(iuv + oft.yx);\n"
    "        float s3 = rand_perlin(iuv + oft.xy);\n"
    "        float s4 = rand_perlin(iuv + oft.yy);\n"
    "        float s12 = mix(s1,s2,fuv.x);\n"
    "        float s34 = mix(s3,s4,fuv.x);\n"
    "        float s1234 = mix(s12,s34,fuv.y);\n"
    "        ret += s1234 * per;\n"
    "        w += per;\n"
    "        f *= 2.0;\n"
    "        per *= persist;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret / max(0.1, w);\n"
    "    return vec4(vec3(ret),1.0);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void GradientNoise::sample(TextureOperationResult& result, GradientNoiseData* attributes, InputInfos& inputs)
{
    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto octaves = attributes->octaves;
    auto smoothness = attributes->smoothness;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string freqId = pushOpOrInput(result, inputs[0], {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(result, inputs[1], {persistance,persistance,persistance,persistance});
    std::string smoothId = pushOpOrInput(result, inputs[2], {smoothness,smoothness,smoothness,smoothness});

    std::string octaveId = qb::in(frame.pushInput({(float)octaves, 0.0, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = perlin_grad($2.x, $3.x, $4.x, $5.x, $6);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveId, freqId, persId, smoothId, qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions((qb::ImageOperationType)GradientNoiseData::TypeId, getOperationCode());
    frame.hasUv = true;
}
//--------------------------------------------------------------
std::string GradientNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec2 rand2_perlin_grad(vec2 uv){\n"
    "    vec2 ret;\n"
    "    ret.x = -1.0 + 2.0*fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "    ret.y = -1.0 + 2.0*fract(sin(dot(uv, vec2(269.1836, 183.672))) * 65347.3216);\n"
    "    return ret;\n"
    "}\n"
    "vec4 perlin_grad(float octs, float freq, float persist, float smth, vec2 uv){\n"
    "    int octaves = int(octs);\n"
    "    vec2 f = vec2(freq);\n"
    "    float per = 0.5;\n"
    "    //float w = 0.0;\n"
    "    float ret = 0.0;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 oft = vec2(0.0, 1.0);\n"
    "        vec2 iuv = floor(uv * f);\n"
    "        vec2 fra = fract(uv * f);\n"
    "        vec2 fuv = smth * fra * fra * (3.0 - 2.0 * fra);\n"
    "        float s1 = dot(fra - oft.xx, (rand2_perlin_grad(iuv + oft.xx)));\n"
    "        float s2 = dot(fra - oft.yx, (rand2_perlin_grad(iuv + oft.yx)));\n"
    "        float s3 = dot(fra - oft.xy, (rand2_perlin_grad(iuv + oft.xy)));\n"
    "        float s4 = dot(fra - oft.yy, (rand2_perlin_grad(iuv + oft.yy)));\n"
    "        float s12 = mix(s1,s2,fuv.x);\n"
    "        float s34 = mix(s3,s4,fuv.x);\n"
    "        float s1234 = mix(s12,s34,fuv.y);\n"
    "        ret += s1234 * per;\n"
    "        //w += per * sign(s1234);\n"
    "        f *= 2.0;\n"
    "        per *= persist;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret*0.5+0.5;"
    "    //ret = ret / max(0.1, w);\n"
    "    return vec4(vec3(ret),1.0);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void SimplexNoise::sample(TextureOperationResult& result, SimplexNoiseData* attributes, InputInfos& inputs)
{
    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto octaves = attributes->octaves;
    auto smoothness = attributes->smoothness;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string freqId = pushOpOrInput(result, inputs[0], {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(result, inputs[1], {persistance,persistance,persistance,persistance});
    std::string smoothId = pushOpOrInput(result, inputs[2], {smoothness,smoothness,smoothness,smoothness});

    std::string octaveId = qb::in(frame.pushInput({(float)octaves, 0.0, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = simplex_grad($2.x, $3.x, $4.x, $5.x, $6);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveId, freqId, persId, smoothId, qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions((qb::ImageOperationType)SimplexNoiseData::TypeId, getOperationCode());
    frame.hasUv = true;
}
//--------------------------------------------------------------
std::string SimplexNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec2 rand2_simplex_grad(vec2 uv){\n"
    "    vec2 ret;\n"
    "    ret.x = -1.0 + 2.0*fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "    ret.y = -1.0 + 2.0*fract(sin(dot(uv, vec2(269.1836, 183.672))) * 65347.3216);\n"
    "    return ret;\n"
    "}\n"
    "vec2 skew_simplex(vec2 st){\n"
    "    vec2 r = vec2(0.0);\n"
    "    r.x = 1.1547*st.x;\n"
    "    r.y = st.y+0.5*r.x;\n"
    "    return r;\n"
    "}\n"
    "vec3 simplex_grid(vec2 st){\n"
    "    vec3 xyz;\n"
    "    st = fract(skew_simplex(st));\n"
    "    if(st.x > st.y){\n"
    "        xyz.xy = vec2(1.0-st.x,st.x-st.y);\n"
    "        xyz.z = st.y;\n"
    "    } else {\n"
    "        xyz.yz = vec2(st.y-st.x,1.0-st.y);\n"
    "        xyz.x = st.x;\n"
    "    }\n"
    "    return fract(xyz);\n"
    "}\n"
    "vec4 simplex_grad(float octs, float freq, float persist, float smth, vec2 uv){\n"
    "    int octaves = int(octs);\n"
    "    vec2 f = vec2(freq);\n"
    "    float pers = 0.5;"
    "    float r = 0.0;\n"
    "    for(int k=0;k<octaves;k++){\n"
    "        vec2 p = fract(skew_simplex(uv * f));\n"
    "        vec2 i = floor(skew_simplex(uv * f));\n"
    "        vec2 oft = vec2(0.0,1.0);\n"
    "        float r1, r2, r3;\n"
    "        if(p.x > p.y){\n"
    "            r1 = dot(p-oft.xx, rand2_simplex_grad(i + oft.xx));\n"
    "            r2 = dot(p-oft.yx, rand2_simplex_grad(i + oft.yx));\n"
    "            r3 = dot(p-oft.yy, rand2_simplex_grad(i + oft.yy));\n"
    "        } else {\n"
    "            r1 = dot(p-oft.yy, rand2_simplex_grad(i + oft.yy));\n"
    "            r2 = dot(p-oft.xy, rand2_simplex_grad(i + oft.xy));\n"
    "            r3 = dot(p-oft.xx, rand2_simplex_grad(i + oft.xx));\n"
    "        }\n"
    "        vec3 fa = simplex_grid(uv * f);\n"
    "        fa = mix(vec3(0.5),fa*fa*fa*(fa*(fa*6.-15.)+10.),smth);\n"
    "        r += dot(vec3(r1,r2,r3), fa) * pers;\n"
    "        pers *= persist;\n"
    "        f *= 2.0;\n"
    "    }\n"
    "    return vec4(vec3(r * 0.5 + 0.5),1.0);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void VoronoiNoise::sample(TextureOperationResult& result, VoronoiNoiseData* attributes, InputInfos& inputs)
{
    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto octaves = attributes->octaves;
    auto mode = attributes->mode;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string freqId = pushOpOrInput(result, inputs[0], {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(result, inputs[1], {persistance,persistance,persistance,persistance});

    std::string octaveModeId = qb::in(frame.pushInput({(float)octaves, (float)mode, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = voronoi($2, $3.x, $4.x, $5);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveModeId, freqId, persId, qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions((qb::ImageOperationType)VoronoiNoiseData::TypeId, getOperationCode());
    frame.hasUv = true;
}
//--------------------------------------------------------------
std::string VoronoiNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_voronoi(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec2 rand2_voronoi(vec2 uv){\n"
    "    vec2 ret;\n"
    "    ret.x = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "    ret.y = fract(sin(dot(uv, vec2(97.1836, 23.672))) * 65347.3216);\n"
    "    return ret;\n"
    "}\n"
    "vec4 voronoi(vec4 octmode, float freq, float persist, vec2 uv){\n"
    "    int octaves = int(octmode.x);\n"
    "    vec2 f = vec2(freq);\n"
    "    float per = persist;\n"
    "    float w = 0.0;\n"
    "    vec3 ret;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 pos = uv * f;"
    "        float minCellDist = 10.0;\n"
    "        vec2 cell = floor(uv * f);\n"
    "        vec2 closestcell;\n"
    "        vec2 closestcellpos;\n"
    "        for(int x=-1; x<=1; x++){\n"
    "        for(int y=-1; y<=1; y++){\n"
    "            vec2 cellpos = cell + vec2(x,y) + rand2_voronoi(cell + vec2(x,y));\n"
    "            float d = distance(cellpos, pos);\n"
    "            if(d < minCellDist) {minCellDist = d; closestcell = cell + vec2(x,y); closestcellpos = cellpos;}\n"
    "        }}\n"
    "        \n"
    "        float minEdgeDist = 10;\n"
    "        for(int x=-1; x<=1; x++){\n"
    "        for(int y=-1; y<=1; y++){\n"
    "            vec2 curcell = cell + vec2(x,y);\n"
    "            if(curcell == closestcell) continue;\n"
    "            vec2 cellpos = curcell + rand2_voronoi(cell + vec2(x,y));\n"
    "            vec2 edgecenter = mix(closestcellpos, cellpos, 0.5);\n"
    "            vec2 celldir = normalize(cellpos - closestcellpos);\n"
    "            float d = dot(edgecenter - pos, celldir);\n"
    "            minEdgeDist = min(minEdgeDist, d);\n"
    "        }}\n"
    "        \n"
    "        ret += vec3(minCellDist, rand_voronoi(closestcell), minEdgeDist) * per;\n"
    "        w += per;\n"
    "        f *= 2.0;\n"
    "        per *= persist;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret / max(0.1, w);\n"
    "    return vec4(vec3(ret[int(octmode.y)]),1.0);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void Mandelbrot::sample(TextureOperationResult& result, MandelbrotData* attributes, InputInfos& inputs)
{
    auto iterations = attributes->iterations;
    auto oftx = attributes->oftx;
    auto ofty = attributes->ofty;
    auto scale = attributes->scale;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    size_t opId = context.getNextVa();
    size_t in1 = frame.pushInput({(float)iterations, oftx, ofty, scale});
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = mandelbrot($2, $3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(in1), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions((qb::ImageOperationType)MandelbrotData::TypeId, getOperationCode());
    frame.hasUv = true;
}
//--------------------------------------------------------------
std::string Mandelbrot::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 mandelbrot(vec4 res, vec2 uv){\n"
    "    int iterations = int(res.x);\n"
    "    vec2 oft = vec2(res.y,res.z);\n"
    "    vec2 scale = vec2(res.w);\n"
    "    uv = ((uv-0.5) * scale+0.5) -oft;\n"
    "    vec2 z = vec2(0.0);\n"
    "    vec2 dz = vec2(0.0);\n"
    "    float di = 1.0;\n"
    "    float m2 = 0.0;\n"
    "    for(int k=0; k<iterations; ++k){\n"
    "        if (m2>1024.0) {di=0.0; break;}\n"
    "        dz = 2.0*vec2(z.x*dz.x-z.y*dz.y, z.x*dz.y + z.y*dz.x) + vec2(1.0,0.0);\n"
    "        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + uv;\n"
    "        m2 = dot(z,z);\n"
    "    };\n"
    "    float d = 0.5*sqrt(dot(z,z)/dot(dz,dz))*log(dot(z,z));\n"
    "    return vec4(vec3(d),1.0);\n"
    "}\n";
    return std::string(code);
}


TexturePreview::TexturePreview(int resolution)
    : resolution(resolution)
{
    glTextureId = RenderInterface::createTarget(resolution,resolution,false);
    RenderInterface::setTarget((unsigned)glTextureId);
    RenderInterface::clear(0x000000FF);
    // glProgram = RenderInterface::createCustomProgram();
}

TexturePreview::~TexturePreview()
{
    RenderInterface::deleteTarget((unsigned)glTextureId);
    // RenderInterface::destroyCustomProgram((unsigned)glProgram);
}

void TexturePreview::attributePreview()
{
    float avail = ImGui::GetContentRegionAvail().x * 0.5f;

    ImVec2 cursor = ImGui::GetCursorPos();

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, cursor.y));
    ImGui::Image(RenderInterface::getTargetResource((unsigned)glTextureId), ImVec2(avail, avail), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        previewClicked = true;
    }

    if(ImGui::CollapsingHeader("glsl"))
    {
        ImGui::Text(glslCode.c_str());
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            ImGui::OpenPopup("Bigger glsl");

        if(ImGui::BeginPopup("Bigger glsl"))
        {
            ImGui::Text("Copyable glsl code");
            ImGui::InputTextMultiline("##glsl-code", glslCode.data(), glslCode.size(), ImVec2(512, 512), ImGuiInputTextFlags_ReadOnly);
            ImGui::EndPopup();
        }
    }
}

void TexturePreview::nodePreview(const Rect& previewArea)
{
    Rect src = Rect::fromPosAndSize(vec2(0.0f,0.0f), vec2((float)resolution,(float)resolution));
    RenderInterface::setColor(0xFFFFFFFF);
    RenderInterface::copy((unsigned)glTextureId, src, previewArea, false, true);
}