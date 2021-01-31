#ifndef QUASAR_BELL_PCMDATA_HPP
#define QUASAR_BELL_PCMDATA_HPP

#include <vector>

//--------------------------------------------------------------
// PCM: Pulse Code Modulation
//--------------------------------------------------------------
class PcmData
{
public:
    void copy(const PcmData& other, size_t oft = 0);
    
    PcmData operator<<(const PcmData& other) const;
    PcmData operator+(const PcmData& other) const;
    PcmData operator*(float aFactor) const;
    
    const void* pcmData() const;
    size_t pcmSize() const;
    
    PcmData subData(size_t oft, size_t size);

public:
    // 16 Mono
    std::vector<short> samples;
};

#endif // QUASAR_BELL_PCMDATA_HPP