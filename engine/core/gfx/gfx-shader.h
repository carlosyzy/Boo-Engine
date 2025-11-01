#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
class GfxContext;

class GfxShader
{
private:
    GfxContext *_context;
    std::string _path;
    VkShaderModule _shaderModule;

    VkShaderModule _createShaderModule(const std::vector<char> &code);
public:
    GfxShader(GfxContext *context,std::string path);
    VkShaderModule getShaderModule(){
        return this->_shaderModule;
    }
    ~GfxShader();
};
