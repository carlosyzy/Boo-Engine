#include "android.h"
#include "boo.h"
#include "log.h"
#include "libs/stb/stb_image.h"
#include "core/game.h"
Android::Android()
{
    this->_width = 0;
    this->_height = 0;
}
#if defined(BOO_PLATFORM_ANDROID)
Android::Android(ANativeWindow *g_nativeWindow, AAssetManager *androidAssetsManager)
{
    this->_nativeWindow = g_nativeWindow;
    this->_androidAssetsManager = androidAssetsManager;
    this->_width = ANativeWindow_getWidth(g_nativeWindow);
    this->_height = ANativeWindow_getHeight(g_nativeWindow);
}
ANativeWindow *Android::getNativeWindow()
{
    return this->_nativeWindow;
}
#endif
const int Android::getWidth()
{
    return this->_width;
}
const int Android::getHeight()
{
    return this->_height;
}
void Android::resize(int width, int height)
{
    this->_width = width;
    this->_height = height;
    Boo::game->resizeView(this->_width, this->_height);
}
void Android::onTouch(int action, float x, float y)
{
    Boo::game->updateTouch(action, x, y);
}
void Android::tick()
{
}
void Android::loadImage(const std::string &assetPath, std::vector<uint8_t> &pixels, int &width, int &height, int &channels)
{
    width = 0;
    height = 0;
    channels = 0;
    std::vector<uint8_t> buffer;
    this->loadBinary(assetPath, buffer);
    if (buffer.empty())
    {
        return;
    }
    // 从内存加载图片
    unsigned char *_pixels = stbi_load_from_memory(
        buffer.data(),
        static_cast<int>(buffer.size()),
        &width, &height, &channels,
        STBI_rgb_alpha // 强制转换为 RGBA
    );
    if (!_pixels)
    {
        LOGE("stb_image failed to load: %s, error: %s",
             assetPath.c_str(), stbi_failure_reason());
        return;
    }
    std::string name = std::filesystem::path(assetPath).stem().string();
    // 3. 创建纹理数据
    channels = 4; // STBI_rgb_alpha 强制为 4 通道
    pixels = std::vector<uint8_t>(static_cast<const uint8_t *>(_pixels), static_cast<const uint8_t *>(_pixels) + (width * height * channels));
    // 4. 释放 stb_image 内存
    stbi_image_free(_pixels);
}
void Android::loadText(const std::string &assetPath, std::string &text)
{
    text = "";
#if defined(BOO_PLATFORM_ANDROID)
    // 打开 asset 文件
    AAsset *asset = AAssetManager_open(this->_androidAssetsManager, assetPath.c_str(), AASSET_MODE_BUFFER);
    if (!asset)
    {
        LOGE("Failed to open asset: %s", assetPath.c_str());
        return;
    }
    // 获取文件大小
    off_t fileSize = AAsset_getLength(asset);
    if (fileSize <= 0)
    {
        LOGE("Asset file size is 0: %s", assetPath.c_str());
        AAsset_close(asset);
        return;
    }
    // 读取文件内容到字符串
    text.resize(fileSize);
    int bytesRead = AAsset_read(asset, &text[0], fileSize);
    AAsset_close(asset);
    if (bytesRead != fileSize)
    {
        LOGE("Failed to read entire asset: %s (read %d of %lld)", assetPath.c_str(), bytesRead, (long long)fileSize);
        return;
    }
#endif
}
void Android::loadBinary(const std::string &assetPath, std::vector<uint8_t> &data)
{
    data.clear();
#if defined(BOO_PLATFORM_ANDROID)
    // 打开 asset 文件
    AAsset *asset = AAssetManager_open(this->_androidAssetsManager, assetPath.c_str(), AASSET_MODE_BUFFER);
    if (!asset)
    {
        LOGE("Failed to open asset: %s", assetPath.c_str());
        return;
    }
    // 获取文件大小
    off_t fileSize = AAsset_getLength(asset);
    if (fileSize <= 0)
    {
        LOGE("Asset file size is 0: %s", assetPath.c_str());
        AAsset_close(asset);
        return;
    }
    // 读取文件内容
    data.resize(fileSize);
    int bytesRead = AAsset_read(asset, data.data(), fileSize);
    AAsset_close(asset);
    if (bytesRead != fileSize)
    {
        LOGE("Failed to read entire asset: %s (read %d of %lld)", assetPath.c_str(), bytesRead, (long long)fileSize);
        return;
    }
#endif
}

Android::~Android()
{
}
