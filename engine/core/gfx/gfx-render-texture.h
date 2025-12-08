#pragma once
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>

class GfxTexture;

class GfxRenderTexture
{
private:
    uint32_t _width;
    uint32_t _height;

    GfxTexture *_colorTexture;
    GfxTexture *_depthTexture;

    VkFramebuffer _framebuffer;
    VkCommandBuffer _commandBuffer;


    void _createTextures();
    void _createFramebuffer();
    void _createCommandBuffer();

    // void _cleanTextures();
    // void _cleanFramebuffer();

public:
    GfxRenderTexture();
    void resize(uint32_t width, uint32_t height);

    VkFramebuffer getFramebuffer() { return _framebuffer; }
    VkCommandBuffer getCommandBuffer() { return _commandBuffer; }

    bool saveToFile1(std::string filePath);

    ~GfxRenderTexture();
};