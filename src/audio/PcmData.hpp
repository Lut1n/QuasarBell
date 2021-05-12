#ifndef QUASAR_BELL_PCMDATA_HPP
#define QUASAR_BELL_PCMDATA_HPP

#include <vector>

#include "Audio/Signal.hpp"

//--------------------------------------------------------------
// PCM: Pulse Code Modulation
//--------------------------------------------------------------

//--------------------------------------------------------------
class PcmDataBase
{
public:

    PcmDataBase(AudioSettings::Format fmt, AudioSettings::SampleRate rate)
        : format(fmt), sampleRate(rate) {}

    virtual size_t count() const = 0;
    virtual void resize(size_t count) = 0;
    virtual float get(size_t i) const = 0;
    virtual void set(size_t i, float s) = 0;
    virtual size_t size() const = 0;
    virtual const void* data() const = 0;

    void copyTo(PcmDataBase& dst) const;

    AudioSettings::Format format;
    AudioSettings::SampleRate sampleRate;
};

//--------------------------------------------------------------
template<AudioSettings::Format Fmt>
class PcmData : public PcmDataBase
{
public:
    using Type = typename FormatTraits<Fmt>::Type;
    static constexpr float Max = std::numeric_limits<Type>::max();

    PcmData() : PcmDataBase(Fmt, AudioSettings::SampleRate_44kHz) {}

    size_t count() const override
    {
        return samples.size();
    }
    
    void resize(size_t count) override
    {
        samples.resize(count);
    }

    float get(size_t i) const override
    {
        return (float)samples[i] / Max;
    }

    void set(size_t i, float s) override
    {
        samples[i] = (Type)(s * Max);
    }

    size_t size() const override
    {
        return count() * sizeof(Type);
    }

    const void* data() const override
    {
        return (void*) samples.data();
    }

    std::vector<Type> samples;
};

//--------------------------------------------------------------
namespace qb
{
    using Pcm16 = PcmData<AudioSettings::Format_Mono16>;
}

#endif // QUASAR_BELL_PCMDATA_HPP