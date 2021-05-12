#ifndef GUI_ADD_SIGNAL_NODE_H
#define GUI_ADD_SIGNAL_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"
#include "signal/operations/EnvelopOperation.hpp"
#include "signal/operations/FilterOperation.hpp"
#include "signal/operations/MixOperation.hpp"
#include "signal/operations/Oscillator.hpp"
#include "signal/operations/PitchSelector.hpp"
#include "signal/operations/KeySampler.hpp"
#include "signal/operations/Quantizer.hpp"
#include "signal/operations/Harmonics.hpp"
#include "signal/operations/StepOperation.hpp"
#include "signal/operations/Waveform.hpp"

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
