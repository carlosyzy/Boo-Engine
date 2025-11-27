#include "texture-asset.h"
#include "../../libs/stb/stb_image.h"
#include "../gfx/gfx-mgr.h"
TextureAsset::TextureAsset(const std::string uuid, const std::string path) : Asset(uuid, path)
{
    this->_type = AssetType::Texture;
    this->_load();
}
TextureAsset::TextureAsset(const std::string uuid, int width, int height, int channels, std::vector<uint8_t> pixels) : Asset(uuid)
{
    this->_type = AssetType::Texture;
    this->_width = width;
    this->_height = height;
    this->_channels = channels;
    this->_pixelsVector = pixels;
    this->createGfxTexture();
}

void TextureAsset::_load()
{
    // std::cout << "Loading TextureAsset: " << this->_uuid << " from path: " << this->_path << std::endl;
    const void *_pixels = stbi_load(this->_path.c_str(), &this->_width, &this->_height, &this->_channels, STBI_rgb_alpha);
    if (_pixels == nullptr)
    {
        std::cerr << "Failed to load TextureAsset: " << this->_path << std::endl;
        return;
    }
    this->_channels = 4;
    this->_pixelsVector = std::vector<uint8_t>(static_cast<const uint8_t *>(_pixels),
                                               static_cast<const uint8_t *>(_pixels) + (_width * _height * _channels));
    stbi_image_free((void *)_pixels);
    this->createGfxTexture();
}
void TextureAsset::createGfxTexture()
{
    GfxMgr::getInstance()->createTexture(this->_uuid, this->_width, this->_height, this->_channels, &this->_pixelsVector);
}
void TextureAsset::clearCache()
{
}
void TextureAsset::destroy()
{
    GfxMgr::getInstance()->destroyTexture(this->_uuid);
}
