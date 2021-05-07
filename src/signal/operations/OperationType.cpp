#include "signal/operations/OperationType.hpp"

#include <unordered_map>

//--------------------------------------------------------------
namespace qb
{
    const static std::unordered_map<OperationType, std::string> s_operationNames = 
    {
        {OperationType_Add, "add"},
        {OperationType_CubicSampler, "cubic"},
        {OperationType_AudioOutput, "audio-output"},
        {OperationType_Envelop, "envelop"},
        {OperationType_Float, "float"},
        {OperationType_Mix, "mix"},
        {OperationType_Mult, "mult"},
        {OperationType_Oscillator, "oscillator"},
        {OperationType_Quantizer, "quantizer"},
        {OperationType_Step, "step"},
        {OperationType_Filter, "filter"},
        {OperationType_Pitch, "pitch"},
        {OperationType_Sub, "sub"},
        {OperationType_Div, "div"},
        {OperationType_Clamp, "clamp"},
        {OperationType_Abs, "abs"},
        {OperationType_Waveform, "waveform"},
        {OperationType_KeySampler, "key-sampler"},
        {OperationType_Harmonics, "harmonics"},
        {OperationType_Polynomial, "polynomial"},
        {OperationType_TimeScale, "time-scale"},
        {OperationType_Noise, "noise"},
        {OperationType_Repeater, "repeater"}
    };
}

//--------------------------------------------------------------
std::string qb::getOperationName(OperationType type)
{
    auto it = s_operationNames.find(type);
    if (it != s_operationNames.end())
        return it->second;
    return "None";
}

//--------------------------------------------------------------
qb::OperationType qb::getOperationType(const std::string& name)
{
    if(name == "sampler") return OperationType_CubicSampler;
    if(name == "debugger") return OperationType_AudioOutput;

    for(auto& pair : s_operationNames)
    {
        if (pair.second == name) return pair.first;
    }
    return OperationType_None;
}

//--------------------------------------------------------------
std::vector<std::string> qb::getOperationNames()
{
    std::vector<std::string> ret(OperationType_Count);
    for(size_t i=0; i<(size_t)OperationType_Count; ++i) ret[i] = getOperationName((OperationType)i);
    return ret;
}