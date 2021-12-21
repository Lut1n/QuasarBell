#ifndef GUI_ADD_SIGNAL_NODE_H
#define GUI_ADD_SIGNAL_NODE_H

#include "App/SignalNode.hpp"
// #include "SignalOperation/SignalOperations.hpp"
#include "SignalOperation/SignalAttributes.hpp"
/*#include "SignalOperation/EnvelopOperation.hpp"
#include "SignalOperation/FilterOperation.hpp"
#include "SignalOperation/MixOperation.hpp"
#include "SignalOperation/Oscillator.hpp"
#include "SignalOperation/PitchSelector.hpp"
#include "SignalOperation/KeySampler.hpp"
#include "SignalOperation/Quantizer.hpp"
#include "SignalOperation/Harmonics.hpp"
#include "SignalOperation/StepOperation.hpp"
#include "SignalOperation/Waveform.hpp"*/

MAKE_SIGNAL_NODE(AddData)
MAKE_SIGNAL_NODE(SubData)
MAKE_SIGNAL_NODE(MultData)
MAKE_SIGNAL_NODE(DivData)
MAKE_SIGNAL_NODE(ClampData)
MAKE_SIGNAL_NODE(AbsData)
MAKE_SIGNAL_NODE(TimeScaleData)
MAKE_SIGNAL_NODE(RepeaterData)
MAKE_SIGNAL_NODE(CubicSamplerData)
MAKE_SIGNAL_NODE(EnvelopData)
MAKE_SIGNAL_NODE(FreqFilterData)
MAKE_SIGNAL_NODE(FloatData)
MAKE_SIGNAL_NODE(NoiseData)
MAKE_SIGNAL_NODE(MixData)
MAKE_SIGNAL_NODE(OscillatorData)
MAKE_SIGNAL_NODE(PitchData)
MAKE_SIGNAL_NODE(KeySamplerData)
MAKE_SIGNAL_NODE(PolynomialSamplerData)
MAKE_SIGNAL_NODE(QuantizerData)
MAKE_SIGNAL_NODE(HarmonicsData)
MAKE_SIGNAL_NODE(StepData)
MAKE_SIGNAL_NODE(WaveformData)
MAKE_SIGNAL_NODE(OutputData)


#endif // GUI_ADD_SIGNAL_NODE_H
