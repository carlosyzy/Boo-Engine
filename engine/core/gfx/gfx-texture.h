#pragma once
#include <string>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>

class GfxContext;

class GfxTexture
{
private:
    GfxContext *_context;

    std::string _path;
    int _width;
    int _height;
    int _channels;
    VkDeviceSize _imageSize;
    const std::vector<uint8_t> *_pixels;

    VkImage _textureImage = VK_NULL_HANDLE;
    VkDeviceMemory _textureImageMemory = VK_NULL_HANDLE;
    VkImageView _textureImageView = VK_NULL_HANDLE;
    VkSampler _textureSampler = VK_NULL_HANDLE;

    void _createTextureImage();
    void _createTextureImageView();
    void _createTextureSampler();

    void _transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1);
    VkCommandBuffer _beginSingleTimeCommands();
    void _endSingleTimeCommands(VkCommandBuffer commandBuffer);
    bool _hasStencilComponent(VkFormat format);
    void _copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void _Log(std::string msg);

public:
   /*  // 主要用于创建附件贴图 */
    GfxTexture(GfxContext *context);
   /*  // 多通道贴图创建 */
    GfxTexture(GfxContext *context, const std::vector<uint8_t> *pixels, uint32_t width, uint32_t height, uint32_t channels);

    void createImage(uint32_t width, uint32_t height, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples,
                     VkImage &image, VkDeviceMemory &imageMemory);
    void createImage(uint32_t width, uint32_t height, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples);
    void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView &imageView);
    void createImageView(VkFormat format, VkImageAspectFlags aspectFlags);

    VkImageView getImageView();
    VkSampler getSampler();
    ~GfxTexture();
};

/* // // 创建单通道字体纹理
// auto fontTexture = std::make_shared<Texture>(context, 256, 256, VK_FORMAT_R8_UNORM);
// fontTexture->uploadData(fontData);

// // 创建RGBA纹理
// auto colorTexture = std::make_shared<Texture>(context, 512, 512, VK_FORMAT_R8G8B8A8_UNORM);
// colorTexture->uploadData(colorData);

// // 创建深度纹理
// auto depthTexture = std::make_shared<Texture>(context, 1024, 1024, VK_FORMAT_D32_SFLOAT);
// // 对于深度纹理，不需要上传数据，只需要初始化 */