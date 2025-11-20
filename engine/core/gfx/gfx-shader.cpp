
#include "gfx-shader.h"
#include "gfx-mgr.h"
#include "gfx-context.h"

GfxShader::GfxShader(GfxContext *context, const std::string &name): _context(context), _name(name)
{
}

// 要将着色器字节码在管线上使用，还需要使用VkShaderModule转换
void GfxShader::createShaderModule(const std::vector<uint32_t> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();
    /*VkShaderModule shaderModule;*/
    if (vkCreateShaderModule(this->_context->getVkDevice(), &createInfo, nullptr, &this->_shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }
    std::cout << "GfxShader: create shader module success " << this->_name << std::endl;
}

void GfxShader::createShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
    if (vkCreateShaderModule(this->_context->getVkDevice(), &createInfo, nullptr, &this->_shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }
    std::cout << "GfxShader: create shader module success " << this->_name << std::endl;
}
void GfxShader::createShaderModule(const uint32_t *data, const uint32_t size)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = size;
    createInfo.pCode = data;
    if (vkCreateShaderModule(this->_context->getVkDevice(), &createInfo, nullptr, &this->_shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }
    // std::cout << "Shader module created: " << this->_name << std::endl;
    std::cout << "GfxShader: create shader module success " << this->_name << std::endl;
}



GfxShader::~GfxShader()
{
    if (this->_shaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(this->_context->getVkDevice(), this->_shaderModule, nullptr);
        this->_shaderModule = VK_NULL_HANDLE;
    }
}