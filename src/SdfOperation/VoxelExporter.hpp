#ifndef QUASAR_BELL_VOXEL_EXPORTER_HPP
#define QUASAR_BELL_VOXEL_EXPORTER_HPP

#include "Core/Math.hpp"
#include "Core/ProgressiveWork.hpp"

#include <vector>
#include <string>
#include <functional>

using u32 = unsigned int;

namespace qb
{
    struct VoxelData
    {
        std::vector<u32> data;
        size_t size[3] = {0,0,0};
        bool indexed = false;

        void resize(size_t x, size_t y, size_t z);

        size_t idx(size_t x, size_t y, size_t z) const;

        void set(size_t x, size_t y, size_t z, u32 value);

        u32 get(size_t x, size_t y, size_t z) const;
        
        void convetColorToIndex();
        void convetColorToIndex(size_t z);

        void forEach(std::function<void(size_t x, size_t y)> fun) const;
        void forEach(std::function<void(size_t x, size_t y, size_t z)> fun) const;

        void copyTo(VoxelData& dst) const;
    };
    
    void optimizeVoxels(const VoxelData& voxels, qb::ProgressiveWork& state);
    qb::VoxelData& getOptimizedVoxel(qb::ProgressiveWork& state);
    void colorIndexing(VoxelData& voxels, qb::ProgressiveWork& state);
    void exportVoxel(const std::string& filepath, const VoxelData& voxels, qb::ProgressiveWork& state);

    struct ScanningWork : public qb::ProgressiveWork::Work
    {
        VoxelData result;
        int count = 0;
        int lastRendered = -1;
        // int lastDownloaded = -1;
        float nStep = 0.0f;
        float zPlan = -2.0f;
    };
};


#endif // QUASAR_BELL_VOXEL_EXPORTER_HPP