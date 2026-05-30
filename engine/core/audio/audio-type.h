#pragma once
#include "libs/miniaudio/miniaudio.h"

namespace Boo{
    enum class EAudioState{
        None,
        Initialized,
        Playing,
        Paused,
        Stopped,
    };
    enum class EAudioType{
        Music,
        Sound,
    };  
}