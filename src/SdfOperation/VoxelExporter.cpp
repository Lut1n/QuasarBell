#include "SdfOperation/VoxelExporter.hpp"

#include <fstream>

//--------------------------------------------------------------
namespace qb
{
    template<typename T>
    void write(std::ofstream& ostrm, T data)
    {
        ostrm.write(reinterpret_cast<char*>(&data), sizeof(T));
    }
}

//--------------------------------------------------------------
void qb::VoxelData::resize(size_t x, size_t y, size_t z)
{
    size[0] = x;
    size[1] = y;
    size[2] = z;
    data.resize(x*y*z);
}

//--------------------------------------------------------------
size_t qb::VoxelData::idx(size_t x, size_t y, size_t z) const
{
    x = qb::clamp((int)x, 0, (int)size[0]-1);
    y = qb::clamp((int)y, 0, (int)size[1]-1);
    z = qb::clamp((int)z, 0, (int)size[2]-1);
    size_t ystep = size[0];
    size_t zstep = ystep * size[1];

    return z * zstep + y * ystep + x;
}

//--------------------------------------------------------------
void qb::VoxelData::set(size_t x, size_t y, size_t z, u32 value)
{
    data[idx(x,y,z)] = value;
}

//--------------------------------------------------------------
u32 qb::VoxelData::get(size_t x, size_t y, size_t z) const
{
    return data[idx(x,y,z)];
}

//--------------------------------------------------------------
void qb::VoxelData::forEach(std::function<void(size_t x, size_t y)> fun) const
{
    for(size_t x=0; x<size[0]; ++x)
        for(size_t y=0; y<size[1]; ++y)
            fun(x,y);
}

//--------------------------------------------------------------
void qb::VoxelData::forEach(std::function<void(size_t x, size_t y, size_t z)> fun) const
{
    for(size_t x=0; x<size[0]; ++x)
        for(size_t y=0; y<size[1]; ++y)
            for(size_t z=0; z<size[2]; ++z)
                fun(x,y,z);
}

//--------------------------------------------------------------
unsigned char getPaletteIndex(unsigned expectedColor)
{
    static constexpr unsigned int default_palette[256] = {
        0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
        0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
        0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
        0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
        0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
        0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
        0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
        0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
        0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
        0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
        0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
        0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
        0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
        0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
        0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
        0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
    };

    vec4 as3f = qb::colorTo4f(expectedColor);

    unsigned char ret = 0;
    float dist = std::numeric_limits<float>::max();
    for(size_t i =1; i<256; ++i)
    {
        vec4 pal = qb::colorTo4f(default_palette[i]);
        vec4 diff(as3f.x - pal.y, as3f.y - pal.z, as3f.z - pal.w, 0.0f);
        float d = diff.length();
        if (d < dist)
        {
            ret = i;
            dist = d;
        }
    }
    return ret;
}

//--------------------------------------------------------------
void qb::VoxelData::convetColorToIndex()
{
    for(size_t i=0; i<size[2]; ++i)
        convetColorToIndex(i);
    indexed = true;
}

//--------------------------------------------------------------
void qb::VoxelData::convetColorToIndex(size_t z)
{
    if (z >= size[2])
        return;

    forEach([&](size_t x, size_t y){
        unsigned color = get(x,y,z);
        unsigned index = 0;
        if (color > 0)
            index = getPaletteIndex(color);
        set(x,y,z,index);
    });

    if (z == size[2]-1)
        indexed = true;
}

//--------------------------------------------------------------
struct ExportationWork : public qb::ProgressiveWork::Work
{
    std::ofstream ostrm;
    const qb::VoxelData* data = nullptr;
    bool initialized = false;
    int z = 0;
    int n = 0;

    ExportationWork(const std::string& filepath, const qb::VoxelData* data)
        : ostrm(filepath,std::ios::binary)
        , data(data)
    {}
};

//--------------------------------------------------------------
struct OptimWork : public qb::ProgressiveWork::Work
{
    qb::VoxelData result;
    qb::Range<int> rangeX;
    qb::Range<int> rangeY;
    qb::Range<int> rangeZ;

    int zScan = 0;
    int zResize = 0;
    bool resizing = false;

    OptimWork()
    {
        result = qb::VoxelData();
        rangeX.reset();
        rangeY.reset();
        rangeZ.reset();
        
        zScan = 0;
        zResize = 0;
        resizing = false;
    }
};

//--------------------------------------------------------------
struct IndexingWork : public qb::ProgressiveWork::Work
{
    int z = 0;
};

//--------------------------------------------------------------
void writeHeader(ExportationWork& state)
{
    auto& ostrm = state.ostrm;

    char id[4] = {'V','O','X',' '};
    ostrm.write(id, 4);
    int version = 150;
    qb::write(ostrm, version);
}

//--------------------------------------------------------------
void writeMAINChunk(ExportationWork& state)
{
    auto& ostrm = state.ostrm;

    char id[4] = {'M','A','I','N'};
    ostrm.write(id, 4);
    int chunkContent = 0;
    qb::write(ostrm, chunkContent);
    int childrenChunks = sizeof(int) * 3 + sizeof(int) + state.n * sizeof(unsigned char) * 4;
    childrenChunks += sizeof(char) * 8; // ids
    qb::write(ostrm, childrenChunks);
}

//--------------------------------------------------------------
void writeSIZEChunk(ExportationWork& state)
{
    auto& ostrm = state.ostrm;
    auto& voxelData = *state.data;

    char id[4] = {'S','I','Z','E'};
    ostrm.write(id, 4);
    int chunkContent = sizeof(int) * 3;
    qb::write(ostrm, chunkContent);
    int childrenChunks = 0;
    qb::write(ostrm, childrenChunks);
    
    int w = voxelData.size[0];
    int h = voxelData.size[1];
    int p = voxelData.size[2];
    qb::write(ostrm, w);
    qb::write(ostrm, h);
    qb::write(ostrm, p);
}

//--------------------------------------------------------------
void writeXYZIChunk(qb::ProgressiveWork& state, bool progressive = false)
{
    auto& work = state.getWork<ExportationWork>();
    auto& ostrm = work.ostrm;
    auto& voxelData = *work.data;

    if (work.z == 0)
    {
        char id[4] = {'X','Y','Z','I'};
        ostrm.write(id, 4);
        
        int chunkContent = sizeof(int) + work.n * sizeof(unsigned char) * 4;
        qb::write(ostrm, chunkContent);
        int childrenChunks = 0;
        qb::write(ostrm, childrenChunks);
        qb::write(ostrm, work.n);
    }

    auto writeYZI = [&ostrm, &voxelData](size_t z)
    {
        voxelData.forEach([&](size_t x, size_t y){
            unsigned char index = voxelData.get(x,y,z);
            if (index > 0)
            {
                unsigned char cx = x;
                unsigned char cy = y;
                unsigned char cz = z;
                qb::write(ostrm, cx);
                qb::write(ostrm, cy);
                qb::write(ostrm, cz);
                qb::write(ostrm, index);
            }
        });
    };

    if (work.z < voxelData.size[2])
    {
        writeYZI(work.z);
        work.z++;
        state.progress = (float)work.z / voxelData.size[2];
        if (work.z == voxelData.size[2])
        {
            ostrm.close();
            state.completed = true;
        }
    }
}

//--------------------------------------------------------------
void qb::exportVoxel(const std::string& filepath, const VoxelData& voxels, qb::ProgressiveWork& state)
{
    // minimum data in magicavoxel file format
    // last commit ee2216c28a78ebb68691dc6cfa9c4ba429117ea2     (2021/09/18)
    // cf. https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
    
    if (!state.initialized)
    {
        state.initialize<ExportationWork>(filepath, &voxels);
        auto& work = state.getWork<ExportationWork>();

        for(size_t i = 0; i<voxels.data.size(); ++i)
            if (voxels.data[i] > 0) work.n++;
        
        writeHeader(work);              // RIFF style header
        writeMAINChunk(work);           // chunk MAIN
        // No pack
        writeSIZEChunk(work);           // chunk SIZE
    }

    if (!state.completed)
        writeXYZIChunk(state, true);    // chunk XYZI
    
    // No palette
}

//--------------------------------------------------------------
void qb::optimizeVoxels(const VoxelData& voxels, qb::ProgressiveWork& state)
{
    if (!state.initialized)
        state.initialize<OptimWork>();

    if (state.completed)
        return;

    auto& work = state.getWork<OptimWork>();

    if (work.resizing)
    {
        if (work.zResize == 0)
        {
            work.result.resize(work.rangeX.size(), work.rangeY.size(), work.rangeZ.size());
            work.result.indexed = voxels.indexed;
        }

        work.result.forEach([&](size_t x, size_t y){
            size_t x2 = x + work.rangeX.min;
            size_t y2 = y + work.rangeY.min;
            size_t z2 = work.zResize + work.rangeZ.min;

            work.result.set(x,y,work.zResize, voxels.get(x2,y2,z2));
        });
        work.zResize++;
        state.completed = work.zResize == work.result.size[2];
    }
    else
    {
        voxels.forEach([&](size_t x, size_t y){
            if (voxels.get(x,y,work.zScan) > 0)
            {
                work.rangeX.expends(x);
                work.rangeY.expends(y);
                work.rangeZ.expends(work.zScan);
            }
        });
        work.zScan++;
        work.resizing = work.zScan == voxels.size[2];
    }

    float ref = work.result.size[2] == 0 ? voxels.size[2] : work.result.size[2];
    state.progress = ((float)work.zScan/voxels.size[2] + (float)work.zResize/qb::max(ref, 1.0f)) / 2.0f;
}

//--------------------------------------------------------------
qb::VoxelData& qb::getOptimizedVoxel(qb::ProgressiveWork& state)
{
    auto& work = state.getWork<OptimWork>();
    return work.result;
}

//--------------------------------------------------------------
void qb::colorIndexing(VoxelData& voxels, qb::ProgressiveWork& state)
{
    if (!state.initialized)
        state.initialize<IndexingWork>();

    auto& work = state.getWork<IndexingWork>();
    if (work.z < voxels.size[2])
    {
        voxels.convetColorToIndex(work.z);
        work.z++;
        state.progress = (float)work.z / voxels.size[2];
    }
    else
    {
        state.completed = true;
    }
}