#include "audio-asset.h"
#include "boo.h"
#include "log.h"

namespace Boo
{
    AudioAsset::AudioAsset()
    {
        this->_type = EAssetType::Audio;
    }
    AudioAsset::AudioAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = EAssetType::Audio;
    }
    AudioAsset::AudioAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = EAssetType::Audio;
    }
    void AudioAsset::create(std::vector<uint8_t> &audioBuffer)
    {
        this->_audioBuffer = audioBuffer;
        ma_decoder _decoder;
        ma_sound sound;
        audioManager->_getMaDecoder(this->_audioBuffer, _decoder);
        audioManager->_getMaSound(_decoder, sound);
        ma_sound_get_length_in_seconds(&sound, &this->_totalSeconds);
        // 5️⃣ 清理
        ma_sound_uninit(&sound);
        ma_decoder_uninit(&_decoder);
    }
    const float &AudioAsset::getTotalSeconds()
    {
        return this->_totalSeconds;
    }
    std::vector<uint8_t> &AudioAsset::getAudioBuffer()
    {
        return this->_audioBuffer;
    }
    void AudioAsset::destroy()
    {
    }
}
