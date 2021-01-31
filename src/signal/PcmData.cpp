#include "PcmData.hpp"

#include "gui/Math.hpp"

//--------------------------------------------------------------
void PcmData::copy(const PcmData& other, size_t oft)
{
    if(oft < samples.size())
    {
        unsigned end = qb::min(samples.size() - oft, other.samples.size());
        for(size_t i=0; i<end; ++i) samples[oft+i] = other.samples[i];
    }
}
//--------------------------------------------------------------
PcmData PcmData::operator<<(const PcmData& other) const
{
    PcmData ret;
    ret.samples.resize(samples.size() + other.samples.size());
    ret.copy(*this, 0);
    ret.copy(other, samples.size());
    return ret;
}
//--------------------------------------------------------------
PcmData PcmData::operator+(const PcmData& other) const
{
    PcmData ret;
    ret.samples.resize(qb::max(samples.size(), other.samples.size()));
    for(unsigned i=0; i<ret.samples.size(); ++i)
    {
        ret.samples[i] = samples[i] + other.samples[i];
    }
    return ret;
}
//--------------------------------------------------------------
PcmData PcmData::operator*(float aFactor) const
{
    PcmData ret;
    ret.samples.resize(samples.size());
    for(unsigned i=0; i<ret.samples.size(); ++i)
    {
        ret.samples[i] = samples[i] * aFactor;
    }
    return ret;
}
//--------------------------------------------------------------
const void* PcmData::pcmData() const
{
    return samples.data();
}
//--------------------------------------------------------------
size_t PcmData::pcmSize() const
{
    return samples.size() * sizeof(short);
}
//--------------------------------------------------------------
PcmData PcmData::subData(size_t oft, size_t size)
{
    unsigned effectiveSize = qb::min(samples.size() - oft, size);
    
    PcmData res;
    res.samples.resize(effectiveSize);
    for(unsigned i=0; i<effectiveSize; ++i)
    {
        res.samples[i] = samples[oft + i];
    }
    return res;   
}