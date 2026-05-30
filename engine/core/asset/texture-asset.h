#pragma once
#include "asset.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "core/gfx/base/gfx-texture.h"

namespace Boo
{
  class TextureAsset : public Asset
  {
  private:
    ETextureType _textureType;
    int _width = 0;
    int _height = 0;
    int _channels = 0;
    std::vector<uint8_t> _pixelsVector;
    GfxTexture *gfxTexture = nullptr;

  protected:
  public:
    TextureAsset();
    TextureAsset(std::string uuid);
    TextureAsset(std::string uuid, std::string path, std::string name);
    /**
     * @brief 创建纹理资产
     * @param width 纹理宽度
     * @param height 纹理高度
     * @param channels 纹理通道数
     * @param textureMode 纹理模式
     * @param pixelsVector 像素数据
     * @param format 纹理格式
     */
    void create(int width, int height, int channels,std::vector<uint8_t> pixelsVector, GfxTextureFormat format);
    void createDynamic(int width, int height, int channels,std::vector<uint8_t> pixelsVector, GfxTextureFormat format);
    /**
     * @brief 创建纹理资产
     * @param pixels 像素数据
     * @param size 像素数据大小
     */
    void create(const unsigned char *data, size_t size);

    /**
     * @brief 获取纹理宽度
     *
     * @return int
     */
    const int getWidth() const { return _width; }
    /**
     * @brief 获取纹理高度
     *
     * @return int
     */
    const int getHeight() const { return _height; }
    /**
     * @brief 获取纹理通道数
     *
     * @return int
     */
    int channels() const { return _channels; }

    /**
     * @brief 获取 GfxTexture 资产
     *
     * @return GfxTexture*
     */
    GfxTexture *getGfxTexture();
    /**
     * @brief 销毁纹理资产
     *
     */
    void destroy() override;
    ~TextureAsset() {}
  };

} // namespace Boo
