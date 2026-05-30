#include "audio-manager.h"
#include "boo.h"
#include "log.h"
#include "core/audio/audio-system.h"
#include "core/asset/audio-asset.h"
#include "core/audio/audio.h"
#include "core/asset/asset-manager.h"

namespace Boo
{
    AudioManager::AudioManager()
    {
        this->_audioSystem = new AudioSystem();
    }
    void AudioManager::init()
    {
        this->_audioSystem->init();
    }
    Audio *AudioManager::playMusic(std::string audioPath, bool loop)
    {
        Asset *asset = assetManager->getAsset(audioPath, true);
        AudioAsset *audioAsset = dynamic_cast<AudioAsset *>(asset);
        return this->playMusic(audioAsset, loop);
    }
    Audio *AudioManager::playMusic(AudioAsset *audioAsset, bool loop)
    {
        if (audioAsset == nullptr)
        {
            LOGW("AudioManager::playMusic: audioAsset is nullptr");
            return nullptr;
        }
        return this->_audioSystem->play(audioAsset, EAudioType::Music, loop);
    }
    void AudioManager::playSound(std::string audioPath)
    {
        Asset *asset = assetManager->getAsset(audioPath, true);
        AudioAsset *audioAsset = dynamic_cast<AudioAsset *>(asset);
        this->playSound(audioAsset);
    }
    void AudioManager::playSound(AudioAsset *audioAsset)
    {
        if (audioAsset == nullptr)
        {
            LOGW("AudioManager::playSound: audioAsset is nullptr");
            return;
        }
        this->_audioSystem->play(audioAsset, EAudioType::Sound, false);
    }
    void AudioManager::update(float dt)
    {
        this->_audioSystem->update(dt);
    }

    void AudioManager::_getMaDecoder(std::vector<uint8_t> &audioBuffer, ma_decoder &decoder)
    {
        this->_audioSystem->_getMaDecoder(audioBuffer, decoder);
    }
    void AudioManager::_getMaSound(ma_decoder &decoder, ma_sound &sound)
    {
        this->_audioSystem->_getMaSound(decoder, sound);
    }
    void AudioManager::destroy()
    {
        this->_audioSystem->destroy();
    }
    AudioManager::~AudioManager()
    {
        delete this->_audioSystem;
    }
}