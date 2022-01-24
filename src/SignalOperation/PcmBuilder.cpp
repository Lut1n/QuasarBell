#include "SignalOperation/PcmBuilder.hpp"

#include "Audio/WavExporter.hpp"

PcmGenerator::PcmGenerator()
{
    registerSignalOperation<FloatInput>();
    registerSignalOperation<NoiseInput>();
    registerSignalOperation<Repeater>();
    registerSignalOperation<CubicSampler>();
    registerSignalOperation<PolynomialSampler>();
    registerSignalOperation<AddOperation>();
    registerSignalOperation<SubOperation>();
    registerSignalOperation<MultOperation>();
    registerSignalOperation<DivOperation>();
    registerSignalOperation<ClampOperation>();
    registerSignalOperation<AbsOperation>();
    registerSignalOperation<TimeScale>();
    registerSignalOperation<OutputOperation>();
    registerSignalOperation<EnvelopOperation>();
    registerSignalOperation<FreqFilter>();
    registerSignalOperation<Harmonics>();
    registerSignalOperation<KeySampler>();
    registerSignalOperation<MixOperation>();
    registerSignalOperation<PitchSelector>();
    registerSignalOperation<Quantizer>();
    registerSignalOperation<StepOperation>();
    registerSignalOperation<Waveform>();
}

void PcmGenerator::reset()
{
    collector.nodes.clear();
}

void PcmGenerator::prepare(BaseOperationNode* node)
{
    if (auto output = dynamic_cast<OutputData*>(node->getAttributes()))
    {
        float duration = output->duration;
        if (output->duration == 0.0f)
        {
            std::cout << "error: duration = 0; 1s is used" << std::endl;
            duration = 1.0;
        }

        state = State();
        state.sampleRate = (float)output->sampleRate;
        state.duration = duration;
        state.timeStep = 1.f / (float)output->sampleRate;
        state.sampleBits = output->sampleBits;
        state.t = 0.0f;

        state.path = output->path;
        state.toPlay = output->toPlay;
        state.toExport = output->toExport;
        output->toPlay = false;
        output->toExport = false;
    }
}

PcmPreview* PcmGenerator::createPreview()
{
    auto result = std::make_unique<PcmPreview>();
    result->data.resize(32);
    previews[currentNode] = std::move(result);

    return static_cast<PcmPreview*>(previews[currentNode].get());
}

bool PcmGenerator::hasChanged()
{
    bool ret = false;
    for(auto node : collector.nodes)
        ret = node->getAttributes()->hasChanged() || ret;
    return ret;
}

void PcmGenerator::compute(BaseOperationNode* outputNode)
{
    reset();
    outputNode->accept(collector);

    if (!hasChanged()) return;

    CleanPreviews cleaner;
    outputNode->accept(cleaner);
    previews.clear();
    infos.clear();

    prepare(outputNode);

    for(auto node : collector.nodes)
        infos[node] = OperationInfo();

    for(auto node : collector.nodes)
    {
        auto& info = infos[node];
        auto op = signalOperations.find(node->nodeTypeId());

        info.operation = op->second.get();
        info.attributes = node->getAttributes();
        for(int i=0; i<node->getSourceCount(); ++i)
        {
            auto source = static_cast<BaseOperationNode*>(node->getSourceNode(i));
            if (source)
                info.inputs.push_back( &infos[source] );
            else
                info.inputs.push_back(nullptr);
        }
    }

    if (state.toPlay || state.toExport)
        generateAudio(outputNode);

    for(auto node : collector.nodes)
        computePreviews(node);

}

void PcmGenerator::computeResult(BaseOperationNode* node, PcmPreview* preview, Time time)
{
    auto& info = infos[node];
    for(size_t i=0; i<preview->count(); ++i)
    {
        preview->set(i, info.operation->sample(time, info.attributes, info.inputs));
        time.elapsed += time.timestep;
        time.advance = time.elapsed / time.duration;
    }
}

void PcmGenerator::computePreviews(BaseOperationNode* node)
{
    const size_t count = 32;

    currentNode = node;
    auto pcmPreview = createPreview();
    pcmPreview->data.resize(count);

    Time time;
    time.duration = 1.0f;
    time.elapsed = 0.0f;
    time.advance = 0.0f;
    time.timestep = time.duration / count;
    time.samplerate = 1.f / time.timestep;

    computeResult(node, pcmPreview, time);

    node->setPreview(previews[node].get());
    node->getAttributes()->validate();
}

void PcmGenerator::generateAudio(BaseOperationNode* node)
{
    const size_t count = state.duration * state.sampleRate;

    currentNode = node;
    auto pcmPreview = createPreview();
    pcmPreview->data.resize(count);

    Time time;
    time.duration = state.duration;
    time.elapsed = 0.0f;
    time.advance = 0.0f;
    time.samplerate = state.sampleRate;
    time.timestep = 1.0f / time.samplerate;

    computeResult(node, pcmPreview, time);

    std::unique_ptr<PcmDataBase> pcm;
    if (state.sampleBits == AudioSettings::Format_Mono8 || state.sampleBits == AudioSettings::Format_Stereo8)
        pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
    if (state.sampleBits == AudioSettings::Format_Mono16 || state.sampleBits == AudioSettings::Format_Stereo16)
        pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();

    pcm->sampleRate = (AudioSettings::SampleRate)state.sampleRate;
    pcm->resize((size_t)(state.duration * state.sampleRate));

    for(size_t i = 0; i<pcmPreview->data.size(); ++i)
        pcm->set(i, pcmPreview->data[i]);

    if (state.toExport)
        WavExporter::exportAsWAV(state.path, *pcm);

    if (state.toPlay)
    {
        auto& sound = *SoundNode::getDefault();
        if(sound.getState() != SoundNode::Playing)
        {
            sound.queue(*pcm.get());
            sound.play();
        }
    }
}
