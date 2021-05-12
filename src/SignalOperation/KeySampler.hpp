#ifndef QUASAR_BELL_KEY_SAMPLER_HPP
#define QUASAR_BELL_KEY_SAMPLER_HPP

#include "SignalOperation/SignalOperation.hpp"

//--------------------------------------------------------------
struct KeySampler : public SignalOperation
{
    enum Interpo
    {
        Flat,
        Linear,
        Cubic
    };

    KeySampler();
    OperationData sample(size_t index, const Time& t) override;

    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    void uiProperties() override;

    float interpolate(float x);

    int count = 1;
    int interpo = 0;
    std::vector<std::pair<float,float>> keys;
};


#endif // QUASAR_BELL_KEY_SAMPLER_HPP