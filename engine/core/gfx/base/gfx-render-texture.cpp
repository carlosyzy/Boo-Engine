#include "gfx-render-texture.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-renderer.h"
#include "gfx-texture.h"
#include "gfx-render-pass.h"
#include "gfx-pipeline.h"

GfxRenderTexture::GfxRenderTexture(std::string uuid)
{
    this->_uuid = uuid;
    this->_colorUuid = uuid + "_color";
    this->_depthUuid = uuid + "_depth";
    this->_width = 0;
    this->_height = 0;
    this->_colorTexture = nullptr;
    this->_depthTexture = nullptr;
    this->_pass = nullptr;
}
void GfxRenderTexture::bindRenderPass(GfxRenderPass *pass)
{
    if (pass == nullptr)
    {
        std::cout << "[Gfx : GfxRenderTexture]::bindRenderPass: pass is null" << std::endl;
        return;
    }
    this->_pass = pass;
    if (this->_colorTexture == nullptr || this->_depthTexture == nullptr)
    {
        std::cout << "[Gfx : GfxRenderTexture]::bindRenderPass: colorTexture or depthTexture is null" << std::endl;
        return;
    }
    this->_createFrameBuffer();
    this->_createCommandBuffer();
}
void GfxRenderTexture::_createFrameBuffer()
{
    if (this->_framebuffer)
    {
        vkDestroyFramebuffer(Gfx::context->getVkDevice(), this->_framebuffer, nullptr);
        this->_framebuffer = VK_NULL_HANDLE;
    }
    std::vector<VkImageView> attachments = {
        this->_colorTexture->getImageView(),
        this->_depthTexture->getImageView()};
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = this->_pass->getVKRenderPass();
    framebufferInfo.attachmentCount = attachments.size();
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = this->_width;
    framebufferInfo.height = this->_height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(Gfx::context->getVkDevice(), &framebufferInfo, nullptr, &this->_framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create framebuffer!");
    }
    std::cout << "[Gfx : GfxRenderTexture]:: create framebuffer success" << std::endl;
}
void GfxRenderTexture::_createCommandBuffer()
{
    if (this->_commandBuffer)
    {
        vkFreeCommandBuffers(Gfx::context->getVkDevice(), Gfx::context->getCommandPool(), 1, &this->_commandBuffer);
        this->_commandBuffer = VK_NULL_HANDLE;
    }
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = Gfx::context->getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    if (vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, &this->_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
    std::cout << "[Gfx : GfxRenderTexture]:: create commandbuffer success" << std::endl;
}
VkFramebuffer &GfxRenderTexture::getFramebuffer()
{
    return this->_framebuffer;
}
VkCommandBuffer &GfxRenderTexture::getCommandBuffer()
{
    return this->_commandBuffer;
}

void GfxRenderTexture::resize(uint32_t width, uint32_t height)
{
    std::cout << "[Gfx : GfxRenderTexture]::resize: width:" << width << " height:" << height << std::endl;
    this->_width = width;
    this->_height = height;
    this->_createTextures();
    if (this->_pass != nullptr)
    {
        this->_createFrameBuffer();
        this->_createCommandBuffer();
    }
}
const uint32_t &GfxRenderTexture::getWidth() const
{
    return this->_width;
}
const uint32_t &GfxRenderTexture::getHeight() const
{
    return this->_height;
}
/**
 * @brief 创建渲染纹理的颜色贴图和深度贴图
 */
void GfxRenderTexture::_createTextures()
{
    this->_clear();
    this->_colorTexture = new GfxTexture(this->_colorUuid);
    this->_colorTexture->createImage(
        this->_width, this->_height,
        Gfx::context->getSwapChainImageFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, /* // 可作为纹理采样 */
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_SAMPLE_COUNT_1_BIT);
    this->_colorTexture->createImageView(
        Gfx::context->getSwapChainImageFormat(),
        VK_IMAGE_ASPECT_COLOR_BIT);
    this->_colorTexture->crateImageSampler();

    this->_depthTexture = new GfxTexture(this->_depthUuid);
    this->_depthTexture->createImage(
        this->_width, this->_height,
        VK_FORMAT_D32_SFLOAT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_SAMPLE_COUNT_1_BIT);
    this->_depthTexture->createImageView(
        VK_FORMAT_D32_SFLOAT,
        VK_IMAGE_ASPECT_DEPTH_BIT);
    this->_depthTexture->crateImageSampler();
    Gfx::renderer->insertTexture(this->_colorUuid, this->_colorTexture);
    Gfx::renderer->insertTexture(this->_depthUuid, this->_depthTexture);

    // ✅ 初始化纹理：转换布局并清除为透明黑色
    this->_initializeRenderTexture();

    std::cout << "[Gfx : GfxRenderTexture]:: create textures success" << std::endl;
}

/**
 * @brief 初始化渲染纹理：清除为透明黑色
 */
void GfxRenderTexture::_initializeRenderTexture()
{
    // 创建一次性命令缓冲区
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Gfx::context->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, &commandBuffer);

    // 开始记录命令
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // 1. 转换颜色纹理布局: UNDEFINED -> TRANSFER_DST_OPTIMAL
    VkImageMemoryBarrier barrier1{};
    barrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier1.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier1.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier1.image = this->_colorTexture->getImage();
    barrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier1.subresourceRange.baseMipLevel = 0;
    barrier1.subresourceRange.levelCount = 1;
    barrier1.subresourceRange.baseArrayLayer = 0;
    barrier1.subresourceRange.layerCount = 1;
    barrier1.srcAccessMask = 0;
    barrier1.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    vkCmdPipelineBarrier(commandBuffer,
                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &barrier1);

    // 2. 清除颜色纹理为透明黑色
    VkClearColorValue clearColor = {{0.0f, 0.0f, 0.0f, 0.0f}};
    VkImageSubresourceRange range{};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;

    vkCmdClearColorImage(commandBuffer, this->_colorTexture->getImage(),
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &range);

    // 3. 转换颜色纹理布局: TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
    VkImageMemoryBarrier barrier2{};
    barrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier2.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier2.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier2.image = this->_colorTexture->getImage();
    barrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier2.subresourceRange.baseMipLevel = 0;
    barrier2.subresourceRange.levelCount = 1;
    barrier2.subresourceRange.baseArrayLayer = 0;
    barrier2.subresourceRange.layerCount = 1;
    barrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier2.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &barrier2);

    // 结束命令记录
    vkEndCommandBuffer(commandBuffer);

    // 提交并等待完成
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Gfx::context->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Gfx::context->getGraphicsQueue());

    // 清理命令缓冲区
    vkFreeCommandBuffers(Gfx::context->getVkDevice(), Gfx::context->getCommandPool(), 1, &commandBuffer);

    std::cout << "[Gfx : GfxRenderTexture]:: initialized render texture (cleared to transparent)" << std::endl;
}

const std::string &GfxRenderTexture::getColorTextureUuid() const
{
    if (this->_colorTexture == nullptr)
    {
        return "";
    }
    if (this->_colorTexture->getImageView() == VK_NULL_HANDLE || this->_depthTexture->getSampler() == VK_NULL_HANDLE)
    {
        return "";
    }
    return this->_colorUuid;
}

void GfxRenderTexture::_clear()
{
    if (this->_colorTexture)
    {
        delete this->_colorTexture;
        this->_colorTexture = nullptr;
    }
    if (this->_depthTexture)
    {
        delete this->_depthTexture;
        this->_depthTexture = nullptr;
    }
}
void GfxRenderTexture::_reset()
{
    this->_createTextures();
}
bool GfxRenderTexture::saveToFile1(std::string filePath)
{
    if (this->_colorTexture == nullptr)
    {
        std::cerr << "[Gfx : GfxRenderTexture]:: Cannot save, color texture is null" << std::endl;
        return false;
    }
    return this->_colorTexture->saveToFile(filePath, this->_width, this->_height);
}
void GfxRenderTexture::destroy()
{
    if (this->_colorTexture)
    {
        delete this->_colorTexture;
        this->_colorTexture = nullptr;
    }
    if (this->_depthTexture)
    {
        delete this->_depthTexture;
        this->_depthTexture = nullptr;
    }
    if (this->_framebuffer)
    {
        vkDestroyFramebuffer(Gfx::context->getVkDevice(), this->_framebuffer, nullptr);
        this->_framebuffer = VK_NULL_HANDLE;
    }
    if (this->_commandBuffer)
    {
        vkFreeCommandBuffers(Gfx::context->getVkDevice(), Gfx::context->getCommandPool(), 1, &this->_commandBuffer);
        this->_commandBuffer = VK_NULL_HANDLE;
    }
}
GfxRenderTexture::~GfxRenderTexture()
{
}

// std::string GfxRenderTexture::_generateUUID()
// {
//     static std::random_device rd;
//     static std::mt19937_64 gen(rd());
//     static std::uniform_int_distribution<uint64_t> dis;

//     uint64_t part1 = dis(gen);
//     uint64_t part2 = dis(gen);

//     std::ostringstream oss;
//     oss << std::hex << std::setfill('0')
//         << std::setw(8) << (part1 & 0xFFFFFFFF) << "-"
//         << std::setw(4) << ((part1 >> 32) & 0xFFFF) << "-"
//         << std::setw(4) << (((part2 >> 48) & 0x0FFF) | 0x4000) << "-" // 版本4
//         << std::setw(4) << (((part2 >> 32) & 0x3FFF) | 0x8000) << "-" // 变体
//         << std::setw(8) << (part2 & 0xFFFFFFFF)
//         << std::setw(4) << ((part1 >> 48) & 0xFFFF);

//     return oss.str();
// }

// /**
//  * @brief 创建渲染纹理的命令缓冲区
//  *
//  */
// void GfxRenderTexture::_createCommandBuffer()
// {
//     if (this->_commandBuffer)
//     {
//         vkFreeCommandBuffers(Gfx::context->getVkDevice(), Gfx::context->getCommandPool(), 1, &this->_commandBuffer);
//         this->_commandBuffer = VK_NULL_HANDLE;
//     }
//     VkCommandBufferAllocateInfo allocInfo = {};
//     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//     allocInfo.commandPool = Gfx::context->getCommandPool();
//     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//     allocInfo.commandBufferCount = 1;

//     if (vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, &this->_commandBuffer) != VK_SUCCESS)
//     {
//         throw std::runtime_error("failed to allocate command buffers!");
//     }
// }
