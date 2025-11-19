#include "gfx-queue.h"
#include "gfx-pass.h"
#include "gfx-context.h"
#include "gfx-object.h"
#include "gfx-pipeline.h"

GfxQueue::GfxQueue(std::string name, GfxContext *context)
{
    this->_name = name;
    this->_context = context;
}
void GfxQueue::create(GfxPass *pass)
{
    this->_pass = pass;
    
    this->_createBuffers();
}
/**
 * 创建帧缓冲区和命令缓冲区
 */
void GfxQueue::_createBuffers()
{
    this->_createTextures();
    this->_createFramebuffers();
    this->_createCommandBuffers();
}

void GfxQueue::_createTextures()
{
    float width = (float)this->_context->getSwapChainExtent().width;
    float height = (float)this->_context->getSwapChainExtent().height;
    GfxPassStruct &passStruct = this->_pass->getGfxPassStruct();

    // 创建深度纹理
    if (passStruct.depthAttachment.enable)
    {
        
        VkFormat depthFormat = this->_pass->getAttachmentFormat(passStruct.depthAttachment.format);
        std::cout << "depthAttachment.format: " << passStruct.depthAttachment.format << " depthFormat: " << depthFormat << std::endl;
        this->_depthTexture = new GfxTexture(this->_context);
        this->_depthTexture->createImage(width, height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
                                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
        this->_depthTexture->createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    // if (passStruct.msaa && passStruct.offscreen)
    // {
    //     VkFormat colorFormat = this->_pass->getAttachmentFormat(passStruct.colorAttachment.format);
    //     this->_colorTexture = new GfxTexture(this->_context);
    //     this->_colorTexture->createImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
    //                                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    //                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
    //     this->_colorTexture->createImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    // }
    // else if (passStruct.offscreen)
    // {
    //     // VkFormat colorFormat = this->_pass->getAttachmentFormat(passStruct.colorAttachment.format);
    //     // this->_colorTexture = new GfxTexture(this->_context);
    //     // this->_colorTexture->createImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
    //     //                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    //     //                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
    //     // std::cout << "this->_colorMsaaTexture createImage end" << std::endl;
    //     // this->_colorTexture->createImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    // }
    // // if(passStruct.offscreen){
    // // }
    // // viewport.width = (float)this->_context->getSwapChainExtent().width;
    // // viewport.height = (float)this->_context->getSwapChainExtent().height;
    // // this->_colorTexture = new GfxTexture(this->_context);
    // // this->_depthTexture = new GfxTexture(this->_context);
}

void GfxQueue::_createFramebuffers()
{
    this->_framebuffers.clear();
    std::vector<VkImageView> &swapChainImageViews = this->_context->getSwapChainImageViews();
    this->_framebuffers.resize(swapChainImageViews.size());
    GfxPassStruct &passStruct = this->_pass->getGfxPassStruct();

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        std::vector<VkImageView> attachments = {};
        attachments.push_back(swapChainImageViews[i]);
        attachments.push_back(this->_depthTexture->getImageView());
        // 多重采样
        // std::array<VkImageView, 3> attachments = {
        //     this->_context->getColorMsaaTexture()->getImageView(),/*  // 多重采样颜色附件 */
        //     swapChainImageViews[i],                                /* // 解析附件（单采样，指向交换链图像） */
        //     this->_context->getDepthMsaaTexture()->getImageView() /*  // 多重采样深度附件 */
        // };
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = this->_pass->getVkRenderPass();

        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());   /* // 指定附着的个数 */
        framebufferInfo.pAttachments = attachments.data();                   /*  // 渲染流程对象用于描述附着信息的pAttachment数组 */
        std::cout << "framebufferInfo.attachmentCount = " << framebufferInfo.attachmentCount << std::endl;
        framebufferInfo.width = this->_context->getSwapChainExtent().width;   /*  // width和height用于指定帧缓冲的大小 */
        framebufferInfo.height = this->_context->getSwapChainExtent().height; /* // 交换链图像都是单层，layers设置为1 */
        framebufferInfo.layers = 1;
        if (vkCreateFramebuffer(this->_context->getVkDevice(), &framebufferInfo, nullptr, &this->_framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

void GfxQueue::_createCommandBuffers()
{
    this->_commandBuffers.resize(this->_framebuffers.size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = this->_context->getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)this->_commandBuffers.size();

    if (vkAllocateCommandBuffers(this->_context->getVkDevice(), &allocInfo, this->_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
    /*  // this->_Log("create commandBuffers success..."); */
}

void GfxQueue::submit(GfxObject *object)
{
    GfxPipeline *pipeline = object->pipeline();
    if (pipeline == nullptr)
        return;
    if (pipeline->isTransparent())
    {
        this->_transparentQueue.push_back(object);
    }
    else
    {
        this->_opaqueQueue.push_back(object);
    }
}
void GfxQueue::render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    // std::cout << "render:queue" << std::endl;
    if (this->_commandBuffers.empty())
    {
        return;
    }
    if (imageIndex >= this->_commandBuffers.size())
        return;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(this->_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
    /*  // 绑定render pass */
    this->_beginBindRenderPass(imageIndex);
    // std::cout << "render:queue pass" << std::endl;
    /* // 更新渲染物体 */
    for (auto object : this->_opaqueQueue)
    {
        // std::cout << "render:opaque" << std::endl;
        object->render(imageIndex, this->_commandBuffers);
    }

    for (auto object : this->_transparentQueue)
    {
        // std::cout << "render:transparent" << std::endl;
        object->render(imageIndex, this->_commandBuffers);
        /*  // 渲染透明物体 */
    }

    /*  // 渲染结束 */
    vkCmdEndRenderPass(this->_commandBuffers[imageIndex]);
    /*  // 结束渲染pass */
    if (vkEndCommandBuffer(this->_commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
    commandBuffers.push_back(this->_commandBuffers[imageIndex]);

    this->_opaqueQueue.clear();
    this->_transparentQueue.clear();
}
/**
 * 绑定render pass
 */
void GfxQueue::_beginBindRenderPass(uint32_t imageIndex)
{
    /*  // 绑定render pass */
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = this->_pass->getVkRenderPass();
    renderPassInfo.framebuffer = this->_framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = this->_context->getSwapChainExtent();

    std::array<VkClearValue, 4> clearValues = {};      /*  // 至少4个，因为最高索引是3 */
                                                       /*  // 设置颜色附件的清除值（索引0） */
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}}; // 黑色
                                                       /*  // 设置其他颜色附件的清除值（如果有，索引1、2等） */
    clearValues[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    /* // 设置深度模板附件的清除值（索引3） */
    clearValues[2].depthStencil = {1.0f, 0}; /* // 深度=1.0f，模板=0 */
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    /* // renderPassInfo.clearValueCount = 1;
    // VkClearValue clearColor{};
    // clearColor.color = {1.0f, 1.0f, 1.0f, 0.0f};
    // renderPassInfo.pClearValues = &clearColor; */

    vkCmdBeginRenderPass(this->_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void GfxQueue::clear()
{
    this->_clearTextures();
    this->_cleanFramebuffers();
    this->_cleanCommandBuffers();
}
void GfxQueue::_clearTextures()
{
    if (this->_depthTexture != nullptr)
    {
       delete this->_depthTexture;
       this->_depthTexture = nullptr;
    }
}
void GfxQueue::_cleanFramebuffers()
{
    /*  // 销毁帧缓冲（Framebuffers） */
    for (auto framebuffer : this->_framebuffers)
    {
        vkDestroyFramebuffer(this->_context->getVkDevice(), framebuffer, nullptr);
    }
    this->_framebuffers.clear();
}
void GfxQueue::_cleanCommandBuffers()
{
    /* // 销毁命令缓冲（Command Buffers） */
    if (!this->_commandBuffers.empty())

    {
        vkFreeCommandBuffers(this->_context->getVkDevice(), this->_context->getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
        this->_commandBuffers.clear();
    }
}
void GfxQueue::reset()
{
    this->_createBuffers();
}

void GfxQueue::_Log(std::string msg)
{
    return;
    std::cout << "GfxQueue: " << msg << std::endl;
}

GfxQueue::~GfxQueue()
{
}