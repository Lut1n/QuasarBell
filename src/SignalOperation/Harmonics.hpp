#ifndef QUASAR_BELL_HARMONICS_HPP
#define QUASAR_BELL_HARMONICS_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct Harmonics : public SignalOperation
{
    Harmonics();
    OperationData sample(size_t index, const Time& t) override;

    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    void uiProperties() override;

    int count = 1;
    std::vector<std::pair<float,float>> freqs;
};


#endif // QUASAR_BELL_HARMONICS_HPP