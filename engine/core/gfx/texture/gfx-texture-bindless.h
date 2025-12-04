#pragma once
#include <vulkan/vulkan_core.h>
class GfxContext;
class GfxTexture;
class GfxTextureBindless
{
private:
    GfxContext *_context;
    uint32_t _nextIndex = 0;
    uint32_t _maxTextures = 500; //池子大小

    VkSampler _textureSampler = VK_NULL_HANDLE;
    void _initSampler();

    void _addTexture(GfxTexture *texture);
protected:
public:
    GfxTextureBindless(GfxContext *context);
    ~GfxTextureBindless();
};
