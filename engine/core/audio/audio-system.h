#pragma once
#include <vector>
#include <unordered_map>
#include "core/audio/audio-type.h"

namespace Boo
{
    class Audio;
    class AudioAsset;
    class AudioSystem
    {
    private:
        ma_engine _engine;
        ma_engine_config _config;
        ma_decoder_config _decoderConfig;
        bool _initialized;
        int _audioId = 0;

        std::unordered_map<int, Audio *> _musics;
        std::unordered_map<int, Audio *> _sounds;
        Audio *_getPlayerAudio(AudioAsset *audioAsset, EAudioType type);
        void _cleanDestroyedAudio();
    public:
        AudioSystem();
        void init();
        Audio *play(AudioAsset *audioAsset, EAudioType type, bool loop);
        void update(float dt);
        void _getMaDecoder(std::vector<uint8_t> &audioBuffer, ma_decoder &decoder);
        void _getMaSound(ma_decoder &decoder, ma_sound &sound);
        void destroy();
        ~AudioSystem();
    };
} // namespace Boo

// // 3. 从内存播放
//     ma_sound sound;
//     ma_result result = ma_sound_init_from_memory(&engine,
//                                                   audioData.data(),
//                                                   audioData.size(),
//                                                   0,      // 标志
//                                                   NULL,   // 额外配置
//                                                   &sound);

//     if (result != MA_SUCCESS) {
//         std::cout << "从内存播放失败" << std::endl;
//         ma_engine_uninit(&engine);
//         return -1;
//     }

//     // 4. 开始播放
//     ma_sound_start(&sound);

//     std::cout << "播放中... 按回车键退出" << std::endl;