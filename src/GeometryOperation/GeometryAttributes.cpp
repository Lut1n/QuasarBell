#include "GeometryOperation/GeometryAttributes.hpp"

#include "Core/Quat.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
PrimitiveData::PrimitiveData(size_t typeId)
    : BaseAttributes(typeId)
{}

//--------------------------------------------------------------
SphereData::SphereData()
    : PrimitiveData(TypeId)
{
    add("radius", &r, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
BoxData::BoxData()
    : PrimitiveData(TypeId)
{
    add("sx", &sx, 0.0f, 2.0f);
    add("sy", &sy, 0.0f, 2.0f);
    add("sz", &sz, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
RoundBoxData::RoundBoxData()
    : PrimitiveData(TypeId)
{
    add("sx", &sx, 0.0f, 2.0f);
    add("sy", &sy, 0.0f, 2.0f);
    add("sz", &sz, 0.0f, 2.0f);
    add("r", &r, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
TorusData::TorusData()
    : PrimitiveData(TypeId)
{
    add("t1", &t1, 0.0f, 2.0f);
    add("t2", &t2, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
HexagonalPrismData::HexagonalPrismData()
    : PrimitiveData(TypeId)
{
    add("h1", &h1, 0.0f, 2.0f);
    add("h2", &h2, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
TriangularPrismData::TriangularPrismData()
    : PrimitiveData(TypeId)
{
    add("t1", &t1, 0.0f, 2.0f);
    add("t2", &t2, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
CapsuleData::CapsuleData()
    : PrimitiveData(TypeId)
{
    add("h", &h, 0.0f, 2.0f);
    add("r", &r, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
ConeData::ConeData()
    : PrimitiveData(TypeId)
{
    add("r", &r, 0.0f, 2.0f);
    add("h", &h, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
RoundedConeData::RoundedConeData()
    : PrimitiveData(TypeId)
{
    add("r1", &r1, 0.0f, 2.0f);
    add("r2", &r2, 0.0f, 2.0f);
    add("h", &h, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
CylinderData::CylinderData()
    : PrimitiveData(TypeId)
{
    add("r", &r, 0.0f, 2.0f);
    add("h", &h, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
RoundedCylinderData::RoundedCylinderData()
    : PrimitiveData(TypeId)
{
    add("r1", &r1, 0.0f, 2.0f);
    add("r2", &r2, 0.0f, 2.0f);
    add("h", &h, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
OctahedronData::OctahedronData()
    : PrimitiveData(TypeId)
{
    add("s", &s, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
PyramidData::PyramidData()
    : PrimitiveData(TypeId)
{
    add("h", &h, 0.0f, 2.0f);
    add("color", Type::Color3, rgb);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
UnionData::UnionData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Geometry);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
SubstractionData::SubstractionData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Geometry);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
IntersectionData::IntersectionData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Geometry);
    addOutput("out", IoType::Geometry);
}
//--------------------------------------------------------------
SmoothUnionData::SmoothUnionData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Geometry);
    addOutput("out", IoType::Geometry);
    add("k",&k, 0.0, 3.0);
}
//--------------------------------------------------------------
SmoothSubstractionData::SmoothSubstractionData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Geometry);
    addOutput("out", IoType::Geometry);
    add("k",&k, 0.0, 3.0);
}
//--------------------------------------------------------------
SmoothIntersectionData::SmoothIntersectionData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Geometry);
    addOutput("out", IoType::Geometry);
    add("k",&k, 0.0, 3.0);
}
//--------------------------------------------------------------
HighResSdfOutputData::HighResSdfOutputData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    add("resolution", Type::Int, &resolution);
}
//--------------------------------------------------------------
void HighResSdfOutputData::ui()
{
    if (ImGui::SliderInt("power", &resolution, 0, 12))
    {
        textureResolution = (int)std::pow(2.0,(float)resolution);
        onChanged();
    }
    int res = textureResolution;
    ImGui::InputInt("resolution", &res);
}
//--------------------------------------------------------------
TransformData::TransformData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addInput("p", IoType::Image);
    addInput("r", IoType::Image);
    addInput("s", IoType::Image);
    addOutput("out", IoType::Geometry);
    add("x",&x, -2.0, 2.0);
    add("y",&y, -2.0, 2.0);
    add("z",&z, -2.0, 2.0);
    add("rx",&rx, -180.0, 180.0);
    add("ry",&ry, -180.0, 180.0);
    add("rz",&rz, -180.0, 180.0);
    add("s",&s, 0.1, 2.0);
}
//--------------------------------------------------------------
RepetitionData::RepetitionData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addOutput("out", IoType::Geometry);
    add("c",&c, 0.0, 2.0);
    add("x",&x, 0, 10);
    add("y",&y, 0, 10);
    add("z",&z, 0, 10);
}
//--------------------------------------------------------------
TwistData::TwistData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addInput("k", IoType::Image);
    addOutput("out", IoType::Geometry);
    add("k",&k, -10.0, 10.0);
}
//--------------------------------------------------------------
BendData::BendData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addInput("k", IoType::Image);
    addOutput("out", IoType::Geometry);
    add("k",&k, -10.0, 10.0);
}
//--------------------------------------------------------------
ElongationData::ElongationData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addInput("xyz", IoType::Image);
    addOutput("out", IoType::Geometry);
    add("x",&x, -2.0, 2.0);
    add("y",&y, -2.0, 2.0);
    add("z",&z, -2.0, 2.0);
}
//--------------------------------------------------------------
SymmetryData::SymmetryData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addOutput("out", IoType::Geometry);
    add("x",&x, 0, 1);
    add("y",&y, 0, 1);
    add("z",&z, 0, 1);
}
//--------------------------------------------------------------
DisplacementData::DisplacementData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Image);
    addOutput("out", IoType::Geometry);
    add("intensity",&intensity, 0.0, 1.0);
    add("mode",&mode, 0, 2);
}
//--------------------------------------------------------------
TexturingData::TexturingData()
    : BaseAttributes(TypeId)
{
    addInput("in1", IoType::Geometry);
    addInput("in2", IoType::Image);
    addOutput("out", IoType::Geometry);
    add("factor",&factor, 0.0, 1.0);
    add("mode",&mode, 0, 2);
}
//--------------------------------------------------------------
VoxelOutputData::VoxelOutputData()
    : BaseAttributes(TypeId)
{
    addInput("in", IoType::Geometry);
    addOutput("out", IoType::Geometry);
    add("voxel size", &voxelSize, 0.004, 1.0);
    add("voxel plan", &zPlan, -2.0, 2.0);
}


//--------------------------------------------------------------
void VoxelOutputData::resetWorks()
{
    exportationWork.reset();
    optimizationWork.reset();
    indexingWork.reset();
    scanningWork.reset();
}

//--------------------------------------------------------------
void VoxelOutputData::ui()
{
    for(size_t i=0; i<count(); ++i) BaseAttributes::ui((int)i);

    auto labelProgressBar = [](const char* label, float progress)
    {
        ImGui::Text(label);
        ImGui::SameLine();
        ImGui::ProgressBar(progress);
    };
    labelProgressBar("    scanning:", scanningWork.progress);
    labelProgressBar("optimization:", optimizationWork.progress);
    labelProgressBar("    indexing:", indexingWork.progress);
    labelProgressBar(" exportation:", exportationWork.progress);

    if (!scanningWork.initialized && ImGui::Button("export to 'voxelOutput.vox'"))
    {
        toExport = true;
    }
}
