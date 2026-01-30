#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

class GfxShader
{
private:
    std::string _name;
    VkShaderModule _shaderModule;

public:
    GfxShader( const std::string &name);
    void createShaderModule(const std::vector<uint32_t> &code);
    // void createShaderModule(const std::vector<char> &code);
    // void createShaderModule(const uint32_t *data, const uint32_t size);
    VkShaderModule getShaderModule()
    {
        return this->_shaderModule;
    }
    void destroy();
    ~GfxShader();
};
