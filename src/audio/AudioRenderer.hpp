#ifndef QUASAR_BELL_RENDERER_HPP
#define QUASAR_BELL_RENDERER_HPP

#include <vector>
#include <memory>

#include <al.h>
#include <alc.h>

#include "Audio/PcmData.hpp"

//--------------------------------------------------------------
class vec3
{
public:
    vec3() : x(0.f), y(0.f), z(0.f) {}
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    
    float& operator[](unsigned i) { return v[i]; }

public:
    union
    {
        float v[3];
        struct {float x, y, z;};
    };
};

//--------------------------------------------------------------
class Node
{
public:
    Node() : _changed(false) {}
    
    void setPosition(const vec3& p)
    {
        _position = p;
        _changed = true;
    }
    
    void setVelocity(const vec3& v)
    {
        _velocity = v;
        _changed = true;
    }
    
    const vec3& getPosition() const
    {
        return _position;
    }
    
    const vec3& getVelocity() const
    {
        return _velocity;
    }
    virtual void update()
    {
        if(_changed)
        {
            updateImpl();
            _changed = false;
        }
    }
    
protected:
    virtual void updateImpl() = 0;
    
public:
    vec3 _position;
    vec3 _velocity;
    bool _changed;
};


//--------------------------------------------------------------
class Scene
{
public:
    Scene();
    virtual ~Scene();
    
    void update();
    void add(Node* node);
    void rem(Node* node);
    
public:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    ALenum error;
    
    std::vector<Node*> nodes;
};

//--------------------------------------------------------------
// basic audio source node where to attach an AudioClip (like GeoNode)
//--------------------------------------------------------------
class SoundNode : public Node
{
public:
    enum State
    {
        Initial,
        Playing,
        Paused,
        Stopped
    };
    
    SoundNode(bool isStream = false);
    virtual ~SoundNode();
    
    State getState() const;
    
    void load(const PcmDataBase& pcm);
    void queue(const PcmDataBase& pcm);
    void play();
    
    virtual void update() override;
    
    void setAsDefault();
    static SoundNode* getDefault();
    
protected:
    void updateStream();
    void updateImpl() override;

public:    
    ALuint _sourceID = 0;
    ALuint _bufferID = 0;
    ALuint _streamBufferID[4] = {0,0,0,0};
    std::vector<ALuint> _bufferReady;
    std::vector<std::unique_ptr<PcmDataBase>> _PcmDatas;
    
    float _pitch = 1.0f;
    float _gain = 1.0f;
    bool _looping = false;
    bool _stream = false;
};

//--------------------------------------------------------------
// basic audio listener (like Camera)
//--------------------------------------------------------------
class MicroNode : public Node
{
public:
    MicroNode();
    virtual ~MicroNode();
    
    void setOrientation(const vec3& at, const vec3& up)
    {
        _lookAt = at;
        _lookUp = up;
        _changed = true;
    }
    
    protected:
    void updateImpl() override;
    
private:
    vec3 _lookAt; // forward
    vec3 _lookUp;
};

#endif // QUASAR_BELL_RENDERER_HPP