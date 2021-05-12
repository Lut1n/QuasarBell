#ifndef QUASAR_BELL_OPERATION_TYPE_HPP
#define QUASAR_BELL_OPERATION_TYPE_HPP

#include "SignalOperation/SignalOperation.hpp"

//--------------------------------------------------------------
namespace qb
{
    enum OperationType
    {
        OperationType_Add = 0,
        OperationType_Mult,
        OperationType_Float,
        OperationType_CubicSampler,
        OperationType_Oscillator,
        OperationType_Quantizer,
        OperationType_Mix,
        OperationType_Envelop,
        OperationType_AudioOutput,
        OperationType_Step,
        OperationType_Filter,
        OperationType_Pitch,
        OperationType_Sub,
        OperationType_Div,
        OperationType_Clamp,
        OperationType_Abs,
        OperationType_Waveform,
        OperationType_KeySampler,
        OperationType_Harmonics,
        OperationType_Polynomial,
        OperationType_TimeScale,
        OperationType_Noise,
        OperationType_Repeater,

        OperationType_Count,

        OperationType_None
    };

    std::string getOperationName(OperationType type);
    OperationType getOperationType(const std::string& name);
    std::vector<std::string> getOperationNames();
};


#endif // QUASAR_BELL_OPERATION_TYPE_HPP