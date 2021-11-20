#ifndef QUASAR_BELL_SDF_VOXEL_HPP
#define QUASAR_BELL_SDF_VOXEL_HPP

#include "SdfOperation/VoxelExporter.hpp"
#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct VoxelOutput : public SdfOperation
{
    VoxelOutput();
    ~VoxelOutput();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    void uiProperties() override;

    std::string pushSlice(size_t index, qb::GlslBuilderVisitor& visitor, float targetZ);

    void resetWorks();
    void scanZ();
    
    float zPlan = 0.0f;
    float voxelSize = 0.1f;

    qb::ProgressiveWork scanningWork;
    qb::ProgressiveWork indexingWork;
    qb::ProgressiveWork exportationWork;
    qb::ProgressiveWork optimizationWork;
};

#endif // QUASAR_BELL_SDF_VOXEL_HPP