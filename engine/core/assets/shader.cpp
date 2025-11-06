#include "shader.h"
Shader::Shader(const std::string key, const std::string path) : Asset(key, path)
{
    this->_type = AssetType::Texture;
    this->_load();
}
void Shader::_load()
{
    std::ifstream file(this->_path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << this->_path << std::endl;
        return;
    }
    // tellg()返回当前定位指针的位置，也代表着输入流的大小。
    size_t fileSize = static_cast<size_t>(file.tellg());
    // 创建缓冲区并读取文件
    this->_data.resize(fileSize);
    file.seekg(0);
    file.read(this->_data.data(), fileSize);
    file.close();
    // std::cout << "Loaded shader: " << this->_key << " from path: " << this->_data << std::endl;
    // GfxMgr::getInstance()->createTexture(this->_key, this->_width, this->_height, this->_channels, &this->_pixelsVector);
}
void Shader::destroy()
{
}
