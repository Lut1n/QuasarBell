#include "signal/WavExporter.hpp"

#include <cstring>
#include <vector>
#include <fstream>
#include <filesystem>

#include  "signal/Signal.hpp"

//--------------------------------------------------------------
using Output = std::ofstream;
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned;
using u64 = unsigned long long;
using pcm = std::vector<short>;

//--------------------------------------------------------------
template<typename Type>
void owrite(Output& output, Type& v)
{
    output.write(reinterpret_cast<char*>(&v), sizeof(Type));
}

//--------------------------------------------------------------
struct ck_data_t
{
    virtual const char* id() = 0;
    virtual u32 size() = 0;
    virtual void write(Output& output) = 0;
};

//--------------------------------------------------------------
struct riff_ck_t
{
    char ckid[4] = {'\0','\0','\0','\0'};
    u32 cksz = 0;
    ck_data_t* data = NULL;
    
    riff_ck_t(ck_data_t* content)
    {
        std::memcpy(ckid, content->id(), sizeof(ckid));
        data = content;
    }
    
    u32 size()
    {
        if(data) cksz = data->size();
        else cksz = 0;
        
        return cksz + sizeof(cksz) + sizeof(ckid);
    }
    
    void write(Output& output)
    {
        output.write(ckid, sizeof ckid);
        owrite(output, cksz);
        if(data) data->write(output);
    }
};

//--------------------------------------------------------------
struct riff_file_t
{
    char fileid[4] =  {'R','I','F','F'};
    u32 filesz = sizeof(formType);
    char formType[4] = {'\0','\0','\0','\0'};
    std::vector< riff_ck_t* > cklist;
    
    void setFormat(const char* format)
    {
        std::memcpy(formType, format, sizeof(formType));
    }
    
    void addCk(riff_ck_t* ck)
    {
        cklist.push_back(ck);
    }
    
    u32 size()
    {
        filesz = sizeof(formType);
        for(auto ck : cklist)
        {
            if(ck) filesz += ck->size();
        }
        
        return filesz + sizeof(filesz) + sizeof(fileid);
    }
    
    void write(Output& output)
    {
        u32 s = size(); // init recursively all size member values
        std::cout << "writing riff file (size: " << s << ")" << std::endl;
        
        output.write(fileid, sizeof(fileid));
        owrite(output, filesz);
        output.write(formType, sizeof(formType));
        for(auto ck : cklist)
        {
            if(ck) ck->write(output);
        }
    }
};

#define DECLARE_RIFF_CK_ID(ckid) const char* id() override {return ckid ;}
#define DECLARE_RIFF_CK_SZ(sz) u32 size() override {return sz ;}

//--------------------------------------------------------------
struct fmt_ck_t : public ck_data_t
{
    DECLARE_RIFF_CK_ID("fmt ")
    DECLARE_RIFF_CK_SZ(16)
    
    u32 freq = 44100;
    u16 chanel = 1;
    u16 bitsPerSample = 16;

    void setup(u32 frequency = 44100, u16 sampleSize = 16)
    {
        freq = frequency;
        bitsPerSample = sampleSize;
    }

    virtual void write(Output& output)
    {
        u16 audio_fmt = 1; // PCM
        u16 bytePerBloc = chanel * bitsPerSample/8;
        u32 bytePerSec = freq * bytePerBloc;
        
        owrite(output, audio_fmt);
        owrite(output, chanel);
        owrite(output, freq);
        owrite(output, bytePerSec);
        owrite(output, bytePerBloc);
        owrite(output, bitsPerSample);
    }
};

//--------------------------------------------------------------
struct wave_data_t : public ck_data_t
{
    DECLARE_RIFF_CK_ID("data")
    
    const PcmDataBase* wave_data;
    
    wave_data_t(const PcmDataBase& wav)
    {
        wave_data = &wav;
    }
    
    virtual u32 size()
    {
        return wave_data->size();
    }
    
    virtual void write(Output& output)
    {
        output.write(reinterpret_cast<const char*>(wave_data->data()), wave_data->size());
    }
};
//--------------------------------------------------------------
bool WavExporter::exportAsWAV(const std::string& filename, const PcmDataBase& pcm)
{
    fmt_ck_t* fmt = new fmt_ck_t();
    wave_data_t* data = new wave_data_t(pcm);

    unsigned bitsPerSample = 0;
    if (pcm.format == AudioSettings::Format_Mono8 || pcm.format == AudioSettings::Format_Stereo8)
        bitsPerSample = 8;
    if (pcm.format == AudioSettings::Format_Mono16 || pcm.format == AudioSettings::Format_Stereo16)
        bitsPerSample = 16;

    fmt->setup((unsigned)pcm.sampleRate, bitsPerSample);

    riff_ck_t fmt_ck(fmt);
    riff_ck_t data_ck(data);

    riff_file_t riff;
    riff.setFormat("WAVE");
    riff.addCk(&fmt_ck);
    riff.addCk(&data_ck);
    
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    Output out(filename, std::ios::binary);
    riff.write(out);
    return true;
}


