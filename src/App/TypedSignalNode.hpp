#ifndef GUI_ADD_SIGNAL_NODE_H
#define GUI_ADD_SIGNAL_NODE_H

#include "App/SignalNode.hpp"
#include "SignalOperation/Operations.hpp"
#include "SignalOperation/EnvelopOperation.hpp"
#include "SignalOperation/FilterOperation.hpp"
#include "SignalOperation/MixOperation.hpp"
#include "SignalOperation/Oscillator.hpp"
#include "SignalOperation/PitchSelector.hpp"
#include "SignalOperation/KeySampler.hpp"
#include "SignalOperation/Quantizer.hpp"
#include "SignalOperation/Harmonics.hpp"
#include "SignalOperation/StepOperation.hpp"
#include "SignalOperation/Waveform.hpp"

MAKE_SIGNAL_NODE(AddOperation, qb::OperationType_Add)
MAKE_SIGNAL_NODE(SubOperation, qb::OperationType_Sub)
MAKE_SIGNAL_NODE(MultOperation, qb::OperationType_Mult)
MAKE_SIGNAL_NODE(DivOperation, qb::OperationType_Div)
MAKE_SIGNAL_NODE(ClampOperation, qb::OperationType_Clamp)
MAKE_SIGNAL_NODE(AbsOperation, qb::OperationType_Abs)
MAKE_SIGNAL_NODE(TimeScale, qb::OperationType_TimeScale)
MAKE_SIGNAL_NODE(Repeater, qb::OperationType_Repeater)
MAKE_SIGNAL_NODE(CubicSampler, qb::OperationType_CubicSampler)
MAKE_SIGNAL_NODE(EnvelopOperation, qb::OperationType_Envelop)
MAKE_SIGNAL_NODE(FreqFilter, qb::OperationType_Filter)
MAKE_SIGNAL_NODE(FloatInput, qb::OperationType_Float)
MAKE_SIGNAL_NODE(NoiseInput, qb::OperationType_Noise)
MAKE_SIGNAL_NODE(MixOperation, qb::OperationType_Mix)
MAKE_SIGNAL_NODE(Oscillator, qb::OperationType_Oscillator)
MAKE_SIGNAL_NODE(PitchSelector, qb::OperationType_Pitch)
MAKE_SIGNAL_NODE(KeySampler, qb::OperationType_KeySampler)
MAKE_SIGNAL_NODE(PolynomialSampler, qb::OperationType_Polynomial)
MAKE_SIGNAL_NODE(Quantizer, qb::OperationType_Quantizer)
MAKE_SIGNAL_NODE(Harmonics, qb::OperationType_Harmonics)
MAKE_SIGNAL_NODE(StepOperation, qb::OperationType_Step)
MAKE_SIGNAL_NODE(Waveform, qb::OperationType_Waveform)
MAKE_SIGNAL_NODE(OutputOperation, qb::OperationType_AudioOutput)


#endif // GUI_ADD_SIGNAL_NODE_H
