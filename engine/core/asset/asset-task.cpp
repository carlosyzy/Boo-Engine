
#include "asset-task.h"
#include <filesystem>
#include <iostream>
#include "boo.h"
#include "log.h"
#include "platforms/platform.h"
#include "platforms/windows/windows.h"     //windows下的窗口封装
#include "platforms/android/android.h"     //android下的窗口封装
#include "platforms/macos/macos.h"         //macos下的窗口封装
#include "platforms/harmonyos/harmonyos.h" //harmonyos下的窗口封装

#include "core/util/json-util.h"
#include "core/asset/texture-asset.h"
#include "core/asset/shader-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/scene-asset.h"
#include "core/asset/asset-manager.h"
#include "core/asset/gltf-asset.h"
#include "core/asset/bmfont-asset.h"
#include "core/asset/audio-asset.h"
// #include <fastgltf/core.hpp>
// #include <fastgltf/tools.hpp>

namespace Boo
{
    AssetTask::AssetTask()
    {
    }
    Asset *AssetTask::load(const std::string &assetPath)
    {
        if (this->_isTextureAsset(assetPath))
        {
            return this->_createTexture(assetPath);
        }
        else if (this->_isShaderVertAsset(assetPath))
        {
            return this->_createShader(assetPath, ShaderVertexAsset);
        }
        else if (this->_isShaderFragAsset(assetPath))
        {
            return this->_createShader(assetPath, ShaderFragmentAsset);
        }
        else if (this->_isMaterialAsset(assetPath))
        {
            return this->_createMaterial(assetPath);
        }
        else if (this->_isGLBAsset(assetPath))
        {
            return this->_createGLBAsset(assetPath);
        }
        else if (this->_isBMFontAsset(assetPath))
        {
            return this->_createBMFontAsset(assetPath);
        }
        else if (this->_isAudioAsset(assetPath))
        {
            return this->_createAudioAsset(assetPath);
        }
        return nullptr;
    }
    bool AssetTask::_isTextureAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".png" || assetExtension == ".PNG" || assetExtension == ".jpg" || assetExtension == ".JPG" || assetExtension == ".jpeg" || assetExtension == ".JPEG";
    }
    bool AssetTask::_isShaderVertAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".vert" || assetExtension == ".VERT";
    }
    bool AssetTask::_isShaderFragAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".frag" || assetExtension == ".FRAG";
    }
    bool AssetTask::_isMaterialAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".mtl" || assetExtension == ".MTL";
    }
    bool AssetTask::_isGLBAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".glb" || assetExtension == ".GLB";
        // assetExtension == ".gltf" || assetExtension == ".GLTF" ||
    }
    bool AssetTask::_isBMFontAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".fnt" || assetExtension == ".FNT";
    }
    bool AssetTask::_isAudioAsset(const std::string &assetPath)
    {
        std::string assetExtension = std::filesystem::path(assetPath).extension().string();
        return assetExtension == ".mp3" || assetExtension == ".MP3" || assetExtension == ".wav" || assetExtension == ".WAV" || assetExtension == ".ogg" || assetExtension == ".OGG";
    }

    Asset *AssetTask::_createTexture(const std::string &assetPath)
    {
#if defined(BOO_PLATFORM_WINDOWS)
        int width = 0;
        int height = 0;
        int channels = 0;
        std::vector<uint8_t> pixels;
        game->getWindows()->loadImage(assetPath, pixels, width, height, channels);
        if (pixels.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        TextureAsset *texture = new TextureAsset(assetPath, assetPath, name);
        texture->create(width, height, channels, pixels, GfxTextureFormat::R8G8B8A8_SRGB);
        return texture;
#elif defined(BOO_PLATFORM_MACOS)
        int width = 0;
        int height = 0;
        int channels = 0;
        std::vector<uint8_t> pixels;
        game->getMacOS()->loadImage(assetPath, pixels, width, height, channels);
        if (pixels.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        TextureAsset *texture = new TextureAsset(assetPath, assetPath, name);
        texture->create(width, height, channels, pixels, GfxTextureFormat::R8G8B8A8_SRGB);
        return texture;
#elif defined(BOO_PLATFORM_ANDROID)
        int width = 0;
        int height = 0;
        int channels = 0;
        std::vector<uint8_t> pixels;
        game->getAndroid()->loadImage(assetPath, pixels, width, height, channels);
        if (pixels.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        TextureAsset *texture = new TextureAsset(assetPath, assetPath, name);
        texture->create(width, height, channels, pixels, GfxTextureFormat::R8G8B8A8_SRGB);
        return texture;
#elif defined(BOO_PLATFORM_HARMONYOS)
        return nullptr;
#endif
        return nullptr;
    }
    Asset *AssetTask::_createShader(const std::string &assetPath, const std::string &type)
    {

#if defined(BOO_PLATFORM_WINDOWS)
        std::string glslData = "";
        game->getWindows()->loadText(assetPath, glslData);
        if (glslData.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        ShaderAsset *shaderAsset = new ShaderAsset(assetPath, assetPath, name);
        shaderAsset->create(type, glslData, {});
        return shaderAsset;
#elif defined(BOO_PLATFORM_MACOS)
        std::string glslData = "";
        game->getMacOS()->loadText(assetPath, glslData);
        if (glslData.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        ShaderAsset *shaderAsset = new ShaderAsset(assetPath, assetPath, name);
        shaderAsset->create(type, glslData, {});
        return shaderAsset;
#elif defined(BOO_PLATFORM_ANDROID)
        std::string glslData = "";
        game->getAndroid()->loadText(assetPath, glslData);
        if (glslData.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        ShaderAsset *shaderAsset = new ShaderAsset(assetPath, assetPath, name);
        shaderAsset->create(type, glslData, {});
        return shaderAsset;
#elif defined(BOO_PLATFORM_HARMONYOS)
        return nullptr;
#endif
        return nullptr;
    }
    Asset *AssetTask::_createMaterial(const std::string &assetPath)
    {
#if defined(BOO_PLATFORM_WINDOWS)
        std::string materail = "";
        game->getWindows()->loadText(assetPath, materail);
        if (materail.empty())
        {
            return nullptr;
        }
        json materialJson = json::parse(materail);
        if (materialJson.is_null() || !materialJson.is_object() || !materialJson.contains("layer") || !materialJson.contains("type") || !materialJson.contains("vert") || !materialJson.contains("frag"))
        {
            LOGW("AssetLoad:Failed to load MaterialAsset: %s", assetPath.c_str());
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        MaterialAsset *material = new MaterialAsset(assetPath, assetPath, name);
        material->create(materialJson);
        return material;
#elif defined(BOO_PLATFORM_MACOS)
        std::string materail = "";
        game->getMacOS()->loadText(assetPath, materail);
        if (materail.empty())
        {
            return nullptr;
        }
        json materialJson = json::parse(materail);
        if (materialJson.is_null() || !materialJson.is_object() || !materialJson.contains("layer") || !materialJson.contains("type") || !materialJson.contains("vert") || !materialJson.contains("frag"))
        {
            LOGW("AssetLoad:Failed to load MaterialAsset: %s", assetPath.c_str());
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        MaterialAsset *material = new MaterialAsset(assetPath, assetPath, name);
        material->create(materialJson);
        return material;
#elif defined(BOO_PLATFORM_ANDROID)
        std::string materail = "";
        game->getAndroid()->loadText(assetPath, materail);
        if (materail.empty())
        {
            return nullptr;
        }
        json materialJson = json::parse(materail);
        if (materialJson.is_null() || !materialJson.is_object() || !materialJson.contains("layer") || !materialJson.contains("type") || !materialJson.contains("vert") || !materialJson.contains("frag"))
        {
            LOGW("AssetLoad:Failed to load MaterialAsset: %s", assetPath.c_str());
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        MaterialAsset *material = new MaterialAsset(assetPath, assetPath, name);
        material->create(materialJson);
        return material;
#elif defined(BOO_PLATFORM_HARMONYOS)
        return nullptr;
#endif
        return nullptr;
    }
    Asset *AssetTask::_createGLBAsset(const std::string &assetPath)
    {
#if defined(BOO_PLATFORM_WINDOWS)
#elif defined(BOO_PLATFORM_MACOS)
#elif defined(BOO_PLATFORM_ANDROID)
#elif defined(BOO_PLATFORM_HARMONYOS)
        return nullptr;
#endif
        // #if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
        //         if (!this->_isWindowAssetFileExist(assetPath))
        //         {
        //             return nullptr;
        //         }
        //         std::filesystem::path fullPath = (std::filesystem::path(assetManager->getAssetsRoot()) / assetPath).generic_string();
        //         auto bufferResult = fastgltf::GltfDataBuffer::FromPath(fullPath);
        //         if (bufferResult.error() != fastgltf::Error::None)
        //         {
        //             // std::cerr << "Failed to load file: " << fastgltf::getErrorString(bufferResult.error()) << std::endl;
        //             LOGE("Failed to load GLTFAsset: %s", fullPath.c_str());
        //             return nullptr;
        //         }
        //         // 2. 配置解析器选项
        //         fastgltf::Parser parser;
        //         // 设置解析选项：加载 GLB 缓冲区数据，启用网格优化压缩解压
        //         auto options = fastgltf::Options::LoadGLBBuffers |
        //                        fastgltf::Options::DecomposeNodeMatrices;
        //         //  fastgltf::Options::LoadExternalBuffers | 不支持读取并加载外部 .bin 文件的内容
        //         auto &dataGetter = bufferResult.get();
        //         auto result = parser.loadGltf(dataGetter, fullPath, options);
        //         if (result.error() != fastgltf::Error::None)
        //         {
        //             LOGE("Failed to parse glTFAsset %d %s", result.error(), fullPath.c_str());
        //             return nullptr;
        //         }
        //         std::string name = std::filesystem::path(fullPath).stem().string();
        //         std::unique_ptr<fastgltf::Asset> m_asset = std::make_unique<fastgltf::Asset>(std::move(result.get()));

        //         GLTFAsset *glTFAsset = new GLTFAsset(assetPath, assetPath, name);
        //         glTFAsset->create(std::move(m_asset));
        //         return glTFAsset;
        // #elif defined(BOO_PLATFORM_ANDROID)
        //         return nullptr;
        // #endif
        return nullptr;
    }
    Asset *AssetTask::_createBMFontAsset(const std::string &assetPath)
    {
#if defined(BOO_PLATFORM_WINDOWS)
        std::string bmFontContent = "";
        game->getWindows()->loadText(assetPath, bmFontContent);
        if (bmFontContent.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        BMFontAsset *bmFontAsset = new BMFontAsset(assetPath, assetPath, name);
        bmFontAsset->create(bmFontContent);
        return bmFontAsset;
#elif defined(BOO_PLATFORM_MACOS)
        std::string bmFontContent = "";
        game->getMacOS()->loadText(assetPath, bmFontContent);
        if (bmFontContent.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        BMFontAsset *bmFontAsset = new BMFontAsset(assetPath, assetPath, name);
        bmFontAsset->create(bmFontContent);
        return bmFontAsset;
#elif defined(BOO_PLATFORM_ANDROID)
        std::string bmFontContent = "";
        game->getAndroid()->loadText(assetPath, bmFontContent);
        if (bmFontContent.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        BMFontAsset *bmFontAsset = new BMFontAsset(assetPath, assetPath, name);
        bmFontAsset->create(bmFontContent);
        return bmFontAsset;
#elif defined(BOO_PLATFORM_HARMONYOS)
        return nullptr;
#endif
        return nullptr;
    }
    Asset *AssetTask::_createAudioAsset(const std::string &assetPath)
    {
#if defined(BOO_PLATFORM_WINDOWS)
        std::vector<uint8_t> bytes;
        game->getWindows()->loadBinary(assetPath, bytes);
        if (bytes.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        AudioAsset *audioAsset = new AudioAsset(assetPath, assetPath, name);
        audioAsset->create(bytes);
        return audioAsset;
#elif defined(BOO_PLATFORM_MACOS)
        std::vector<uint8_t> bytes;
        game->getMacOS()->loadBinary(assetPath, bytes);
        if (bytes.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        AudioAsset *audioAsset = new AudioAsset(assetPath, assetPath, name);
        audioAsset->create(bytes);
        return audioAsset;
#elif defined(BOO_PLATFORM_ANDROID)
        std::vector<uint8_t> bytes;
        game->getAndroid()->loadBinary(assetPath, bytes);
        if (bytes.empty())
        {
            return nullptr;
        }
        std::string name = std::filesystem::path(assetPath).stem().string();
        AudioAsset *audioAsset = new AudioAsset(assetPath, assetPath, name);
        audioAsset->create(bytes);
        return audioAsset;
#elif defined(BOO_PLATFORM_HARMONYOS)
        return nullptr;
#endif
        return nullptr;
    }
    AssetTask::~AssetTask()
    {
    }
}
