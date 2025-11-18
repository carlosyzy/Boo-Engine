#include "gfx-pass.h"
#include "gfx-mgr.h"
#include "gfx-context.h"

GfxPass::GfxPass(std::string name, GfxContext *context)
{
    this->_name = name;
    this->_context = context;
}
void GfxPass::create()
{
    // PassAttachmentColor attachmentColor{};
    // this->_attachmentColor = attachmentColor; /* // 不用引用，外部的销毁 */
    this->_createVkRenderPass();
}
void GfxPass::_createVkRenderPass()
{
    this->_Log("create render pass start...");
    std::vector<VkAttachmentDescription> attachments(3);
    //     // 多重采样
    //     /* // 1. 多重采样的颜色附件 */
    //     attachments[0].format = this->_context->getSwapChainImageFormat();
    //     attachments[0].samples = MsaaSamples; /* // 使用之前设置的MSAA采样数 */
    //     attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    //     attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; /* // 多重采样图像不需要存储，DONT_CARE更高效 */
    //     attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //     attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //     attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //     attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    //    /*  // 2. 解析附件（指向交换链图像） */
    //     attachments[1].format = this->_context->getSwapChainImageFormat();
    //     attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;/*  // 解析目标必须是单采样 */
    //     attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //     attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;/*  // 必须存储，因为要显示 */
    //     attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //     attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //     attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //     attachments[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //    /*  // 3. 多重采样的深度附件 */
    //     attachments[2].format = VK_FORMAT_D32_SFLOAT; /* // 需要实现这个函数来寻找深度格式 */
    //     attachments[2].samples = MsaaSamples;                             /*  // 深度样本数应与颜色一致 */
    //     attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    //     attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; /* // 渲染完成后通常不需要深度数据 */
    //     attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //     attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //     attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //     attachments[2].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = this->_context->getSwapChainImageFormat(); // 颜色缓冲附着(VkImageView)的格式
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                    // 指定采样数，没有使用多重采样，设置为1
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // VK_ATTACHMENT_LOAD_OP_LOAD;                // 重要！
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    /* // 一个渲染流程包含多个子流程，子流程依赖上一流程处理后的帧缓冲内容 */
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0; /*  // 第一个附件是多重采样颜色附件 */
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // 多重采样
    // VkAttachmentReference resolveAttachmentRef{};
    // resolveAttachmentRef.attachment = 1; /* // 第二个附件是解析附件 */
    // resolveAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // VkAttachmentReference depthAttachmentRef{};
    // depthAttachmentRef.attachment = 2; /* // 第三个附件是深度附件 */
    // depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    // 多重采样
    // subpass.pDepthStencilAttachment = &depthAttachmentRef;
    // subpass.pResolveAttachments = &resolveAttachmentRef; /*  // 指定解析附件 */

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    /*  // VkClearValue clearColor = {{0.0f, 0.0f, 0.0f, 0.0f}}; // RGBA = (0,0,0,0) */

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    // 多重采样
    // renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    // renderPassInfo.pAttachments = attachments.data();

    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;

    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    /*  // 创建渲染流程 */
    if (vkCreateRenderPass(this->_context->getVkDevice(), &renderPassInfo, nullptr, &this->_vkRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
    this->_Log("create render pass success...");
}
void GfxPass::clear()
{
    if (this->_vkRenderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(this->_context->getVkDevice(), this->_vkRenderPass, nullptr);

        this->_vkRenderPass = VK_NULL_HANDLE;
    }
}
void GfxPass::reset()
{
    this->_createVkRenderPass();
}

void GfxPass::_Log(std::string msg)
{
    return;
    std::cout << "GfxPass: " << msg << std::endl;
}
GfxPass::~GfxPass()
{
}
