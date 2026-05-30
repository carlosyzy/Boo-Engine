#pragma once
#include "core/audio/audio-type.h"
namespace Boo
{
    class AudioAsset;
    class Audio
    {
    private:
        ma_decoder _decoder;
        ma_sound _sound;
    private:
        int _id;
        AudioAsset *asset;
        float _duration;
        float _time;
        bool _loop;
        float _volume;
        EAudioState _state;
        EAudioType _type;
        bool _destroyFlag;
    public:
        void _clean();
        bool _isDestroyFlag();

    public:
        Audio(int id);
        void init(AudioAsset *audioAsset, EAudioType type);
        void play(bool loop);
        void pause();
        void resume();
        void stop();
        void setVolume(float volume);
        float getVolume();
        const EAudioState getState();
        const int getId();
        void update(float dt);
        void destroy();
        ~Audio();
    };
} // namespace Boo
