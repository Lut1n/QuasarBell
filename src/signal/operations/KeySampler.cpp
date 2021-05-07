#include "signal/operations/KeySampler.hpp"

#include <iostream>

#include "json/Json.hpp"

//--------------------------------------------------------------
KeySampler::KeySampler()
{
    _hasCustomData = true;
    initialize({},{DataType_Float});
    makeProperty({"count", DataType_Int, &count});
    makeProperty({"interpo", DataType_Int, &interpo});
    keys.resize(1,{0.f,1.f});
}
//--------------------------------------------------------------
void KeySampler::validate()
{
}
//--------------------------------------------------------------
OperationData KeySampler::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);

    data.type = output->type;
    data.count = output->count;

    int index1 = -1;
    int index2 = -1;
    for(size_t i=0; i<keys.size(); ++i)
    {
        int j = (int)(keys.size()-1-i);
        if(keys[i].first <= t.t) index1 = (int)i;
        if(keys[j].first >= t.t) index2 = (int)j;
    }

    if(index1 == -1 && index2 == -1)
        data.fvec[0] = 0.0;
    else if(index1 == -1)
        data.fvec[0] = keys[index2].second;
    else if(index2 == -1)
        data.fvec[0] = keys[index1].second;
    else if(index1 == index2)
        data.fvec[0] = keys[index1].second;
    else
    {
        float x = (t.t-keys[index1].first) / (keys[index2].first-keys[index1].first);
        data.fvec[0] = keys[index1].second + interpolate(x) * (keys[index2].second-keys[index1].second);
    }
    
    return data;
}
//--------------------------------------------------------------
float KeySampler::interpolate(float x)
{
    if (interpo == Interpo::Flat)
        return 0;
    else if (interpo == Interpo::Linear)
        return x;
    else if (interpo == Interpo::Cubic)
        return x * x * (3.f - 2.f * x);
    return x;
}
//--------------------------------------------------------------
void KeySampler::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("key-values");
    int index = 0;
    for (auto kv : keys)
    {
        jArray.setPath(index, 0).set(kv.first);
        jArray.setPath(index, 1).set(kv.second);
        index++;
    }
}
//--------------------------------------------------------------
void KeySampler::loadCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("key-values");
    count = (int)jArray.count();
    keys.resize(jArray.count());
    int index = 0;
    for(auto& jkv : jArray.array.values)
    {
        keys[index] = {(float)jkv.setPath(0).getNumeric(),(float) jkv.setPath(1).getNumeric()};
        index++;
    }
}