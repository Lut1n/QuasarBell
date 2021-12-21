#include "ImageOperation/ImageAttributes.hpp"

#include "Core/Math.hpp"
#include "App/UserFileInput.hpp"

#include "imgui.h"

static UserFileInput tgaExportIo("tga path", ".tga", "./exported/output.tga");

//--------------------------------------------------------------
ColorData::ColorData()
    : BaseAttributes(TypeId)
{
    add("r", Type::Float, &r);
    add("g", Type::Float, &g);
    add("b", Type::Float, &b);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
Construct3fData::Construct3fData()
    : BaseAttributes(TypeId)
{
    add("c1", Type::Float, &c1);
    add("c2", Type::Float, &c2);
    add("c3", Type::Float, &c3);
    addInput("in1", IoType::ImageGeometry);
    addInput("in2", IoType::ImageGeometry);
    addInput("in3", IoType::ImageGeometry);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
Split3fData::Split3fData()
    : BaseAttributes(TypeId)
{
    add("r", Type::Float, &r);
    add("g", Type::Float, &g);
    add("b", Type::Float, &b);
    add("index", &index, 0, 2);
    addInput("in", IoType::Image);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
DirectionalSignalData::DirectionalSignalData()
    : BaseAttributes(TypeId)
{
    add("direction x", &directionX, -1.0, 1.0);
    add("direction y", &directionY, -1.0, 1.0);
    add("frequency", &frequency, 1.0, 10.0);
    add("amplitude", &amplitude, 0.0, 1.0);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
RadialSignalData::RadialSignalData()
    : BaseAttributes(TypeId)
{
    add("center x", &centerX, 0.0, 1.0);
    add("center y", &centerY, 0.0, 1.0);
    add("frequency", &frequency, 1.0, 10.0);
    add("amplitude", &amplitude, 0.0, 1.0);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
DynamicsData::DynamicsData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::ImageGeometry);
    addInput("brightness", IoType::Image);
    addInput("contrast", IoType::Image);
    addOutput("out", IoType::Image);
    add("brightness", &brightness, -1.0f, 1.0f);
    add("contrast", &contrast, 0.0f, 10.0f);
}
//--------------------------------------------------------------
ImageOutput::ImageOutput()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::ImageGeometry);
    add("resolution", Type::Int, &resolution);
}
//--------------------------------------------------------------
TimeData::TimeData()
    : BaseAttributes(TypeId)
{
    addOutput("time", IoType::Image);
}
//--------------------------------------------------------------
UvInputData::UvInputData()
    : BaseAttributes(TypeId)
{
    addOutput("uv", IoType::Image);
}
//--------------------------------------------------------------
SphericalCoordData::SphericalCoordData()
    : BaseAttributes(TypeId)
{
    addOutput("uv", IoType::Image);
}
//--------------------------------------------------------------
UvDistortionData::UvDistortionData()
    : BaseAttributes(TypeId)
{
    addInput("uv", IoType::Image);
    addInput("normals", IoType::Image);
    addOutput("uv", IoType::Image);
    add("force", &force, 0.0f, 1.0f);
}
//--------------------------------------------------------------
UvMappingData::UvMappingData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Image);
    addInput("uv", IoType::Image);
    addOutput("out", IoType::Image);
    add("u offset", &uOft, -1.0f, 1.0f);
    add("v offset", &vOft, -1.0f, 1.0f);
    add("u factor", &uFct, 0.0001f, 10.0f);
    add("v factor", &vFct, 0.0001f, 10.0f);
}
//--------------------------------------------------------------
BlurFilterData::BlurFilterData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Image);
    addOutput("out", IoType::Image);
    add("radius", &radius, 1, 10);
}
//--------------------------------------------------------------
SharpenFilterData::SharpenFilterData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Image);
    addOutput("out", IoType::Image);
    add("radius", &radius, 1, 10);
}
//--------------------------------------------------------------
MorphoFilterData::MorphoFilterData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Image);
    addInput("radius", IoType::Image);
    addOutput("out", IoType::Image);
    add("radius", &radius, 1, 10);
    add("mode", &mode, 0, 1);
}
//--------------------------------------------------------------
BumpToNormalData::BumpToNormalData()
    : BaseAttributes(TypeId)
{
    addInput("bump", IoType::Image);
    addOutput("normal", IoType::Image);
}
//--------------------------------------------------------------
WhiteNoiseData::WhiteNoiseData()
    : BaseAttributes(TypeId)
{
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
ValueNoiseData::ValueNoiseData()
    : BaseAttributes(TypeId)
{
    add("octaves", &octaves, 1, 10);
    add("frequency", &frequency, 1.0f, 32.0f);
    add("persistance", &persistance, 0.0f, 1.0f);
    add("smoothness", &smoothness, 0.0f, 1.0f);
    addInput("freq", IoType::Image);
    addInput("pers", IoType::Image);
    addInput("smth", IoType::Image);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
GradientNoiseData::GradientNoiseData()
    : BaseAttributes(TypeId)
{
    add("octaves", &octaves, 1, 10);
    add("frequency", &frequency, 1.0f, 32.0f);
    add("persistance", &persistance, 0.0f, 1.0f);
    add("smoothness", &smoothness, 0.0f, 1.0f);
    addInput("freq", IoType::Image);
    addInput("pers", IoType::Image);
    addInput("smth", IoType::Image);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
SimplexNoiseData::SimplexNoiseData()
    : BaseAttributes(TypeId)
{
    add("octaves", &octaves, 1, 10);
    add("frequency", &frequency, 1.0f, 32.0f);
    add("persistance", &persistance, 0.0f, 1.0f);
    add("smoothness", &smoothness, 0.0f, 1.0f);
    addInput("freq", IoType::Image);
    addInput("pers", IoType::Image);
    addInput("smth", IoType::Image);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
VoronoiNoiseData::VoronoiNoiseData()
    : BaseAttributes(TypeId)
{
    add("octaves", &octaves, 1, 10);
    add("frequency", &frequency, 1.0f, 32.0f);
    add("persistance", &persistance, 0.0f, 1.0f);
    add("mode", &mode, 0, 2);
    addInput("freq", IoType::Image);
    addInput("pers", IoType::Image);
    addOutput("out", IoType::Image);
}
//--------------------------------------------------------------
MandelbrotData::MandelbrotData()
    : BaseAttributes(TypeId)
{
    addOutput("out", IoType::Image);
    add("iterations", &iterations, 1, 300);
    add("oftx", &oftx, -2.0f, 2.0f);
    add("ofty", &ofty, -2.0f, 2.0f);
    add("scale", &scale, 0.01f, 10.0f);
}



//--------------------------------------------------------------
void ColorData::ui()
{
    vec4 color = {r,g,b,a};
    if (ImGui::ColorEdit4("color", (float*)&color))
    {
        r = color.x; g = color.y; b = color.z; a = color.w;
        onChanged();
    }
}
//--------------------------------------------------------------
void Split3fData::ui()
{
    ImGui::BeginChild("##properties");
    vec4 color = {r,g,b,0.0f};
    if (ImGui::ColorEdit3("color", (float*)&color))
    {
        r = color.x; g = color.y; b = color.z;
        onChanged();
    }
    BaseAttributes::ui(3);
    ImGui::EndChild();
}
//--------------------------------------------------------------
void ImageOutput::ui()
{
    if (ImGui::SliderInt("power", &resolution, 0, 12))
    {
        textureResolution = (int)std::pow(2.0,(float)resolution);
        onChanged();
    }
    ImGui::InputInt("resolution", &textureResolution);

    if (ImGui::Button("..."))
        tgaExportIo.open();
    ImGui::SameLine();
    ImGui::Text(path.c_str());

    tgaExportIo.display();
    if (tgaExportIo.confirmed)
        path = tgaExportIo.filepath;
    if (ImGui::Button("Export"))
    {
        toExport = true;
        onChanged();
    }
}
