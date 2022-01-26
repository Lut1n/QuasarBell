#include "SignalOperation/PcmBuilder.hpp"

#include "Audio/WavExporter.hpp"

//--------------------------------------------------------------
struct PcmGeneratorWork : public qb::ProgressiveWork::Work
{
    std::list<std::unique_ptr<PcmDataBase>> pcmQueue;

    BaseOperationNode* node = nullptr;
    std::unordered_map<BaseOperationNode*, OperationInfo> infos;
    int sampleBits = 16;

    Time time;
    float duration = 1.0f;
    size_t sampleRate = 0;
    size_t count = 0;
    size_t index = 0;
    size_t remaining = 0;

    const float chunkDuration = 1.0f;
    size_t chunkSize = 0;

    PcmGeneratorWork(BaseOperationNode* node, const std::unordered_map<BaseOperationNode*, OperationInfo>& infos, float duration, size_t sampleRate, int sampleBits)
        : node(node)
        , infos(infos)
        , sampleBits(sampleBits)
        , duration(duration)
        , sampleRate(sampleRate)
    {
        count = (size_t)duration * sampleRate;
        remaining = count;
        chunkSize = (size_t)chunkDuration * sampleRate;
        
        time = Time();
        time.duration = duration;
        time.elapsed = 0.0f;
        time.advance = 0.0f;
        time.timestep = time.duration / count;
        time.samplerate = 1.f / time.timestep;
    }

    void play()
    {
        if (remaining <= 0)
            return;
        
        size_t size = qb::min(remaining, chunkSize);
        std::vector<float> data(size);

        auto& info = infos[node];
        for(size_t i=0; i<size; ++i)
        {
            data[i] = info.operation->sample(time, info.attributes, info.inputs);
            time.elapsed += time.timestep;
            time.advance = time.elapsed / time.duration;
        }

        std::unique_ptr<PcmDataBase> pcm;
        if (sampleBits == AudioSettings::Format_Mono8 || sampleBits == AudioSettings::Format_Stereo8)
            pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
        if (sampleBits == AudioSettings::Format_Mono16 || sampleBits == AudioSettings::Format_Stereo16)
            pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();

        pcm->sampleRate = (AudioSettings::SampleRate)sampleRate;
        pcm->resize((size_t)size);

        for(size_t i = 0; i<data.size(); ++i)
            pcm->set(i, data[i]);

        auto& sound = *SoundNode::getDefault();
        sound.queue(*pcm);
        if(sound.getState() != SoundNode::Playing)
        {
            sound.play();
        }

        index += size;
        remaining -= size;
    }

    void stop()
    {
        auto& sound = *SoundNode::getDefault();
        sound.stop();
    }

    bool isPlaying()
    {
        auto& sound = *SoundNode::getDefault();
        return sound.getState() == SoundNode::Playing;
    }
};

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
        state.toStop = output->toStop;
        state.toExport = output->toExport;
        output->toPlay = false;
        output->toStop = false;
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

    if (auto output = dynamic_cast<OutputData*>(outputNode->getAttributes()))
    {
        if (output->pcmPlaying.initialized)
        {
            if (output->pcmPlaying.completed || hasChanged() || output->toStop)
            {
                if (output->toStop)
                {
                    auto& work = output->pcmPlaying.getWork<PcmGeneratorWork>();
                    work.stop();
                }
                output->pcmPlaying.reset();
            }
            else
            {
                auto& work = output->pcmPlaying.getWork<PcmGeneratorWork>();
                work.play();
                output->pcmPlaying.progress = (float)work.index / (float)work.count;
                output->pcmPlaying.completed = output->pcmPlaying.progress >= 1.0f && !work.isPlaying();
            }
        }
    }

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
    if (state.toExport)
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
        WavExporter::exportAsWAV(state.path, *pcm);
    }

    if (state.toPlay)
    {
        if (auto output = dynamic_cast<OutputData*>(node->getAttributes()))
        {
            output->pcmPlaying.initialize<PcmGeneratorWork>(node, infos, state.duration, state.sampleRate, state.sampleBits);
        }
    }
}
