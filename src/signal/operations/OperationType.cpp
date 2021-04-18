#include "signal/operations/OperationType.hpp"

//--------------------------------------------------------------
std::string qb::getOperationName(OperationType type)
{
    switch(type)
    {
        case OperationType_Add:
        return "add";
        case OperationType_CubicSampler:
        return "sampler";
        case OperationType_Debug:
        return "debugger";
        case OperationType_Envelop:
        return "envelop";
        case OperationType_Float:
        return "float";
        case OperationType_Mix:
        return "mix";
        case OperationType_Mult:
        return "mult";
        case OperationType_Oscillator:
        return "oscillator";
        case OperationType_Quantizer:
        return "quantizer";
    }
    return "None";
}