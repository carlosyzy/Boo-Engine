#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <functional>
#include <vulkan/vulkan_core.h>

class GfxTexture;

class GfxRenderTexture
{
private:
    uint32_t _width;
    uint32_t _height;

    std::string _colorUuid;
    std::string _depthUuid;
    GfxTexture *_colorTexture;
    GfxTexture *_depthTexture;

    VkFramebuffer _framebuffer;
    VkCommandBuffer _commandBuffer;

    std::string _generateUUID();
    void _createTextures();
    void _createFramebuffer();
    void _createCommandBuffer();

    // void _cleanTextures();
    // void _cleanFramebuffer();

public:
    GfxRenderTexture();
    VkFramebuffer getFramebuffer() { return _framebuffer; }
    VkCommandBuffer getCommandBuffer() { return _commandBuffer; }
    uint32_t getWidth() { return _width; }
    uint32_t getHeight() { return _height; }

    void resize(uint32_t width, uint32_t height);
    void _clear();
    void _reset();

    bool saveToFile1(std::string filePath);

    ~GfxRenderTexture();
};