#include "audio-system.h"
#include "log.h"
#include "core/audio/audio.h"
#include "core/asset/audio-asset.h"

namespace Boo
{
    AudioSystem::AudioSystem()
    {
        this->_config = ma_engine_config_init();
        this->_config.channels = 2;
        this->_config.sampleRate = 48000;
        this->_initialized = false;
        this->_decoderConfig = ma_decoder_config_init(
            ma_format_f32, // 输出格式
            2,             // 声道数（自动转）
            0              // 采样率（自动）
        );
    }

    void AudioSystem::init()
    {
        if (ma_engine_init(&this->_config, &this->_engine) != MA_SUCCESS)
        {
            LOGE("Failed to initialize engine.");
            this->_initialized = false;
            return;
        }
        this->_initialized = true;
    }
    Audio *AudioSystem::_getPlayerAudio(AudioAsset *audioAsset, EAudioType type)
    {
        int id = this->_audioId++;
        Audio *audio = new Audio(id);
        audio->init(audioAsset, type);
        if (type == EAudioType::Music)
        {
            this->_musics[id] = audio;
        }
        else
        {
            this->_sounds[id] = audio;
        }
        return audio;
    }
    Audio *AudioSystem::play(AudioAsset *audioAsset, EAudioType type, bool loop)
    {
        Audio *audio = this->_getPlayerAudio(audioAsset, type);
        audio->play(loop);
        return audio;
    }
    void AudioSystem::update(float dt)
    {
        for (auto &[id, audio] : this->_musics)
        {
            audio->update(dt);
        }
        for (auto &[id, audio] : this->_sounds)
        {
            audio->update(dt);
        }
        this->_cleanDestroyedAudio();
        // std::cout<<"AudioSystem update audio size: "<<this->_musics.size()<<" "<<this->_sounds.size()<<std::endl;
    }

    void AudioSystem::_cleanDestroyedAudio()
    {
        std::vector<Audio *> destroyedMusics;
        for (auto &[id, audio] : this->_musics)
        {
            if (audio->_isDestroyFlag())
            {
                destroyedMusics.push_back(audio);
            }
        }
        for (auto audio : destroyedMusics)
        {
            audio->_clean();
            int id = audio->getId();
            this->_musics.erase(id);
            delete audio;
            audio = nullptr;
        }
        destroyedMusics.clear();

        std::vector<Audio *> destroyedSounds;
        for (auto &[id, audio] : this->_sounds)
        {
            if (audio->_isDestroyFlag())
            {
                destroyedSounds.push_back(audio);
            }
        }
        for (auto audio : destroyedSounds)
        {
            audio->_clean();
            int id = audio->getId();
            this->_sounds.erase(id);
            delete audio;
            audio = nullptr;
        }
        destroyedSounds.clear();
    }
    void AudioSystem::_getMaDecoder(std::vector<uint8_t> &audioBuffer, ma_decoder &decoder)
    {
        if (!this->_initialized)
        {
            LOGE("AudioSystem not initialized");
            return;
        }
        if (audioBuffer.empty())
        {
            LOGE("audioBuffer is empty");
            return;
        }
        ma_result result = ma_decoder_init_memory(audioBuffer.data(), audioBuffer.size(), &this->_decoderConfig, &decoder);
        if (result != MA_SUCCESS)
        {
            LOGE("Failed to init decoder from memory: %d", result);
            return;
        }
    }
    void AudioSystem::_getMaSound(ma_decoder &decoder, ma_sound &sound)
    {
        if (!this->_initialized)
        {
            LOGE("AudioSystem not initialized");
            return;
        }

        ma_result result = ma_sound_init_from_data_source(
            &this->_engine,
            &decoder, // 需要指针
            0,        // flags
            nullptr,  // 可选：filters
            &sound    // 需要指针
        );
        if (result != MA_SUCCESS)
        {
            LOGE("Failed to init sound from decoder: %d", result);
        }
    }
    void AudioSystem::destroy()
    {
        ma_engine_uninit(&this->_engine);
    }
    AudioSystem::~AudioSystem()
    {
    }
} // namespace Boo