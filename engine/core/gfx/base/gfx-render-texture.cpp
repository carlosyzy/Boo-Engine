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
}
void GfxRenderTexture::bindRenderPass(GfxRenderPass *pass)
{
    if (this->_pass != nullptr)
    {
        return;
    }
    std::cout << "GfxRenderTexture::bindRenderPass: pass:" << pass << std::endl;
    this->_pass = pass;
    if (this->_colorTexture == nullptr || this->_depthTexture == nullptr)
    {
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
    std::cout << "GfxRenderTexture::_createFrameBuffer: framebuffer:" << this->_framebuffer << std::endl;
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
    std::cout << "GfxRenderTexture::_createCommandBuffer: commandBuffer:" << this->_commandBuffer << std::endl;
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
    std::cout << "GfxRenderTexture::resize: width:" << width << " height:" << height << std::endl;
    this->_width = width;
    this->_height = height;
    this->_width = Gfx::context->getSwapChainExtent().width;
    this->_height = Gfx::context->getSwapChainExtent().height;
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

    std::cout << "GfxRenderTexture::_createTextures: colorTexture:" << this->_colorTexture << " depthTexture:" << this->_depthTexture << std::endl;
}
const std::string &GfxRenderTexture::getColorTextureUuid() const
{
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
        std::cerr << "GfxRenderTexture : Cannot save, color texture is null" << std::endl;
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
