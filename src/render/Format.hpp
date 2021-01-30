#ifndef QUASAR_BELL_FORMAT_H
#define QUASAR_BELL_FORMAT_H

//--------------------------------------------------------------
ALenum getFormatAL(AudioSettings::Format format)
{
    switch(format)
    {
        case AudioSettings::Format_Mono8:
        return AL_FORMAT_MONO8;
        case AudioSettings::Format_Mono16:
        return AL_FORMAT_MONO16;
        case AudioSettings::Format_Stereo8:
        return AL_FORMAT_STEREO8;
        case AudioSettings::Format_Stereo16:
        return AL_FORMAT_STEREO16;
    };
    
    return AL_FORMAT_MONO16;
}

#endif // QUASAR_BELL_FORMAT_H