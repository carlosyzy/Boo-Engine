#include "shader.h"
#include <filesystem>
#include "../gfx/gfx-mgr.h"
Shader::Shader(const std::string uuid, const std::string path) : Asset(uuid, path)
{
    std::string extension = std::filesystem::path(path).extension().string();
    if (extension == ".vert" )
    {
        this->_shaderType = "Vert";
    }
    else if (extension == ".frag") {
        this->_shaderType = "Frag";
    }
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
    this->_shaderData.resize(fileSize);
    file.seekg(0);
    file.read(this->_shaderData.data(), fileSize);
    file.close();
    //std::cout << "Loaded shader: " << this->_key << " from path: " << this->_shaderData << std::endl;
    std::map<std::string, std::string> no;
    this->createGfxShader(no);
}
void Shader::createGfxShader(const std::map<std::string, std::string> &macros)
{
    GfxMgr::getInstance()->createShader(this->_uuid, this->_shaderType, this->_shaderData, macros);
}
void Shader::destroy()
{
}
