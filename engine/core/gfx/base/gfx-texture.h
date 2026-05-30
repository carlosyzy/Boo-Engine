#pragma once
#include <string>
// #include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include "../gfx-struct.h"

class GfxTexture
{
private:
    std::string _uuid;
    uint32_t _width;
    uint32_t _height;
    uint32_t _channels;
    int _textureType = 0;
    VkFormat _format;
    std::vector<uint8_t> _pixels;
    int _changeFlag = 0;

    VkImage _textureImage = VK_NULL_HANDLE;
    VkDeviceMemory _textureImageMemory = VK_NULL_HANDLE;
    VkImageView _textureImageView = VK_NULL_HANDLE;

    // 绑定less 采样器
    // 后续更具不同类型,添加到采样池中,不需要每个纹理都创建一个采样器
    VkSampler _textureSampler = VK_NULL_HANDLE;

    // 绑定less 索引
    uint32_t _bindlessIndex = 0;

    void _updateTextureData();
    void _transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1);
    VkCommandBuffer _beginSingleTimeCommands();
    void _endSingleTimeCommands(VkCommandBuffer commandBuffer);
    bool _hasStencilComponent(VkFormat format);
    void _copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void _clearPixels();
public:
    /**
     * @brief 创建纹理
     *
     * @�理uuid
     */
    GfxTexture(std::string uuid, uint32_t width, uint32_t height, uint32_t channels, int textureType, VkFormat format);
    /**
     * @brief 创建纹理
     *
     * @param pixels 纹理数据
     * @param width 纹理宽度
     * @param height 纹理高度
     * @param channels 纹理通道数
     * @param format 纹理格式
     */
    void create(const std::vector<uint8_t> *pixels);
    void createImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples);
    void createImageView(VkImageAspectFlags aspectFlags);
    void crateImageSampler();

    void setPixel(uint32_t x, uint32_t y, std::vector<uint8_t> pixel);
    void updateData();

    VkImage getImage();
    VkImageView getImageView();
    VkSampler getSampler();
    std::string getUuid();
    void destroy();
    bool saveToFile(std::string filePath);
    ~GfxTexture();
};

// // 创建RGBA纹理
// auto colorTexture = std::make_shared<Texture>(context, 512, 512, VK_FORMAT_R8G8B8A8_UNORM);
// colorTexture->uploadData(colorData);

// // 创建深度纹理
// auto depthTexture = std::make_shared<Texture>(context, 1024, 1024, VK_FORMAT_D32_SFLOAT);
// // 对于深度纹理，不需要上传数据，只需要初始化 */