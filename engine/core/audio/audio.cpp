#include "audio.h"
#include "boo.h"
#include "log.h"
#include "core/asset/audio-asset.h"
namespace Boo
{
    Audio::Audio(int id) : _id(id),
                           asset(nullptr),
                           _duration(0.0f),
                           _time(0.0f),
                           _loop(false),
                           _volume(0.5f),
                           _state(EAudioState::None)
    {
        this->_id = id;
    }

    void Audio::init(AudioAsset *audioAsset, EAudioType type)
    {
        this->_duration = 0.0;
        this->_time = 0.0f;
        this->_destroyFlag = false;
        this->asset = audioAsset;
        this->_type = type;
        audioManager->_getMaDecoder(audioAsset->getAudioBuffer(), this->_decoder);
        audioManager->_getMaSound(this->_decoder, this->_sound);
        ma_sound_get_length_in_seconds(&this->_sound, &this->_duration);
        ma_sound_set_volume(&this->_sound, this->_volume);
        if (&this->_decoder == NULL || &this->_sound == NULL || this->_duration <= 0.0f)
        {
            LOGE("Audio::init: decoder or sound is nullptr");
            this->_destroyFlag = this->_type == EAudioType::Sound;
            return;
        }
        this->_state = EAudioState::Initialized;
    }
    void Audio::play(bool loop)
    {
        if (this->_state == EAudioState::None)
            return;
        this->stop();
        this->_time = 0;
        this->_loop = loop;
        this->_state = EAudioState::Playing;
        ma_sound_seek_to_second(&this->_sound, 0);
        ma_sound_set_looping(&this->_sound, this->_loop ? MA_TRUE : MA_FALSE); // 启用循环
        ma_sound_start(&this->_sound);
    }
    void Audio::pause()
    {
        if (this->_state != EAudioState::Playing)
            return;
        this->_state = EAudioState::Paused;
        ma_sound_get_cursor_in_seconds(&this->_sound, &this->_time); //_time
        ma_sound_stop(&this->_sound);
    }
    void Audio::resume()
    {
        if (this->_state != EAudioState::Paused)
            return;
        this->_state = EAudioState::Playing;
        ma_sound_seek_to_second(&this->_sound, this->_time);
        ma_sound_start(&this->_sound);
    }
    void Audio::stop()
    {
        if (this->_state != EAudioState::Playing)
            return;
        this->_time = 0;
        this->_state = EAudioState::Stopped;
        ma_sound_stop(&this->_sound);
        ma_sound_seek_to_second(&this->_sound, 0); // ✅ 关键：重置到开头
    }
    void Audio::setVolume(float volume)
    {
        if (this->_state == EAudioState::None)
            return;
        if (this->_volume == volume)
            return;
        this->_volume = std::clamp(volume, 0.0f, 1.0f);
        ma_sound_set_volume(&this->_sound, this->_volume);
    }
    float Audio::getVolume()
    {
        return this->_volume;
    }
    const EAudioState Audio::getState()
    {
        return this->_state;
    }
    const int Audio::getId()
    {
        return this->_id;
    }
    void Audio::update(float dt)
    {
        // 非播放状态, 不更
        if (this->_state != EAudioState::Playing)
            return;
        ma_sound_get_cursor_in_seconds(&this->_sound, &this->_time);
        if (this->_type == EAudioType::Sound)
        {
            if (!ma_sound_is_playing(&_sound))
            {
                this->destroy();
            }
            return;
        }
    }
    void Audio::destroy()
    {
        this->_state = EAudioState::None;
        this->_destroyFlag = true;
    }
    bool Audio::_isDestroyFlag()
    {
        return this->_destroyFlag;
    }
    void Audio::_clean()
    {
        this->_duration = 0.0;
        this->_time = 0.0f;
        this->asset = nullptr;
        ma_sound_uninit(&this->_sound);
        ma_decoder_uninit(&this->_decoder);
    }
    Audio::~Audio()
    {
    }
} // namespace Boo
// 设置音量
//  ma_sound_set_volume(&currentMusic, volume);