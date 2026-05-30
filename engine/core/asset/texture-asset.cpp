#include "texture-asset.h"
#include "boo.h"
#include "core/gfx/gfx-manager.h"
#include "libs/stb/stb_image.h"
namespace Boo
{
  TextureAsset::TextureAsset() : Asset(),
                                 _width(0),
                                 _height(0),
                                 _channels(0)
  {
    this->_type = EAssetType::Texture;
  }
  TextureAsset::TextureAsset(std::string uuid) : Asset(uuid),
                                                 _width(0),
                                                 _height(0),
                                                 _channels(0)
  {
    this->_type = EAssetType::Texture;
    this->_textureType = ETextureType::Static;
  }
  TextureAsset::TextureAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name),
                                                                                     _width(0),
                                                                                     _height(0),
                                                                                     _channels(0)
  {
    this->_type = EAssetType::Texture;
    this->_textureType = ETextureType::Static;
  }
  void TextureAsset::create(int width, int height, int channels, std::vector<uint8_t> pixelsVector, GfxTextureFormat format)
  {
    this->_width = width;
    this->_height = height;
    this->_channels = channels;
    this->_pixelsVector = pixelsVector;
    this->gfxTexture = GfxManager::getInstance()->createTexture(this->_uuid, this->_width, this->_height, this->_channels, int(this->_textureType), &this->_pixelsVector, format);
    // GPU 上传完成后释放 CPU 端数据
    this->_pixelsVector.clear();
    this->_pixelsVector.shrink_to_fit();
  }
  void TextureAsset::createDynamic(int width, int height, int channels, std::vector<uint8_t> pixelsVector, GfxTextureFormat format)
  {
    this->_textureType = ETextureType::Dynamic;
    this->create(width, height, channels, pixelsVector, format);
  }
  void TextureAsset::create(const unsigned char *data, size_t size)
  {
    const void *_pixels = stbi_load_from_memory(
        data, size, &this->_width, &this->_height, nullptr, STBI_rgb_alpha);
    if (_pixels == nullptr)
    {
      std::cerr << "Failed to load TextureAsset from memory" << std::endl;
      return;
    }
    this->_channels = 4;
    this->_pixelsVector = std::vector<uint8_t>(
        static_cast<const uint8_t *>(_pixels),
        static_cast<const uint8_t *>(_pixels) + (this->_width * this->_height * this->_channels));
    stbi_image_free((void *)_pixels);
    this->gfxTexture = GfxManager::getInstance()->createTexture(this->_uuid, this->_width, this->_height, this->_channels, int(this->_textureType), &this->_pixelsVector, GfxTextureFormat::R8G8B8A8_SRGB);
    // GPU 上传完成后释放 CPU 端数据
    this->_pixelsVector.clear();
    this->_pixelsVector.shrink_to_fit();
  }
  GfxTexture *TextureAsset::getGfxTexture()
  {
    return this->gfxTexture;
  }
  void TextureAsset::destroy()
  {
    GfxManager::getInstance()->destroyTexture(this->gfxTexture);
    this->gfxTexture = nullptr;
  }

} // namespace Boo