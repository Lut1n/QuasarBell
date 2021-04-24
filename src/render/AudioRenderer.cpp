#include "AudioRenderer.hpp"

#include <algorithm> // find
#include <cstring> // memcpy

#include <al.h>
#include <alc.h>

#include "signal/Signal.hpp"
#include "render/Error.hpp"
#include "render/Format.hpp"

//--------------------------------------------------------------
Scene::Scene()
{
    device = alcOpenDevice(NULL);
    if (device)
    {
        context = alcCreateContext(device,NULL);
        alcMakeContextCurrent(context);
    }
}
//--------------------------------------------------------------
Scene::~Scene()
{
    if(device)
    {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
}
//--------------------------------------------------------------
void Scene::update()
{
    for(auto it : nodes)
    {
        it->update();
    }
}
//--------------------------------------------------------------
void Scene::add(Node* node)
{
    nodes.push_back(node);
}
//--------------------------------------------------------------
void Scene::rem(Node* node)
{
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if(it != nodes.end()) nodes.erase(it);
}

//--------------------------------------------------------------
SoundNode::SoundNode(bool isStream)
    : Node()
    , _stream(isStream)
{
    alGetError(); // clear error code
    
    // Generate Sources
    alGenSources(1, &_sourceID);
    ackErr("alGenSources 1 : ");
    
    if(_stream)
    {
        alGenBuffers(4, _streamBufferID);
        ackErr("alGenBuffers");
        for(unsigned i=0; i<4; ++i)
            _bufferReady.push_back(_streamBufferID[i]);
    }
}
//--------------------------------------------------------------
SoundNode::~SoundNode()
{
    alDeleteSources(1, &_sourceID);
    if(_bufferID != 0)
        alDeleteBuffers(1, &_bufferID);
    
    if(_streamBufferID[0] != 0)
    {
        alDeleteBuffers(4, _streamBufferID);
        ackErr("alDelBuffers");
    }
}
//--------------------------------------------------------------
SoundNode::State SoundNode::getState() const
{
    ALint source_state;
    alGetSourcei(_sourceID, AL_SOURCE_STATE, &source_state);
    if(source_state == AL_INITIAL)
        return Initial;
    if(source_state == AL_PLAYING)
        return Playing;
    if(source_state == AL_PAUSED)
        return Paused;
    if(source_state == AL_STOPPED)
        return Stopped;
    
    return Initial;
}
//--------------------------------------------------------------
void SoundNode::load(const PcmDataBase& pcm)
{
    if(_stream) return;
    
    alGetError(); // clear error code

    // detach from source
    alSourcei(_sourceID, AL_BUFFER, 0);
    ackErr("aDetach");
    
    if(_bufferID != 0)
    {
        alDeleteBuffers(1, &_bufferID);
        ackErr("alDelBuffers");
    }
    
    if(_streamBufferID[0] != 0)
    {
        alDeleteBuffers(4, _streamBufferID);
        ackErr("alDelBuffers");
    }
    
    // Generate Buffers
    alGenBuffers(1, &_bufferID);
    ackErr("alGenBuffers");
    
    alBufferData(_bufferID, getFormatAL(pcm.format), pcm.data(), pcm.size(), pcm.sampleRate);
    ackErr("alBufferData buffer 0");
    // Attach buffer 0 to source
    alSourcei(_sourceID, AL_BUFFER, _bufferID);
    ackErr("alSourcei AL_BUFFER 0");
    
    update();
}
//--------------------------------------------------------------
void SoundNode::queue(const PcmDataBase& pcm)
{
    std::unique_ptr<PcmDataBase> dst;
    if (pcm.format == AudioSettings::Format_Mono8 || pcm.format == AudioSettings::Format_Stereo8)
        dst = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
    if (pcm.format == AudioSettings::Format_Mono16 || pcm.format == AudioSettings::Format_Stereo16)
        dst = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();
    if (dst.get() == nullptr) return;
    
    pcm.copyTo(*dst);
    _PcmDatas.emplace_back(std::move(dst));
}
//--------------------------------------------------------------
void SoundNode::play()
{
    updateStream();
    
    // play generated sound
    alSourcePlay(_sourceID);
}
//--------------------------------------------------------------
void SoundNode::update()
{
    if(_changed)
    {
        updateImpl();
        _changed = false;
    }
    
    updateStream();
}
//--------------------------------------------------------------
void SoundNode::updateStream()
{
    if(!_stream) return;
    
    // auto& settings = AudioSettings::defaultSettings;
    
    ALint iBuffersProcessed;
    alGetSourcei(_sourceID, AL_BUFFERS_PROCESSED, &iBuffersProcessed);
    
    for(int i=0; i<iBuffersProcessed; ++i)
    {
        ALuint uiBuffer;
        alSourceUnqueueBuffers(_sourceID, 1, &uiBuffer);
        ackErr("alSourceUnqueueBuffers");
        _bufferReady.push_back(uiBuffer);
    }
    
    
    if(_bufferReady.size() > 0 && _PcmDatas.size() > 0)
    {
        unsigned count = std::min(_bufferReady.size(), _PcmDatas.size());
        for(unsigned i=0;i<count;++i)
        {
            alBufferData(_bufferReady[i], getFormatAL(_PcmDatas[i]->format), _PcmDatas[i]->data(), _PcmDatas[i]->size(), _PcmDatas[i]->sampleRate);
            ackErr("alBufferData");
            alSourceQueueBuffers(_sourceID, 1, &_bufferReady[i]);
            ackErr("alSourceQueueBuffers");
        }
        
        for(unsigned i=0;i<count; ++i)
        {
            _PcmDatas.erase(_PcmDatas.begin());
            _bufferReady.erase(_bufferReady.begin());
        }
    }
}
//--------------------------------------------------------------
void SoundNode::updateImpl()
{
    alGetError(); // clear error state
    alSourcef(_sourceID,AL_PITCH,_pitch);
    ackErr("alSourcef 0 AL_PITCH");
    
    alSourcef(_sourceID,AL_GAIN,_gain);
    ackErr("alSourcef 0 AL_GAIN");
     
    alSourcefv(_sourceID,AL_POSITION,_position.v);
    ackErr("alSourcefv 0 AL_POSITION");

    alSourcefv(_sourceID,AL_VELOCITY,_velocity.v);
    ackErr("alSourcefv 0 AL_VELOCITY");
    
    alSourcei(_sourceID,AL_LOOPING,_looping);
    ackErr("alSourcei 0 AL_LOOPING true");
}

//--------------------------------------------------------------
MicroNode::MicroNode()
    : Node()
    , _lookAt(0.0,0.0,-1.0)
    , _lookUp(0.0,1.0,0.0)
{
    update();
}
//--------------------------------------------------------------
MicroNode::~MicroNode()
{
}
//--------------------------------------------------------------
void MicroNode::updateImpl()
{
    // Listener setting up
    alGetError(); // clear error state
    
    alListenerfv(AL_POSITION, _position.v);
    ackErr("alListenerfv POSITION");
        
    alListenerfv(AL_VELOCITY, _velocity.v);
    ackErr("alListenerfv VELOCITY");
    
    size_t vec3Size = sizeof(vec3);
    ALfloat orientation[6];
    std::memcpy(orientation, _lookAt.v, vec3Size);
    std::memcpy(orientation + vec3Size, _lookUp.v, vec3Size);
    alListenerfv(AL_ORIENTATION, orientation);
    ackErr("alListenerfv ORIENTATION");
}

