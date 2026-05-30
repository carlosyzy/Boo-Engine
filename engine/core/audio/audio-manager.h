#pragma once
#include <string>
#include <vector>
#include "core/audio/audio-type.h"
namespace Boo
{
    class AudioSystem;
    class Audio;
    class AudioAsset;
    class AudioManager
    {
    private:
        AudioSystem *_audioSystem = nullptr;

    public:
        void _getMaDecoder(std::vector<uint8_t> &audioBuffer, ma_decoder &decoder);
        void _getMaSound(ma_decoder &decoder, ma_sound &sound);

    public:
        AudioManager();
        ~AudioManager();
        void init();
        /**
         * @brief 播放音乐
         * @param audioPath 音乐路径
         * @return Audio对象
         * 播放完毕后不会自动销毁，需要手动调用destroy方法
         */
        Audio *playMusic(std::string audioPath, bool loop);
        /**
         * @brief 播放音乐
         *
         * @param audioAsset 音乐资产
         * @return Audio对象
         */
        Audio *playMusic(AudioAsset *audioAsset, bool loop);
        void playSound(std::string audioPath);
        void playSound(AudioAsset *audioAsset);
        void update(float dt);
        void destroy();
    };
}