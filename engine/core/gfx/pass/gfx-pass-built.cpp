#include "gfx-pass-built.h"
#include "../gfx.h"
#include "../gfx-context.h"
/**
 * 直接渲染到交换链图像
 * 只有一个颜色附件
 * 保留上一帧的内容
 */
GfxPassBuilt::GfxPassBuilt(std::string name) : GfxPass(name)
{
    this->_create();
}
void GfxPassBuilt::_create()
{
    GfxPass::_create();
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = Gfx::context->getSwapChainImageFormat(); // 颜色缓冲附着(VkImageView)的格式
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                  // 指定采样数，没有使用多重采样，设置为1
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; //; // 重要！VK_ATTACHMENT_LOAD_OP_CLEAR
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkClearValue clearColor = {{0.0f, 0.0f, 0.0f, 0.0f}}; // RGBA = (0,0,0,0)
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    // 多重采样
    /*  // 创建渲染流程 */
    if (vkCreateRenderPass(Gfx::context->vkDevice(), &renderPassInfo, nullptr, &this->_vkRenderPass) != VK_SUCCESS)
    {
        // throw std::runtime_error("failed to create render pass!");
        std::cout << "Gfx : Pass :: create render pass failed " << this->_name << std::endl;
        return;
    }
    std::cout << "Gfx : Pass :: create render pass success " << this->_name << std::endl;
}

GfxPassBuilt::~GfxPassBuilt()
{
}
