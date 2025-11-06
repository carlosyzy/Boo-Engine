#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../libs/stb/stb_image.h"
#include "../gfx/gfx-mgr.h"
Texture::Texture(const std::string key, const std::string path) : Asset(key, path)
{
    this->_type = AssetType::Texture;
    this->_load();
}
void Texture::_load()
{
    // std::cout << "Loading texture: " << this->_key << " from path: " << this->_path << std::endl;
    const void *_pixels = stbi_load(this->_path.c_str(), &this->_width, &this->_height, &this->_channels, STBI_rgb_alpha);
    if (_pixels == nullptr)
    {
        std::cerr << "Failed to load texture: " << this->_path << std::endl;
        return;
    }
    this->_channels = 4;
    this->_pixelsVector = std::vector<uint8_t>(static_cast<const uint8_t *>(_pixels),
                                               static_cast<const uint8_t *>(_pixels) + (_width * _height * _channels));
    stbi_image_free((void *)_pixels);
    GfxMgr::getInstance()->createTexture(this->_key, this->_width, this->_height, this->_channels, &this->_pixelsVector);
}
void Texture::clearCache()
{
}
void Texture::destroy()
{
    GfxMgr::getInstance()->destroyTexture(this->_key);
}
