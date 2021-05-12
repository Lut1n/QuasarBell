#include "App/TypedSignalNode.hpp"

MAKE_SIGNAL_NODE_CREATOR(AddOperation, qb::OperationType_Add)
MAKE_SIGNAL_NODE_CREATOR(SubOperation, qb::OperationType_Sub)
MAKE_SIGNAL_NODE_CREATOR(MultOperation, qb::OperationType_Mult)
MAKE_SIGNAL_NODE_CREATOR(DivOperation, qb::OperationType_Div)
MAKE_SIGNAL_NODE_CREATOR(ClampOperation, qb::OperationType_Clamp)
MAKE_SIGNAL_NODE_CREATOR(AbsOperation, qb::OperationType_Abs)
MAKE_SIGNAL_NODE_CREATOR(TimeScale, qb::OperationType_TimeScale)
MAKE_SIGNAL_NODE_CREATOR(Repeater, qb::OperationType_Repeater)
MAKE_SIGNAL_NODE_CREATOR(CubicSampler, qb::OperationType_CubicSampler)
MAKE_SIGNAL_NODE_CREATOR(EnvelopOperation, qb::OperationType_Envelop)
MAKE_SIGNAL_NODE_CREATOR(FreqFilter, qb::OperationType_Filter)
MAKE_SIGNAL_NODE_CREATOR(FloatInput, qb::OperationType_Float)
MAKE_SIGNAL_NODE_CREATOR(NoiseInput, qb::OperationType_Noise)
MAKE_SIGNAL_NODE_CREATOR(MixOperation, qb::OperationType_Mix)
MAKE_SIGNAL_NODE_CREATOR(Oscillator, qb::OperationType_Oscillator)
MAKE_SIGNAL_NODE_CREATOR(PitchSelector, qb::OperationType_Pitch)
MAKE_SIGNAL_NODE_CREATOR(KeySampler, qb::OperationType_KeySampler)
MAKE_SIGNAL_NODE_CREATOR(PolynomialSampler, qb::OperationType_Polynomial)
MAKE_SIGNAL_NODE_CREATOR(Quantizer, qb::OperationType_Quantizer)
MAKE_SIGNAL_NODE_CREATOR(Harmonics, qb::OperationType_Harmonics)
MAKE_SIGNAL_NODE_CREATOR(StepOperation, qb::OperationType_Step)
MAKE_SIGNAL_NODE_CREATOR(Waveform, qb::OperationType_Waveform)
MAKE_SIGNAL_NODE_CREATOR(OutputOperation, qb::OperationType_AudioOutput)