#include "gfx-texture.h"
#include "gfx.h"
#include "gfx-context.h"
#include "gfx-struct.h"
#include "gfx-mgr.h"

#include "../../libs/stb/stb_image_write.h"

/* // 主要用于创建附件贴图 */
GfxTexture::GfxTexture()
{
}
GfxTexture::GfxTexture( const std::vector<uint8_t> *pixels, uint32_t width, uint32_t height, uint32_t channels)
{
    this->_width = width;
    this->_height = height;
    this->_channels = channels;
    this->_imageSize = width * height * channels;
    this->_pixels = pixels;
    this->_createTextureImage();
    this->_createTextureImageView();
    this->_createTextureSampler();
    std::cout << "Gfx : Texture :: create success" << std::endl;
}

void GfxTexture::_createTextureImage()
{
    /* // 创建暂存缓冲区 */
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingBuffer,
        &stagingBufferMemory,
        this->_imageSize, nullptr);
    /* // 复制数据到暂存缓冲区 */
    void *data;
    vkMapMemory(Gfx::context->getVkDevice(), stagingBufferMemory, 0, this->_imageSize, 0, &data);
    memcpy(data, this->_pixels->data(), static_cast<size_t>(this->_imageSize));
    vkUnmapMemory(Gfx::context->getVkDevice(), stagingBufferMemory);
    /* // 计算格式 */
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    if (this->_channels == 1)
    {
        format = VK_FORMAT_R8_UNORM;
    }
   /*  // 创建纹理图像 */
    this->createImage(this->_width, this->_height, format, VK_IMAGE_TILING_OPTIMAL,
                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SAMPLE_COUNT_1_BIT,
                      this->_textureImage, this->_textureImageMemory);
    /* // 转换布局并复制数据
    // VK_IMAGE_LAYOUT_UNDEFINED 待确定 */
    this->_transitionImageLayout(this->_textureImage, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    this->_copyBufferToImage(stagingBuffer, this->_textureImage, static_cast<uint32_t>(this->_width), static_cast<uint32_t>(this->_height));
    this->_transitionImageLayout(this->_textureImage, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

   /*  // 清理暂存资源 */
    vkDestroyBuffer(Gfx::context->getVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(Gfx::context->getVkDevice(), stagingBufferMemory, nullptr);
}
void GfxTexture::_createTextureImageView()
{
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    if (this->_channels == 1)
    {
        format = VK_FORMAT_R8_UNORM;
    }
    this->createImageView(this->_textureImage, format, VK_IMAGE_ASPECT_COLOR_BIT, this->_textureImageView);
}
void GfxTexture::_createTextureSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR; /* // VK_FILTER_LINEAR */
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 16.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(Gfx::context->getVkDevice(), &samplerInfo, nullptr, &this->_textureSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
    // this->_Log("create texture sampler success...");
    
}
/* 
// 以下实现辅助方法... */
void GfxTexture::createImage(uint32_t width, uint32_t height, VkFormat format,
                             VkImageTiling tiling, VkImageUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples,
                             VkImage &image, VkDeviceMemory &imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = samples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = 0;

    if (vkCreateImage(Gfx::context->getVkDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Gfx::context->getVkDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = GfxMgr::getInstance()->getMemoryTypeIndex(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Gfx::context->getVkDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(Gfx::context->getVkDevice(), image, imageMemory, 0);
}
void GfxTexture::createImage(uint32_t width, uint32_t height, VkFormat format,
                             VkImageTiling tiling, VkImageUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples)
{
    this->createImage(width, height, format, tiling, usage, properties, samples, this->_textureImage, this->_textureImageMemory);
}

void GfxTexture::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView &imageView)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    if (format == VK_FORMAT_R8_UNORM)
    {
        /* // 单通道纹理的组件映射 */
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_R;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_R;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_ONE;
    }

    if (vkCreateImageView(Gfx::context->getVkDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }
}
void GfxTexture::createImageView(VkFormat format, VkImageAspectFlags aspectFlags)
{
    this->createImageView(this->_textureImage, format, aspectFlags, this->_textureImageView);
}

void GfxTexture::_transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
{

    VkCommandBuffer commandBuffer = this->_beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

    // 设置子资源范围
    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if (this->_hasStencilComponent(format))
        {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    /* // 设置管道阶段和访问掩码 */
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0,
                         0, nullptr, 0, nullptr, 1, &barrier);

    this->_endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer GfxTexture::_beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Gfx::context->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}
void GfxTexture::_endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Gfx::context->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Gfx::context->getGraphicsQueue());

    vkFreeCommandBuffers(Gfx::context->getVkDevice(), Gfx::context->getCommandPool(), 1, &commandBuffer);
}
bool GfxTexture::_hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}
void GfxTexture::_copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = this->_beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    this->_endSingleTimeCommands(commandBuffer);
}

VkImageView GfxTexture::getImageView()
{
    return this->_textureImageView;
}
VkSampler GfxTexture::getSampler()
{
    return this->_textureSampler;
}

// 绑定less 索引
uint32_t GfxTexture::getBindlessIndex()
{
    return this->_bindlessIndex;
}
void GfxTexture::setBindlessIndex(uint32_t index)
{
    this->_bindlessIndex = index;
}

bool GfxTexture::saveToFile(std::string filePath, uint32_t width, uint32_t height)
{
    /* // 1. 创建 staging buffer 用于从 GPU 复制数据到 CPU */
    VkDeviceSize imageSize = width * height * 4; /* // RGBA 格式 */
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingBuffer,
        &stagingBufferMemory,
        imageSize, nullptr);

    /* // 2. 开始命令缓冲区 */
    VkCommandBuffer commandBuffer = this->_beginSingleTimeCommands();

    /* // 3. 将图像布局从 SHADER_READ_ONLY_OPTIMAL 转换到 TRANSFER_SRC_OPTIMAL */
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = this->_textureImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    /* // 4. 复制图像到 staging buffer */
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyImageToBuffer(
        commandBuffer,
        this->_textureImage,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        stagingBuffer,
        1,
        &region);

    /* // 5. 将布局转换回 SHADER_READ_ONLY_OPTIMAL */
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    this->_endSingleTimeCommands(commandBuffer);

    /* // 6. 映射内存并读取数据 */
    void* data;
    vkMapMemory(Gfx::context->getVkDevice(), stagingBufferMemory, 0, imageSize, 0, &data);

    /* // 7. 保存为图片文件 */
    bool success = false;
    std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

    // /* // 使用 stb_image_write 保存 */
    // #define STB_IMAGE_WRITE_IMPLEMENTATION
    // #include <stb_image_write.h>

    if (extension == "png" || extension == "PNG")
    {
        success = stbi_write_png(filePath.c_str(), width, height, 4, data, width * 4);
    }
    else if (extension == "jpg" || extension == "JPG" || extension == "jpeg" || extension == "JPEG")
    {
        success = stbi_write_jpg(filePath.c_str(), width, height, 4, data, 90);
    }
    else if (extension == "bmp" || extension == "BMP")
    {
        success = stbi_write_bmp(filePath.c_str(), width, height, 4, data);
    }
    else
    {
        std::cerr << "Unsupported image format: " << extension << std::endl;
    }

    /* // 8. 解除内存映射 */
    vkUnmapMemory(Gfx::context->getVkDevice(), stagingBufferMemory);

    /* // 9. 清理 staging buffer */
    vkDestroyBuffer(Gfx::context->getVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(Gfx::context->getVkDevice(), stagingBufferMemory, nullptr);

    if (success)
    {
        std::cout << "GfxTexture: Saved to " << filePath << std::endl;
    }
    else
    {
        std::cerr << "GfxTexture: Failed to save to " << filePath << std::endl;
    }

    return success;
}

GfxTexture::~GfxTexture()
{
   /*  // 销毁采样器 */
    if (this->_textureSampler != VK_NULL_HANDLE)
    {
        vkDestroySampler(Gfx::context->getVkDevice(), this->_textureSampler, nullptr);
        this->_textureSampler = VK_NULL_HANDLE;
    }

    /* // 销毁图像视图 */
    if (this->_textureImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(Gfx::context->getVkDevice(), this->_textureImageView, nullptr);
        this->_textureImageView = VK_NULL_HANDLE;
    }

   /*  // 销毁图像 */
    if (this->_textureImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(Gfx::context->getVkDevice(), this->_textureImage, nullptr);
        this->_textureImage = VK_NULL_HANDLE;
    }

   /*  // 释放图像内存 */
    if (this->_textureImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->getVkDevice(), this->_textureImageMemory, nullptr);
        this->_textureImageMemory = VK_NULL_HANDLE;
    }
}
