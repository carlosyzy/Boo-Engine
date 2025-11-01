
#include "gfx-shader.h"
#include "gfx-mgr.h"
#include "gfx-context.h"

/**
 * shader
 * vert, frag, geom的统称
 * 一个shader代表了vert, frag, geom中的其中一个
 */
GfxShader::GfxShader(GfxContext *context, std::string path)
{
    this->_context = context;
    this->_path = path;
    try
    {
        std::vector<char> vertexShaderCode = GfxMgr::getInstance()->readShaderFile(this->_path);
        this->_shaderModule = this->_createShaderModule(vertexShaderCode);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        if (this->_shaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(this->_context->getVkDevice(), this->_shaderModule, nullptr);
            this->_shaderModule = VK_NULL_HANDLE;
        }
    }
}

// 要将着色器字节码在管线上使用，还需要使用VkShaderModule转换
VkShaderModule GfxShader::_createShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    // std::cout << "shader code size: " << code.size() << "\n";

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(this->_context->getVkDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }

    return shaderModule;
}
GfxShader::~GfxShader()
{
    if (this->_shaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(this->_context->getVkDevice(), this->_shaderModule, nullptr);
        this->_shaderModule = VK_NULL_HANDLE;
    }
}