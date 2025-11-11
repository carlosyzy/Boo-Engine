#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
class GfxContext;

class GfxShader
{
private:
    GfxContext *_context;
    std::string _name;
    VkShaderModule _shaderModule;

public:
    GfxShader(GfxContext *context, const std::string &name);
    void createShaderModule(const std::vector<uint32_t> &code);
    void createShaderModule(const std::vector<char> &code);
    VkShaderModule getShaderModule()
    {
        return this->_shaderModule;
    }
    ~GfxShader();
};
