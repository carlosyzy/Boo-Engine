#include "gfx-render-target.h"
#include "gfx-context.h"
#include "gfx-texture.h"
#include "gfx-pass.h"
#include "gfx-mgr.h"
#include <stdexcept>
#include <iostream>

GfxRenderTarget::GfxRenderTarget(std::string name, GfxContext *context, uint32_t width, uint32_t height)
    : _name(name), _context(context), _width(width), _height(height),
      _pass(nullptr), _colorTexture(nullptr), _depthTexture(nullptr),
      _colorMsaaTexture(nullptr), _framebuffer(VK_NULL_HANDLE)
{
}

void GfxRenderTarget::create(GfxPass *pass)
{
    this->_pass = pass;
    this->_createTextures();
    this->_createFramebuffer();
    this->_Log("RenderTarget created successfully");
}

void GfxRenderTarget::_createTextures()
{
    this->_Log("Creating render target textures...");

    /* // 1. 创建 MSAA 颜色附件（如果启用 MSAA） */
    if (MsaaSamples != VK_SAMPLE_COUNT_1_BIT)
    {
        this->_colorMsaaTexture = new GfxTexture(this->_context);
        this->_colorMsaaTexture->createImage(
            this->_width, this->_height,
            this->_context->getSwapChainImageFormat(),
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            MsaaSamples);
        this->_colorMsaaTexture->createImageView(
            this->_context->getSwapChainImageFormat(),
            VK_IMAGE_ASPECT_COLOR_BIT);
    }

    /* // 2. 创建最终颜色附件（resolve target 或直接渲染目标） */
    this->_colorTexture = new GfxTexture(this->_context);
    this->_colorTexture->createImage(
        this->_width, this->_height,
        this->_context->getSwapChainImageFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, /* // 可作为纹理采样 */
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_SAMPLE_COUNT_1_BIT);
    this->_colorTexture->createImageView(
        this->_context->getSwapChainImageFormat(),
        VK_IMAGE_ASPECT_COLOR_BIT);

    /* // 3. 创建深度附件 */
    this->_depthTexture = new GfxTexture(this->_context);
    this->_depthTexture->createImage(
        this->_width, this->_height,
        VK_FORMAT_D32_SFLOAT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        MsaaSamples);
    this->_depthTexture->createImageView(
        VK_FORMAT_D32_SFLOAT,
        VK_IMAGE_ASPECT_DEPTH_BIT);

    this->_Log("Textures created");
}

void GfxRenderTarget::_createFramebuffer()
{
    this->_Log("Creating framebuffer...");

    std::vector<VkImageView> attachments;

    if (MsaaSamples != VK_SAMPLE_COUNT_1_BIT)
    {
        /* // MSAA 模式：[MSAA 颜色, Resolve 颜色, MSAA 深度] */
        attachments = {
            this->_colorMsaaTexture->getImageView(),
            this->_colorTexture->getImageView(),
            this->_depthTexture->getImageView()};
    }
    else
    {
        /* // 非 MSAA 模式：[颜色, 深度] */
        attachments = {
            this->_colorTexture->getImageView(),
            this->_depthTexture->getImageView()};
    }

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = this->_pass->getVkRenderPass();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = this->_width;
    framebufferInfo.height = this->_height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(this->_context->getVkDevice(), &framebufferInfo, nullptr, &this->_framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create offscreen framebuffer!");
    }

    this->_Log("Framebuffer created");
}

void GfxRenderTarget::_cleanTextures()
{
    if (this->_colorMsaaTexture)
    {
        delete this->_colorMsaaTexture;
        this->_colorMsaaTexture = nullptr;
    }
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

void GfxRenderTarget::_cleanFramebuffer()
{
    if (this->_framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(this->_context->getVkDevice(), this->_framebuffer, nullptr);
        this->_framebuffer = VK_NULL_HANDLE;
    }
}

void GfxRenderTarget::clear()
{
    this->_cleanFramebuffer();
    this->_cleanTextures();
}

void GfxRenderTarget::reset()
{
    this->clear();
    this->_createTextures();
    this->_createFramebuffer();
}

bool GfxRenderTarget::saveToFile(const std::string& filePath)
{
    if (this->_colorTexture == nullptr)
    {
        std::cerr << "GfxRenderTarget [" << this->_name << "]: Cannot save, color texture is null" << std::endl;
        return false;
    }

    return this->_colorTexture->saveToFile(filePath, this->_width, this->_height);
}

void GfxRenderTarget::_Log(std::string msg)
{
    std::cout << "GfxRenderTarget [" << this->_name << "]: " << msg << std::endl;
}

GfxRenderTarget::~GfxRenderTarget()
{
    this->clear();
}
