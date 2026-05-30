#pragma once
#include <string>
#include <vector>
#include "platforms/platform.h"
#if defined(BOO_PLATFORM_ANDROID)
#include <android/native_window.h>
#include <android/asset_manager.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

class Android
{
private:
    int _width;
    int _height;
private:
#if defined(BOO_PLATFORM_ANDROID)
    ANativeWindow *_nativeWindow = nullptr;
    AAssetManager *_androidAssetsManager = nullptr;
#endif
public:
    Android();
#if defined(BOO_PLATFORM_ANDROID)
    Android(ANativeWindow *g_nativeWindow,AAssetManager *androidAssetsManager);
    ANativeWindow *getNativeWindow();
#endif
    void init();
    const int getWidth();
    const int getHeight();
    void resize(int width, int height);
    void onTouch(int action, float x, float y);
    void tick();
    void loadImage(const std::string &assetPath,std::vector<uint8_t> &pixels, int &width, int &height, int &channels);
    void loadText(const std::string &assetPath, std::string &text);
    void loadBinary(const std::string &assetPath, std::vector<uint8_t> &data);
    ~Android();
};
