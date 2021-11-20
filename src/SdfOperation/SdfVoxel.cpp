#include "SdfOperation/SdfVoxel.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <fstream>
#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"


//--------------------------------------------------------------
struct ScanningWork : public qb::ProgressiveWork::Work
{
    qb::VoxelData result;
    int count = 0;
    int lastRendered = -1;
    int lastDownloaded = -1;
    float nStep = 0.0f;
    float zPlan = -2.0f;
};

//--------------------------------------------------------------
VoxelOutput::VoxelOutput()
    : SdfOperation(qb::SdfOperationType_Voxel)
{
    makeInput("in", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("voxel size", &voxelSize, 0.004, 1.0);

    preview.resolution = 1024;
}

//--------------------------------------------------------------
VoxelOutput::~VoxelOutput()
{
}

//--------------------------------------------------------------
std::string VoxelOutput::pushSlice(size_t index, qb::GlslBuilderVisitor& visitor, float targetZ)
{
    auto* co = getInput(index);

    if (inputHasFlag(index, UiPin::Type_S3d))
    {
        size_t frameId = visitor.pushFrame(qb::GlslFrame::Type::VoxelPlan);
        {
            auto& currFrame = visitor.getCurrentFrame();
            currFrame.targetZ = targetZ;
            currFrame.voxelSize = voxelSize;
        }
        bool ret = co->refs[0].operation->sample(co->refs[0].index, visitor);
        visitor.popFrame();
        
        if (ret)
        {
            return qb::sa(frameId);
        }
    }

    return "";
}
//--------------------------------------------------------------
bool VoxelOutput::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    float targetZ = zPlan;
    if (scanningWork.initialized && !scanningWork.completed)
        targetZ = scanningWork.getWork<ScanningWork>().zPlan;

    auto& frame = visitor.getCurrentFrame();
    frame.type = qb::GlslFrame::Type::Texture;
    frame.hasUv = true;
    auto& context = frame.getContext();
    
    size_t opId = context.getNextVa();

    std::string valueId = pushSlice(0, visitor, targetZ);

    if(valueId.empty())
        return false;

    std::string glsl = "vec4 $1 = texture2D($2, uv0).xyzw;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});

    context.pushVa(opId);
    context.pushCode(glsl);

    if (scanningWork.initialized && !scanningWork.completed)
    {
        auto& work = scanningWork.getWork<ScanningWork>();
        if (work.lastRendered < work.count && work.lastDownloaded == work.lastRendered)
        {
            work.lastRendered++;
            work.zPlan += voxelSize;
        }
    }

    return true;
}

//--------------------------------------------------------------
void VoxelOutput::resetWorks()
{
    exportationWork.reset();
    optimizationWork.reset();
    indexingWork.reset();
    scanningWork.reset();
}

//--------------------------------------------------------------
void VoxelOutput::scanZ()
{
    auto& work = scanningWork.getWork<ScanningWork>();

    unsigned frameId = (unsigned)preview.renderFrame->glResource;
    qb::ImageData slice = RenderInterface::downloadTargetImage(frameId);

    work.result.forEach([&](size_t x, size_t y){
        unsigned s = slice.sample(x * work.nStep, y * work.nStep);
        work.result.set(x,work.lastRendered,(work.count-1)-y, s);
    });
    
    work.lastDownloaded++;
    scanningWork.progress = (float)work.lastDownloaded / work.count;
    if (scanningWork.progress >= 1.0)
        scanningWork.completed = true;
}

//--------------------------------------------------------------
void VoxelOutput::uiProperties()
{
    uiPreview();

    for(size_t i=0; i<getPropertyCount(); ++i) uiProperty((int)i);

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
    
    if (scanningWork.initialized)
    {
        auto& work = scanningWork.getWork<ScanningWork>();
        
        if (exportationWork.completed)
            resetWorks();
        else if (indexingWork.completed)
            qb::exportVoxel("./voxelOutput.vox", qb::getOptimizedVoxel(optimizationWork), exportationWork);
        else if (optimizationWork.completed)
             qb::colorIndexing(qb::getOptimizedVoxel(optimizationWork), indexingWork);
        else if (scanningWork.completed)
            qb::optimizeVoxels(work.result, optimizationWork);     
        else if (work.lastRendered > work.lastDownloaded)
            scanZ();
    }
    else if (preview.renderFrame && ImGui::Button("export to 'voxelOutput.vox'"))
    {
        scanningWork.initialize<ScanningWork>();
        auto& work = scanningWork.getWork<ScanningWork>();
        work.nStep = voxelSize * 0.25f;
        work.count = (size_t)(1.0f / work.nStep);
        work.zPlan = -2.0f + voxelSize * 0.5f;
        work.result.resize(work.count, work.count, work.count);
    }
}
