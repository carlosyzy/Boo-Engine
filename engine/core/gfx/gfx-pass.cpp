#include "gfx-pass.h"
#include "gfx.h"
#include "gfx-context.h"

GfxPass::GfxPass(std::string name)
{
    this->_name = name;
}
void GfxPass::create(GfxPassStruct gfxPassStruct)
{
    this->_gfxPassStruct = gfxPassStruct;
    this->_createVkRenderPass();
}
void GfxPass::_createVkRenderPass()
{
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;

    uint32_t attachmentCount = this->_gfxPassStruct.attachmentCount;
    std::vector<VkAttachmentDescription> attachments(attachmentCount);

    // 颜色附件
    VkAttachmentReference colorAttachmentRef{};
    if (this->_gfxPassStruct.colorAttachment.enable)
    {
        uint32_t attachment = this->_gfxPassStruct.colorAttachment.attachment;
        attachments[attachment].format = this->getAttachmentFormat(this->_gfxPassStruct.colorAttachment.format);
        attachments[attachment].samples = this->_gfxPassStruct.colorAttachment.samples;
        attachments[attachment].loadOp = this->getAttachmentLoadOp(this->_gfxPassStruct.colorAttachment.loadOp);
        attachments[attachment].storeOp = this->getAttachmentStoreOp(this->_gfxPassStruct.colorAttachment.storeOp);
        attachments[attachment].stencilLoadOp = this->getAttachmentLoadOp(this->_gfxPassStruct.colorAttachment.stencilLoadOp);
        attachments[attachment].stencilStoreOp = this->getAttachmentStoreOp(this->_gfxPassStruct.colorAttachment.stencilStoreOp);
        attachments[attachment].initialLayout = this->getAttachmentLayout(this->_gfxPassStruct.colorAttachment.initialLayout);
        attachments[attachment].finalLayout = this->getAttachmentLayout(this->_gfxPassStruct.colorAttachment.finalLayout);

        colorAttachmentRef.attachment = attachment;
        colorAttachmentRef.layout = this->getAttachmentLayout(this->_gfxPassStruct.colorAttachment.refLayout);
        subpass.pColorAttachments = &colorAttachmentRef;
    }
    // 解析附件
    VkAttachmentReference resolveAttachmentRef{};
    if (this->_gfxPassStruct.resolveAttachment.enable)
    {
        uint32_t attachment = this->_gfxPassStruct.resolveAttachment.attachment;
        attachments[attachment].format = this->getAttachmentFormat(this->_gfxPassStruct.resolveAttachment.format);
        attachments[attachment].samples = this->_gfxPassStruct.resolveAttachment.samples;
        attachments[attachment].loadOp = this->getAttachmentLoadOp(this->_gfxPassStruct.resolveAttachment.loadOp);
        attachments[attachment].storeOp = this->getAttachmentStoreOp(this->_gfxPassStruct.resolveAttachment.storeOp);
        attachments[attachment].stencilLoadOp = this->getAttachmentLoadOp(this->_gfxPassStruct.resolveAttachment.stencilLoadOp);
        attachments[attachment].stencilStoreOp = this->getAttachmentStoreOp(this->_gfxPassStruct.resolveAttachment.stencilStoreOp);
        attachments[attachment].initialLayout = this->getAttachmentLayout(this->_gfxPassStruct.resolveAttachment.initialLayout);
        attachments[attachment].finalLayout = this->getAttachmentLayout(this->_gfxPassStruct.resolveAttachment.finalLayout);
        resolveAttachmentRef.attachment = attachment;
        resolveAttachmentRef.layout = this->getAttachmentLayout(this->_gfxPassStruct.resolveAttachment.refLayout);
        subpass.pResolveAttachments = &resolveAttachmentRef;
    }
    // 深度附件
    VkAttachmentReference depthAttachmentRef{};
    if (this->_gfxPassStruct.depthAttachment.enable)
    {
        uint32_t attachment = this->_gfxPassStruct.depthAttachment.attachment;
        attachments[attachment].format = this->getAttachmentFormat(this->_gfxPassStruct.depthAttachment.format);
        attachments[attachment].samples = this->_gfxPassStruct.depthAttachment.samples;
        attachments[attachment].loadOp = this->getAttachmentLoadOp(this->_gfxPassStruct.depthAttachment.loadOp);
        attachments[attachment].storeOp = this->getAttachmentStoreOp(this->_gfxPassStruct.depthAttachment.storeOp);
        attachments[attachment].stencilLoadOp = this->getAttachmentLoadOp(this->_gfxPassStruct.depthAttachment.stencilLoadOp);
        attachments[attachment].stencilStoreOp = this->getAttachmentStoreOp(this->_gfxPassStruct.depthAttachment.stencilStoreOp);
        attachments[attachment].initialLayout = this->getAttachmentLayout(this->_gfxPassStruct.depthAttachment.initialLayout);
        attachments[attachment].finalLayout = this->getAttachmentLayout(this->_gfxPassStruct.depthAttachment.finalLayout);
        depthAttachmentRef.attachment = attachment;
        depthAttachmentRef.layout = this->getAttachmentLayout(this->_gfxPassStruct.depthAttachment.refLayout);
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
    }

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    // 多重采样
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;


    /*  // 创建渲染流程 */
    if (vkCreateRenderPass(Gfx::context->vkDevice(), &renderPassInfo, nullptr, &this->_vkRenderPass) != VK_SUCCESS)
    {
        // throw std::runtime_error("failed to create render pass!");
        std::cout << "GfxPass :create render pass failed " << this->_name << std::endl;
        return;
    }
    std::cout << "GfxPass :create render pass success " << this->_name << std::endl;
}
VkFormat GfxPass::getAttachmentFormat(uint32_t attachment)
{
    if (attachment == 0)
    {
        return Gfx::context->getSwapChainImageFormat();
    }
    else if (attachment == 1)
    {
        return VK_FORMAT_D32_SFLOAT;
    }
    else if (attachment == 2)
    {
        return VK_FORMAT_D24_UNORM_S8_UINT;
    }
    else if (attachment == 3)
    {
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    }
    return VK_FORMAT_UNDEFINED;
}
VkAttachmentLoadOp GfxPass::getAttachmentLoadOp(GfxPassAttachmentLoadOp loadOp)
{
    if (loadOp == GfxPassAttachmentLoadOp::Load)
    {
        return VK_ATTACHMENT_LOAD_OP_LOAD;
    }
    else if (loadOp == GfxPassAttachmentLoadOp::Clear)
    {
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    }
    else if (loadOp == GfxPassAttachmentLoadOp::DontCare)
    {
        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
    return VK_ATTACHMENT_LOAD_OP_CLEAR;
}
VkAttachmentStoreOp GfxPass::getAttachmentStoreOp(GfxPassAttachmentStoreOp storeOp)
{
    if (storeOp == GfxPassAttachmentStoreOp::Store)
    {
        return VK_ATTACHMENT_STORE_OP_STORE;
    }
    else if (storeOp == GfxPassAttachmentStoreOp::DontCare)
    {
        return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
}
/**
 * @brief 获取附件的布局类型
 *
 * @param finalLayout 最终布局类型
 * @return VkImageLayout Vulkan 附件最终布局
 */
VkImageLayout GfxPass::getAttachmentLayout(GfxPassAttachmentLayout layout)
{
    if (layout == GfxPassAttachmentLayout::Undefined)
    {
        return VK_IMAGE_LAYOUT_UNDEFINED;
    }
    else if (layout == GfxPassAttachmentLayout::Color)
    {
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    else if (layout == GfxPassAttachmentLayout::Shader)
    {
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    else if (layout == GfxPassAttachmentLayout::Depth)
    {
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }
    else if (layout == GfxPassAttachmentLayout::Present)
    {
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    return VK_IMAGE_LAYOUT_UNDEFINED;
}

void GfxPass::clear()
{
    if (this->_vkRenderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(Gfx::context->vkDevice(), this->_vkRenderPass, nullptr);
        this->_vkRenderPass = VK_NULL_HANDLE;
    }
}
void GfxPass::reset()
{
    this->_createVkRenderPass();
}
GfxPass::~GfxPass()
{
}

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

// VkAttachmentDescription colorAttachment = {};
// colorAttachment.format = this->_context->getSwapChainImageFormat(); // 颜色缓冲附着(VkImageView)的格式
// colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                    // 指定采样数，没有使用多重采样，设置为1
// colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;               // VK_ATTACHMENT_LOAD_OP_LOAD;                // 重要！
// colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
// colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
// colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

// /* // 一个渲染流程包含多个子流程，子流程依赖上一流程处理后的帧缓冲内容 */
// VkAttachmentReference colorAttachmentRef{};
// colorAttachmentRef.attachment = 0; /*  // 第一个附件是多重采样颜色附件 */
// colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// // 多重采样
// // VkAttachmentReference resolveAttachmentRef{};
// // resolveAttachmentRef.attachment = 1; /* // 第二个附件是解析附件 */
// // resolveAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
// // VkAttachmentReference depthAttachmentRef{};
// // depthAttachmentRef.attachment = 2; /* // 第三个附件是深度附件 */
// // depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

// VkSubpassDescription subpass = {};
// subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
// subpass.colorAttachmentCount = 1;
// subpass.pColorAttachments = &colorAttachmentRef;
// // 多重采样
// // subpass.pDepthStencilAttachment = &depthAttachmentRef;
// // subpass.pResolveAttachments = &resolveAttachmentRef; /*  // 指定解析附件 */