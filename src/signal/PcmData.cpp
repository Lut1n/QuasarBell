#include "PcmData.hpp"

//--------------------------------------------------------------
void PcmDataBase::copyTo(PcmDataBase& dst) const
{
    dst.sampleRate = sampleRate;
    dst.resize(count());
    for(size_t i=0; i<count(); ++i) dst.set(i, get(i));
}
