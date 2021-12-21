#ifndef QUASAR_BELL_SIGNAL_OPERATION_HPP
#define QUASAR_BELL_SIGNAL_OPERATION_HPP

#include "SignalOperation/SignalAttributes.hpp"
#include "App/BaseOperationNode.hpp"
#include "Audio/AudioRenderer.hpp"

#include "imgui.h"

#include <vector>

/*template<typename PropType>
struct OperationMap
{
    using Get = void;
};

#define MAP_OPERATION(P,O) \
template<>struct OperationMap<P>{using Get = O;};\*/


struct Time
{
    float advance = 0.0f;
    float elapsed = 0.0f;
    float duration = 1.0f;
    float timestep = 0.0f;
    float samplerate = 0.0f;
};

struct AbstractSignalOperation;

struct OperationInfo
{
    BaseAttributes* attributes;
    AbstractSignalOperation* operation;
    std::vector<OperationInfo*> inputs;
};

struct AbstractSignalOperation
{
    virtual float sample(const Time& time, BaseAttributes* attributes, std::vector<OperationInfo*>& inputs) = 0;
};

template<typename PropsType>
struct ConcretSignalOperation : public AbstractSignalOperation
{
    static constexpr size_t TypeId = PropsType::TypeId;

    float sample(const Time& time, BaseAttributes* attributes, std::vector<OperationInfo*>& inputs) override
    {
        return sample(time, static_cast<PropsType*>(attributes),inputs);
    }

    virtual float sample(const Time& time, PropsType* attributes, std::vector<OperationInfo*>& inputs) = 0;
};

struct FloatInput : public ConcretSignalOperation<FloatData>
{
    float sample(const Time& time, FloatData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct NoiseInput : public ConcretSignalOperation<NoiseData>
{
    float sample(const Time& time, NoiseData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct Repeater : public ConcretSignalOperation<RepeaterData>
{
    float sample(const Time& time, RepeaterData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct CubicSampler : public ConcretSignalOperation<CubicSamplerData>
{
    float sample(const Time& time, CubicSamplerData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct PolynomialSampler : public ConcretSignalOperation<PolynomialSamplerData>
{
    float sample(const Time& time, PolynomialSamplerData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct AddOperation : public ConcretSignalOperation<AddData>
{
    float sample(const Time& time, AddData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct SubOperation : public ConcretSignalOperation<SubData>
{
    float sample(const Time& time, SubData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct MultOperation : public ConcretSignalOperation<MultData>
{
    float sample(const Time& time, MultData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct DivOperation : public ConcretSignalOperation<DivData>
{
    float sample(const Time& time, DivData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct ClampOperation : public ConcretSignalOperation<ClampData>
{
    float sample(const Time& time, ClampData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct AbsOperation : public ConcretSignalOperation<AbsData>
{
    float sample(const Time& time, AbsData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct TimeScale : public ConcretSignalOperation<TimeScaleData>
{
    float sample(const Time& time, TimeScaleData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct OutputOperation : public ConcretSignalOperation<OutputData>
{
    float sample(const Time& time, OutputData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct EnvelopOperation : public ConcretSignalOperation<EnvelopData>
{
    float sample(const Time& time, EnvelopData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct FreqFilter : public ConcretSignalOperation<FreqFilterData>
{
    float sample(const Time& time, FreqFilterData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct Harmonics : public ConcretSignalOperation<HarmonicsData>
{
    float sample(const Time& time, HarmonicsData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct KeySampler : public ConcretSignalOperation<KeySamplerData>
{
    enum Interpo
    {
        Flat,
        Linear,
        Cubic
    };

    float sample(const Time& time, KeySamplerData* attributes, std::vector<OperationInfo*>& inputs) override;
    float interpolate(float x, KeySamplerData* props);
};

struct MixOperation : public ConcretSignalOperation<MixData>
{
    float sample(const Time& time, MixData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct Oscillator : public ConcretSignalOperation<OscillatorData>
{
    float sample(const Time& time, OscillatorData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct PitchSelector : public ConcretSignalOperation<PitchData>
{
    float sample(const Time& time, PitchData* attributes, std::vector<OperationInfo*>& inputs) override;

    int getMidiIndex(PitchData* props) const;
    float getFreq(PitchData* props) const;
};

struct Quantizer : public ConcretSignalOperation<QuantizerData>
{
    float sample(const Time& time, QuantizerData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct StepOperation : public ConcretSignalOperation<StepData>
{
    float sample(const Time& time, StepData* attributes, std::vector<OperationInfo*>& inputs) override;
};

struct Waveform : public ConcretSignalOperation<WaveformData>
{
    float sample(const Time& time, WaveformData* attributes, std::vector<OperationInfo*>& inputs) override;

    float sampleClip(float t, WaveformData* attributes);
};


struct PcmPreview : BasePreview
{
    std::vector<float> data;
    float maxV = 0.0f;
    float minV = 0.0f;

    void reset(size_t c)
    {
        minV = 0.0f;
        maxV = 0.0f;
        data.resize(c);
    }

    void set(size_t i, float v)
    {
        maxV = std::max(v,maxV);
        minV = std::min(v,minV);
        if (i < data.size()) data[i] = v;
    }

    size_t count() const
    {
        return data.size();
    }

    void attributePreview() override
    {
        ImGui::PlotLines("##preview", (float*)data.data(), data.size(), 0, NULL, -1.0, 1.0, ImVec2(0, 100.0f));
    }

    void nodePreview(const Rect& previewArea) override
    {
        const int count = (int)data.size();
        RenderInterface::setColor(0x777777FF);
        RenderInterface::fill(previewArea.p0, previewArea.p1);
        vec2 barS(previewArea.size().x / (float)count, 0);
        RenderInterface::setColor(0xEEEEEEFF);

        for(int i=0; i< count; ++i)
        {
            vec2 p = previewArea.p0 + vec2((float)i*barS.x,previewArea.size().y*0.5f);
            float y = data[i] / maxV;
            float h = previewArea.size().y*0.5f * -y;
            RenderInterface::fill(p, p+vec2(barS.x,h));
        }
    }
};

#endif // QUASAR_BELL_SIGNAL_OPERATION_HPP
