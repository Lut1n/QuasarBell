#include "ImageOperation/ImageNoises.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
WhiteNoise::WhiteNoise()
    : ImageOperation(qb::ImageOperationType_WhiteNoise)
{
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool WhiteNoise::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = white_noise($2);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
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
    "    return vec4(r,r,r,1.0f);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
ValueNoise::ValueNoise()
    : ImageOperation(qb::ImageOperationType_ValueNoise)
{
    makeProperty("octaves", &octaves, 1, 10);
    makeProperty("frequency", &frequency, 1.0f, 32.0f);
    makeProperty("persistance", &persistance, 0.0f, 1.0f);
    makeProperty("smoothness", &smoothness, 0.0f, 1.0f);
    makeInput("freq", BaseOperationDataType::Float);
    makeInput("pers", BaseOperationDataType::Float);
    makeInput("smth", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool ValueNoise::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    std::string freqId = pushOpOrInput(0,visitor, {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(1,visitor, {persistance,persistance,persistance,persistance});
    std::string smoothId = pushOpOrInput(2,visitor, {smoothness,smoothness,smoothness,smoothness});
    
    std::string octaveId = qb::in(frame.pushInput({(float)octaves, 0.0, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = perlin($2.x, $3.x, $4.x, $5.x, $6);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveId, freqId, persId, smoothId, qb::uv(uvId)});
    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
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
    "    float w = 0.0f;\n"
    "    float ret = 0.0f;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 oft = vec2(0.0f, 1.0f);\n"
    "        vec2 iuv = floor(uv * f);\n"
    "        vec2 fuv = fract(uv * f);\n"
    "        fuv = smth * fuv * fuv * (3.f - 2.f * fuv);\n"
    "        float s1 = rand_perlin(iuv + oft.xx);\n"
    "        float s2 = rand_perlin(iuv + oft.yx);\n"
    "        float s3 = rand_perlin(iuv + oft.xy);\n"
    "        float s4 = rand_perlin(iuv + oft.yy);\n"
    "        float s12 = mix(s1,s2,fuv.x);\n"
    "        float s34 = mix(s3,s4,fuv.x);\n"
    "        float s1234 = mix(s12,s34,fuv.y);\n"
    "        ret += s1234 * per;\n"
    "        w += per;\n"
    "        f *= 2.0f;\n"
    "        per *= persist;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret / max(0.1f, w);\n"
    "    return vec4(vec3(ret),1.0f);\n"
    "}\n";
    return std::string(code);
}


//--------------------------------------------------------------
GradientNoise::GradientNoise()
    : ImageOperation(qb::ImageOperationType_GradientNoise)
{
    makeProperty("octaves", &octaves, 1, 10);
    makeProperty("frequency", &frequency, 1.0f, 32.0f);
    makeProperty("persistance", &persistance, 0.0f, 1.0f);
    makeProperty("smoothness", &smoothness, 0.0f, 1.0f);
    makeInput("freq", BaseOperationDataType::Float);
    makeInput("pers", BaseOperationDataType::Float);
    makeInput("smth", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool GradientNoise::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    std::string freqId = pushOpOrInput(0,visitor, {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(1,visitor, {persistance,persistance,persistance,persistance});
    std::string smoothId = pushOpOrInput(2,visitor, {smoothness,smoothness,smoothness,smoothness});

    std::string octaveId = qb::in(frame.pushInput({(float)octaves, 0.0, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = perlin_grad($2.x, $3.x, $4.x, $5.x, $6);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveId, freqId, persId, smoothId, qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
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
    "    //float w = 0.0f;\n"
    "    float ret = 0.0f;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 oft = vec2(0.0f, 1.0f);\n"
    "        vec2 iuv = floor(uv * f);\n"
    "        vec2 fra = fract(uv * f);\n"
    "        vec2 fuv = smth * fra * fra * (3.f - 2.f * fra);\n"
    "        float s1 = dot(fra - oft.xx, (rand2_perlin_grad(iuv + oft.xx)));\n"
    "        float s2 = dot(fra - oft.yx, (rand2_perlin_grad(iuv + oft.yx)));\n"
    "        float s3 = dot(fra - oft.xy, (rand2_perlin_grad(iuv + oft.xy)));\n"
    "        float s4 = dot(fra - oft.yy, (rand2_perlin_grad(iuv + oft.yy)));\n"
    "        float s12 = mix(s1,s2,fuv.x);\n"
    "        float s34 = mix(s3,s4,fuv.x);\n"
    "        float s1234 = mix(s12,s34,fuv.y);\n"
    "        ret += s1234 * per;\n"
    "        //w += per * sign(s1234);\n"
    "        f *= 2.0f;\n"
    "        per *= persist;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret*0.5+0.5;"
    "    //ret = ret / max(0.1f, w);\n"
    "    return vec4(vec3(ret),1.0f);\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
SimplexNoise::SimplexNoise()
    : ImageOperation(qb::ImageOperationType_SimplexNoise)
{
    makeProperty("octaves", &octaves, 1, 10);
    makeProperty("frequency", &frequency, 1.0f, 32.0f);
    makeProperty("persistance", &persistance, 0.0f, 1.0f);
    makeProperty("smoothness", &smoothness, 0.0f, 1.0f);
    makeInput("freq", BaseOperationDataType::Float);
    makeInput("pers", BaseOperationDataType::Float);
    makeInput("smth", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool SimplexNoise::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    std::string freqId = pushOpOrInput(0,visitor, {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(1,visitor, {persistance,persistance,persistance,persistance});
    std::string smoothId = pushOpOrInput(2,visitor, {smoothness,smoothness,smoothness,smoothness});

    std::string octaveId = qb::in(frame.pushInput({(float)octaves, 0.0, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = simplex_grad($2.x, $3.x, $4.x, $5.x, $6);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveId, freqId, persId, smoothId, qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
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
    "    float pers = 0.5f;"
    "    float r = 0.0f;\n"
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
    "    return vec4(vec3(r * 0.5 + 0.5),1.0f);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
VoronoiNoise::VoronoiNoise()
    : ImageOperation(qb::ImageOperationType_Voronoi)
{
    makeProperty("octaves", &octaves, 1, 10);
    makeProperty("frequency", &frequency, 1.0f, 32.0f);
    makeProperty("persistance", &persistance, 0.0f, 1.0f);
    makeProperty("mode", &mode, 0, 2);
    makeInput("freq", BaseOperationDataType::Float);
    makeInput("pers", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool VoronoiNoise::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    std::string freqId = pushOpOrInput(0,visitor, {frequency,frequency,frequency,frequency});
    std::string persId = pushOpOrInput(1,visitor, {persistance,persistance,persistance,persistance});

    std::string octaveModeId = qb::in(frame.pushInput({(float)octaves, (float)mode, 0.0, 0.0}));

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = voronoi($2, $3.x, $4.x, $5);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), octaveModeId, freqId, persId, qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
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
    "    float w = 0.0f;\n"
    "    vec3 ret;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 pos = uv * f;"
    "        float minCellDist = 10.0f;\n"
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
    "        f *= 2.0f;\n"
    "        per *= persist;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret / max(0.1f, w);\n"
    "    return vec4(vec3(ret[int(octmode.y)]),1.0f);\n"
    "}\n";
    return std::string(code);
}


//--------------------------------------------------------------
Mandelbrot::Mandelbrot()
    : ImageOperation(qb::ImageOperationType_Mandelbrot)
{
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("iterations", &iterations, 1, 300);
    makeProperty("oftx", &oftx, -2.0f, 2.0f);
    makeProperty("ofty", &ofty, -2.0f, 2.0f);
    makeProperty("scale", &scale, 0.01f, 10.0f);
}
//--------------------------------------------------------------
bool Mandelbrot::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    size_t opId = context.getNextVa();
    size_t in1 = frame.pushInput({(float)iterations, oftx, ofty, scale});
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = mandelbrot($2, $3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(in1), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
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