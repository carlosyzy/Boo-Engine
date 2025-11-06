#include "gfx-queue.h"
#include "gfx-pass.h"
#include "gfx-context.h"
#include "gfx-object.h"
#include "gfx-pipeline.h"

GfxQueue::GfxQueue(GfxContext *context, GfxPass *pass)
{
    this->_context = context;
    this->_pass = pass;
    this->_createBuffers();
}
/**
 * 创建帧缓冲区和命令缓冲区
 */
void GfxQueue::_createBuffers()
{
    this->_createFramebuffers();
    this->_createCommandBuffers();
}
void GfxQueue::_createFramebuffers()
{
    this->_swapChainFramebuffers.clear();
    std::vector<VkImageView> &swapChainImageViews = this->_context->getSwapChainImageViews();
    this->_swapChainFramebuffers.resize(swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 3> attachments = {
            this->_context->getColorMsaaTexture()->getImageView(),/*  // 多重采样颜色附件 */
            swapChainImageViews[i],                                /* // 解析附件（单采样，指向交换链图像） */
            this->_context->getDepthMsaaTexture()->getImageView() /*  // 多重采样深度附件 */
        };
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = this->_pass->getVkRenderPass();

        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        ;                                                                     /* // 指定附着的个数 */
        framebufferInfo.pAttachments = attachments.data();                   /*  // 渲染流程对象用于描述附着信息的pAttachment数组 */
        framebufferInfo.width = this->_context->getSwapChainExtent().width;  /*  // width和height用于指定帧缓冲的大小 */
        framebufferInfo.height = this->_context->getSwapChainExtent().height; /* // 交换链图像都是单层，layers设置为1 */
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(this->_context->getVkDevice(), &framebufferInfo, nullptr, &this->_swapChainFramebuffers[i]) != VK_SUCCESS)

        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
    /* // this->_Log("create framebuffers success..."); */
}
void GfxQueue::_createCommandBuffers()
{
    this->_commandBuffers.resize(this->_swapChainFramebuffers.size());
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
    renderPassInfo.framebuffer = this->_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = this->_context->getSwapChainExtent();

    std::array<VkClearValue, 4> clearValues = {};/*  // 至少4个，因为最高索引是3 */
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
    this->_cleanFramebuffers();
    this->_cleanCommandBuffers();
}
void GfxQueue::_cleanFramebuffers()
{
   /*  // 销毁帧缓冲（Framebuffers） */
    for (auto framebuffer : this->_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(this->_context->getVkDevice(), framebuffer, nullptr);
    }
    this->_swapChainFramebuffers.clear();
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