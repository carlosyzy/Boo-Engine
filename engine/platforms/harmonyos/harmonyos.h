#pragma once
#include "platforms/platform.h"
#if defined(BOO_PLATFORM_HARMONYOS)
   #include <native_window/external_window.h>
#endif
class HarmonyOS
{
private:
    int _width;
    int _height;
private:
#if defined(BOO_PLATFORM_HARMONYOS)
    OHNativeWindow *_nativeWindow = nullptr;
#endif
public:
    HarmonyOS();
#if defined(BOO_PLATFORM_HARMONYOS)
    HarmonyOS(OHNativeWindow* window);
    OHNativeWindow *getNativeWindow();
#endif
    const int getWidth();
	const int getHeight();
    void tick();
};