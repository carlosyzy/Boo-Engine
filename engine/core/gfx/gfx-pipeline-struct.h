#pragma once
#include <vulkan/vulkan_core.h>

// 保存名字：Blend:0|DepthTest:0|DepthWrite:0|vert:filename|frag:filename

struct OpaqueBlendState
{
    // VkPipelineColorBlendAttachmentState opaqueBlendState = {
    //     .blendEnable = VK_FALSE,           // 禁用混合
    //     .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
    //     .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
    //     .colorBlendOp = VK_BLEND_OP_ADD,
    //     .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    //     .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    //     .alphaBlendOp = VK_BLEND_OP_ADD,
    //     .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
    //                      VK_COLOR_COMPONENT_G_BIT |
    //                      VK_COLOR_COMPONENT_B_BIT |
    //                      VK_COLOR_COMPONENT_A_BIT
    // };
};
struct TransparentBlendState
{
    // VkPipelineColorBlendAttachmentState alphaBlendState = {
    //     .blendEnable = VK_TRUE,
    //     .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,        // src_alpha
    //     .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, // 1-src_alpha
    //     .colorBlendOp = VK_BLEND_OP_ADD,
    //     .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    //     .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    //     .alphaBlendOp = VK_BLEND_OP_ADD,
    //     .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
    //                      VK_COLOR_COMPONENT_G_BIT |
    //                      VK_COLOR_COMPONENT_B_BIT |
    //                      VK_COLOR_COMPONENT_A_BIT
    // };
};

// VkPipelineColorBlendAttachmentState additiveBlendState = {
//     .blendEnable = VK_TRUE,
//     .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // one
//     .dstColorBlendFactor = VK_BLEND_FACTOR_ONE, // one
//     .colorBlendOp = VK_BLEND_OP_ADD,
//     .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
//     .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
//     .alphaBlendOp = VK_BLEND_OP_ADD,
//     .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
//                       VK_COLOR_COMPONENT_G_BIT |
//                       VK_COLOR_COMPONENT_B_BIT |
//                       VK_COLOR_COMPONENT_A_BIT};

// VkPipelineColorBlendAttachmentState multiplyBlendState = {
//     .blendEnable = VK_TRUE,
//     .srcColorBlendFactor = VK_BLEND_FACTOR_DST_COLOR, // dst_color
//     .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,      // zero
//     .colorBlendOp = VK_BLEND_OP_ADD,
//     .srcAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA,
//     .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
//     .alphaBlendOp = VK_BLEND_OP_ADD,
//     .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
//                       VK_COLOR_COMPONENT_G_BIT |
//                       VK_COLOR_COMPONENT_B_BIT |
//                       VK_COLOR_COMPONENT_A_BIT};