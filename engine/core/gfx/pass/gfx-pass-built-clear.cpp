//#include "gfx-pass-built-clear.h"
//#include "../gfx.h"
//#include "../gfx-context.h"
///**
// *  首帧渲染
// *  清除颜色附件
// */
//GfxPassBuiltClear::GfxPassBuiltClear(std::string name) : GfxPass(name)
//{
//}
//void GfxPassBuiltClear::create()
//{
//    VkAttachmentDescription colorAttachment = {};
//    colorAttachment.format = Gfx::context->getSwapChainImageFormat(); // 颜色缓冲附着(VkImageView)的格式
//    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                  // 指定采样数，没有使用多重采样，设置为1
//    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;              // VK_ATTACHMENT_LOAD_OP_LOAD;                // 重要！
//    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//    VkAttachmentReference colorAttachmentRef{};
//    colorAttachmentRef.attachment = 0; /*  // 第一个附件是多重采样颜色附件 */
//    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//    VkSubpassDescription subpass{};
//    subpass.pColorAttachments = &colorAttachmentRef;
//
//    VkSubpassDependency dependency = {};
//    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//    dependency.dstSubpass = 0;
//    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//    dependency.srcAccessMask = 0;
//    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//    dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//    VkRenderPassCreateInfo renderPassInfo = {};
//    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//    subpass.colorAttachmentCount = 1;
//    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//    // 多重采样
//    renderPassInfo.attachmentCount = 1;
//    renderPassInfo.pAttachments = &colorAttachment;
//    renderPassInfo.subpassCount = 1;
//    renderPassInfo.pSubpasses = &subpass;
//    renderPassInfo.dependencyCount = 1;
//    renderPassInfo.pDependencies = &dependency;
//
//    /*  // 创建渲染流程 */
//    if (vkCreateRenderPass(Gfx::context->vkDevice(), &renderPassInfo, nullptr, &this->_vkRenderPass) != VK_SUCCESS)
//    {
//        // throw std::runtime_error("failed to create render pass!");
//        std::cout << "GfxPass :create render pass failed " << this->_name << std::endl;
//        return;
//    }
//    std::cout << "GfxPass :create render pass success " << this->_name << std::endl;
//}
//
//GfxPassBuiltClear::~GfxPassBuiltClear()
//{
//}
