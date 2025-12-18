#include "texture-asset.h"
#include "../../libs/stb/stb_image.h"
#include "../gfx/gfx-mgr.h"

TextureAsset::TextureAsset() : Asset()
{
  this->_type = AssetType::Texture;
}
TextureAsset::TextureAsset(const std::string uuid) : Asset(uuid)
{
  this->_type = AssetType::Texture;
}
void TextureAsset::create(std::string path)
{
  Asset::create(path);
  const void *_pixels =
      stbi_load(this->_path.c_str(), &this->_width, &this->_height,
                nullptr, STBI_rgb_alpha);
  if (_pixels == nullptr)
  {
    std::cerr << "Failed to load TextureAsset: " << this->_path << std::endl;
    return;
  }
  this->_channels = 4;
  this->_pixelsVector = std::vector<uint8_t>(
      static_cast<const uint8_t *>(_pixels),
      static_cast<const uint8_t *>(_pixels) + (_width * _height * _channels));
  stbi_image_free((void *)_pixels);
  std::cout << "TextureAsset::create: width=" << this->_width << ",height=" << this->_height << ",channels=" << this->_channels << std::endl;
  GfxMgr::getInstance()->createTexture(this->_uuid, this->_width, this->_height,
                                       this->_channels, &this->_pixelsVector);
}

void TextureAsset::create(int width, int height, int channels,
                          std::vector<uint8_t> pixelsVector)
{
  this->_width = width;
  this->_height = height;
  this->_channels = channels;
  this->_pixelsVector = pixelsVector;
  GfxMgr::getInstance()->createTexture(this->_uuid, this->_width, this->_height,
                                       this->_channels, &this->_pixelsVector);
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
  // std::cout << "TextureAsset::create from memory " << this->_width << "," << this->_height << "," << this->_channels << std::endl;
  GfxMgr::getInstance()->createTexture(this->_uuid, this->_width, this->_height,
                                       this->_channels, &this->_pixelsVector);
}
void TextureAsset::destroy()
{
  GfxMgr::getInstance()->destroyTexture(this->_uuid);
}
// TextureAsset::TextureAsset(const std::string uuid, const std::string path)
//     : Asset(uuid, path) {
//   this->_type = AssetType::Texture;
//   this->_load();
// }
// TextureAsset::TextureAsset(const std::string uuid, const unsigned char *data,
//                            const size_t size)
//     : Asset(uuid, "") {
//   this->_type = AssetType::Texture;
//   stbi_load_from_memory(data, size, &this->_width, &this->_height,
//                         &this->_channels, 0);
// }
// TextureAsset::TextureAsset(const std::string uuid, int width, int height,
//                            int channels, std::vector<uint8_t> pixels)
//     : Asset(uuid) {
//   this->_type = AssetType::Texture;
//   this->_width = width;
//   this->_height = height;
//   this->_channels = channels;
//   this->_pixelsVector = pixels;
//   this->createGfxTexture();
// }

// void TextureAsset::_load() {
//   const void *_pixels =
//       stbi_load(this->_path.c_str(), &this->_width, &this->_height,
//                 &this->_channels, STBI_rgb_alpha);
//   if (_pixels == nullptr) {
//     std::cerr << "Failed to load TextureAsset: " << this->_path << std::endl;
//     return;
//   }
//   this->_channels = 4;
//   this->_pixelsVector = std::vector<uint8_t>(
//       static_cast<const uint8_t *>(_pixels),
//       static_cast<const uint8_t *>(_pixels) + (_width * _height *
//       _channels));
//   stbi_image_free((void *)_pixels);
//   this->createGfxTexture();
// }
// void TextureAsset::createGfxTexture() {
//   GfxMgr::getInstance()->createTexture(this->_uuid, this->_width,
//   this->_height,
//                                        this->_channels,
//                                        &this->_pixelsVector);
// }
