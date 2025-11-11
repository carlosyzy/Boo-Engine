#include "shader.h"
#include <filesystem>
#include "../gfx/gfx-mgr.h"
Shader::Shader(const std::string uuid, const std::string path) : Asset(uuid, path)
{
    this->_shaderFormat = ShaderFormat::None;
}
void Shader::_load()
{
}
/**
 * @brief 加载GLSL文件
 * 着色器文件路径必须以.vert或.frag结尾
 */
void Shader::loadGlsl()
{
    this->_shaderFormat = ShaderFormat::GLSL;
    std::string extension = std::filesystem::path(this->_path).extension().string();
    if (extension == ".vert")
    {
        this->_glslType = "Vert";
    }
    else if (extension == ".frag")
    {
        this->_glslType = "Frag";
    }
    std::ifstream file(this->_path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << this->_path << std::endl;
        return;
    }
    // tellg()返回当前定位指针的位置，也代表着输入流的大小。
    size_t fileSize = static_cast<size_t>(file.tellg());
    // 创建缓冲区并读取文件
    this->_glslData.resize(fileSize);
    file.seekg(0);
    file.read(this->_glslData.data(), fileSize);
    file.close();
    this->createGfxShader({});
}
/**
 * @brief 加载SPIR-V文件
 * 着色器文件路径必须以.spv结尾
 */
void Shader::loadSpv()
{
    this->_shaderFormat = ShaderFormat::SPIRV;
    /*   // ate: 表示从文件末未开始读取
    // binary: 表示以二进制方式读取 */
    std::ifstream file(this->_path, std::ios::ate | std::ios::binary);
    /*    // std::cout << filename << std::endl; */
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }
    /*  // tellg()返回当前定位指针的位置，也代表着输入流的大小。 */
    size_t fileSize = (size_t)file.tellg();
    /*  // std::cout << fileSize << std::endl; */
    this->_spirvData.resize(fileSize);
    /*  // seekg()是对输入流的操作 g是get缩写，0是代表从开头读起。 */
    file.seekg(0);
    /* // 读入到Buffer当中 */
    file.read(this->_spirvData.data(), fileSize);
    file.close();
    // this->createGfxSpirvShader();
    this->createGfxShader({});
}
/**
 * @brief 创建图形资源
 * 参数 宏定义
 */
void Shader::createGfxShader(const std::map<std::string, std::string> &macros)
{
    if (this->_shaderFormat == ShaderFormat::GLSL)
    {
        GfxMgr::getInstance()->createGlslShader(this->_uuid, this->_glslType, this->_glslData, macros);
    }
    else if (this->_shaderFormat == ShaderFormat::SPIRV)
    {
        GfxMgr::getInstance()->createSpirvShader(this->_uuid, this->_spirvData);
    }
}

// void Shader::createGfxGlslShader(const std::map<std::string, std::string> &macros)
// {
//     GfxMgr::getInstance()->createGlslShader(this->_uuid, this->_glslType, this->_glslData, macros);
// }
// void Shader::createGfxSpirvShader()
// {
//     GfxMgr::getInstance()->createSpirvShader(this->_uuid, this->_spirvData);
// }

void Shader::destroy()
{
}
