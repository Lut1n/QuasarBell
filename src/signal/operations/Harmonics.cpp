#include "signal/operations/Harmonics.hpp"

#include <cmath> // sin
#include <iostream>

#include "io/json/Json.hpp"

//--------------------------------------------------------------
Harmonics::Harmonics()
{
    _hasCustomData = true;
    initialize({},{DataType_Float});
    makeProperty({"count", DataType_Int, &count});
    freqs.resize(1,{440.0,1.0});
}

//--------------------------------------------------------------
void Harmonics::validate()
{
}

//--------------------------------------------------------------
OperationData Harmonics::sample(size_t index, const Time& t)
{
    auto output  = getOutput(0);

    OperationData data;
    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = 0.0;
    for(auto f : freqs) data.fvec[0] += std::sin( t.t * f.first * 2.0 * 3.141592 ) * f.second;
    return data;
}

//--------------------------------------------------------------
void Harmonics::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("freq-ampl");
    int index = 0;
    for (auto fa : freqs)
    {
        jArray.setPath(index, 0).set(fa.first);
        jArray.setPath(index, 1).set(fa.second);
        index++;
    }
}
//--------------------------------------------------------------
void Harmonics::loadCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("freq-ampl");
    count = jArray.count();
    freqs.resize(count);
    int index = 0;
    for(auto& jfa : jArray.array.values)
    {
        freqs[index] = {jfa.setPath(0).getNumeric(),jfa.setPath(1).getNumeric()};
        index++;
    }
}