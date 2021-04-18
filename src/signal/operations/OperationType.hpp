#ifndef QUASAR_BELL_OPERATION_TYPE_HPP
#define QUASAR_BELL_OPERATION_TYPE_HPP

#include "signal/operations/SignalOperation.hpp"

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
        OperationType_Debug,

        OperationType_Count,

        OperationType_None
    };

    std::string getOperationName(OperationType type);
};


#endif // QUASAR_BELL_OPERATION_TYPE_HPP