#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan_core.h>

// // struct GfxPipelineType{
// //     const std::string UI = "UI";
// //     const std::string MODEL = "MODEL";
// // };
// enum class GfxPipelineType
// {
//     /**
//      * UI 渲染管线
//      */
//     UI = 0,
//     /**
//      * 模型渲染管线
//      */
//     MODEL = 1,
// };
// const std::unordered_map<GfxPipelineType, std::string> GfxPipelineTypeNames = {
//     {GfxPipelineType::UI, "UI"},
//     {GfxPipelineType::MODEL, "MODEL"},
// };

/**
 * 深度比较操作
 */
enum class GfxPipelineCompareOp
{
    /**
     * 从不通过深度测试
     */
    Never = 0,
    /**
     * 仅当新像素的深度值小于当前深度值时通过
     * 特殊效果，如深度相等高亮
     */
    Less = 1,
    /**
     * 仅当新像素的深度值小于等于当前深度值时通过
     * 阴影映射、某些后处理效果
     */
    LessOrEqual = 2,
    /**
     * 仅当新像素的深度值大于当前深度值时通过
     * 反转深度测试、特殊效果
     */
    Greater = 3,
    /**
     * 仅当新像素的深度值大于等于当前深度值时通过
     * 通过  反射、门户效果
     */
    GreaterOrEqual = 4,
    /**
     * 仅当新像素的深度值等于当前深度值时通过
     * 特殊效果，如深度相等高亮
     */
    Equal = 5,
    /**
     * 仅当新像素的深度值不等于当前深度值时通过
     * 轮廓效果、剔除特定深度
     */
    NotEqual = 6,
    /**
     * 总是通过深度测试
     */
    Always = 7,
};
enum class GfxPipelineStencilOp
{
    /**
     * 保持原值
     */
    Keep = 0,
    /**
     * 测试通过：值+1
     */
    Increment_Add = 3,
    /**
     * 测试通过：值-1
     */
    Decrement_Subtract = 4,
    /**
     * 测试通过：替换为参考值
     */
    Replace = 5,

};
enum class GfxPipelineColorBlendFactor
{
    /**
     * 加法混合
     */
    One = 0,
    /**
     * 减法混合
     */
    Zero = 1,
    /**
     * 源颜色
     */
    SrcColor = 2,
    /**
     * 目标颜色
     */
    DstColor = 3,
    /**
     * 源alpha
     */
    SrcAlpha = 4,
    /**
     * 目标alpha
     */
    DstAlpha = 5,
    /**
     * 源1-minus alpha
     */
    OneMinusSrcAlpha = 6,
    /**
     * 目标1-minus alpha
     */
    OneMinusDstAlpha = 7,
};
enum class GfxPipelineColorBlendOp
{
    /**
     * 加法混合
     */
    Add = 0,
    /**
     * 减法混合
     */
    Subtract = 1,
};
enum class GfxPipelinePolygonMode
{
    /**
     * 填充多边形
     */
    Fill = 0,
    /**
     * 线框多边形
     */
    Line = 1,

};
enum class GfxPipelineCullMode
{
    /**
     * 不剔除任何三角形
     */
    None = 0,
    /**
     * 剔除正面三角形（逆时针）
     */
    Front = 1,
    /**
     * 剔除反面三角形（顺时针）
     */
    Back = 2,
};

struct GfxPipelineStruct
{
    std::string vert = "";
    std::string geom = "";
    std::string frag = "";
    std::string pass = "";

    /**
     * 多边形模式
     */
    GfxPipelinePolygonMode polygonMode = GfxPipelinePolygonMode::Fill;
    /**
     * 剔除模式
     */
    GfxPipelineCullMode cullMode = GfxPipelineCullMode::Back;

    /**
     * 深度测试
     * 开关
     */
    int depthTest = 0;
    /**
     * 深度写入
     * 开关
     */
    int depthWrite = 0;
    /**
     * 深度比较操作
     */
    GfxPipelineCompareOp depthCompareOp = GfxPipelineCompareOp::LessOrEqual;

    /**
     * 模板测试
     * 开关
     */
    int stencilTest = 0;
    /**
     * 正面三角形（逆时针）的Stencil操作
     */
    GfxPipelineCompareOp stencilFrontCompareOp = GfxPipelineCompareOp::Never;
    GfxPipelineStencilOp stencilFrontFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add;
    uint32_t stencilFrontCompareMask = 0xFFFFFFFF;
    uint32_t stencilFrontWriteMask = 0xFFFFFFFF;
    uint32_t stencilFrontRreference = 0;
    /**
     * 反面三角形（顺时针）的Stencil操作
     */
    GfxPipelineCompareOp stencilBackCompareOp = GfxPipelineCompareOp::Never;
    GfxPipelineStencilOp stencilBackFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
    uint32_t stencilBackCompareMask = 0xFFFFFFFF;
    uint32_t stencilBackWriteMask = 0xFFFFFFFF;
    uint32_t stencilBackRreference = 0;

    /**
     * 颜色缓和
     * 开关
     */
    int colorBlend = 0;
    /**
     * 混合模式
     */
    /**
     * 源颜色混合因子
     */
    GfxPipelineColorBlendFactor srcColorBlendFactor = GfxPipelineColorBlendFactor::One;
    /**
     * 目标颜色混合因子
     */
    GfxPipelineColorBlendFactor dstColorBlendFactor = GfxPipelineColorBlendFactor::Zero;
    /**
     * 颜色混合操作
     */
    GfxPipelineColorBlendOp colorBlendOp = GfxPipelineColorBlendOp::Add;
    /**
     * 源alpha混合因子
     */
    GfxPipelineColorBlendFactor srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
    /**
     * 目标alpha混合因子
     */
    GfxPipelineColorBlendFactor dstAlphaBlendFactor = GfxPipelineColorBlendFactor::Zero;
    /**
     * alpha混合操作
     */
    GfxPipelineColorBlendOp alphaBlendOp = GfxPipelineColorBlendOp::Add;
    /**
     * 颜色写入掩码
     * 1:R
     * 1:G
     * 2:B
     * 3:A
     * 暂时统一按照RGBA顺序
     */
    uint32_t colorWriteMask = 0;

    int pushConstant = 0;
    VkDeviceSize pushConstantSize = 0;

    int descriptor = 0;
    std::string descriptorSet = "";
};





// 后期方案确定
// 方案 3: 动态渲染（最佳，Vulkan 1.3+）

//   // ✅ 使用动态渲染，完全避免 Render Pass 和 Pipeline 的耦合

//   // 创建 Pipeline 时不指定 Render Pass
//   VkPipelineRenderingCreateInfo pipelineRenderingInfo{};
//   pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
//   pipelineRenderingInfo.colorAttachmentCount = 1;
//   VkFormat colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
//   pipelineRenderingInfo.pColorAttachmentFormats = &colorFormat;
//   pipelineRenderingInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;

//   VkGraphicsPipelineCreateInfo pipelineInfo{};
//   pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//   pipelineInfo.pNext = &pipelineRenderingInfo;
//   pipelineInfo.renderPass = VK_NULL_HANDLE;  // ⚠️ 不需要 Render Pass！
//   // ...

//   vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo,
//                            nullptr, &graphicsPipeline);

//   // 渲染时动态指定
//   VkRenderingAttachmentInfo colorAttachment{};
//   colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
//   colorAttachment.imageView = swapChainImageViews[imageIndex];
//   colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//   colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//   colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//   colorAttachment.clearValue = clearColor;

//   VkRenderingInfo renderingInfo{};
//   renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
//   renderingInfo.renderArea = {{0, 0}, swapChainExtent};
//   renderingInfo.layerCount = 1;
//   renderingInfo.colorAttachmentCount = 1;
//   renderingInfo.pColorAttachments = &colorAttachment;
//   renderingInfo.pDepthAttachment = &depthAttachment;

//   vkCmdBeginRendering(commandBuffer, &renderingInfo);
//   vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//   graphicsPipeline);
//   // ...
//   vkCmdEndRendering(commandBuffer);

/**
 * Blend:1
 * DepthTest:0  深度测试
 *      UI:不启用深度测试
 *      Mask:不启用深度测试
 * DepthWrite:0  深度写入
 *      UI:不启用深度测试
 *      Mask:不启用深度测试
 * StencilTest:0  缓冲区的深度处理
 *      UI:不启用深度测试
 *      Mask:启用
 *    ADD:
 *       compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
 *       passOp: VK_STENCIL_OP_INCREMENT_AND_WRAP  测试通过：值+1
 *    SUBTRACT:
 *       compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
 *       passOp: VK_STENCIL_OP_DECREMENT_AND_WRAP  测试通过：值-1
 *    REPLACE:
 *       compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
 *       passOp: VK_STENCIL_OP_REPLACE  测试通过：替换为参考值
 *   front: 正面三角形（逆时针）的Stencil操作
 *      compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
 *      failOp: VK_STENCIL_OP_KEEP  测试失败：保持原值
 *      depthFailOp: VK_STENCIL_OP_KEEP  深度测试失败：保持原值
 *      passOp: VK_STENCIL_OP_INCREMENT_AND_WRAP  测试通过：值+1
 *      compareMask: 0xFF  比较所有位
 *      writeMask: 0xFF  写入所有位
 *      reference: 1  参考值（会被动态覆盖）
 *   back: 背面三角形（顺时针）的Stencil操作
 *      compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
 *      failOp: VK_STENCIL_OP_KEEP  测试失败：保持原值
 *      depthFailOp: VK_STENCIL_OP_KEEP  深度测试失败：保持原值
 *      passOp: VK_STENCIL_OP_DECREMENT_AND_WRAP  测试通过：值-1
 *      compareMask: 0xFF  比较所有位
 *      writeMask: 0xFF  写入所有位
 *      reference: 1  参考值（会被动态覆盖）
 * DepthCompareOp
 *      VK_COMPARE_OP_NEVER 永远不通过深度测试
 *      VK_COMPARE_OP_LESS  当前深度 < 缓冲区深度 → 通过  标准深度测试，近处物体遮挡远处物体
 *      VK_COMPARE_OP_EQUAL  当前深度 = 缓冲区深度 → 通过  特殊效果，如深度相等高亮
 *      VK_COMPARE_OP_LESS_OR_EQUAL  当前深度 ≤ 缓冲区深度 → 通过  阴影映射、某些后处理效果
 *      VK_COMPARE_OP_GREATER  当前深度 > 缓冲区深度 → 通过  反转深度测试、特殊效果
 *      VK_COMPARE_OP_NOT_EQUAL  当前深度 ≠ 缓冲区深度 → 通过  轮廓效果、剔除特定深度
 *      VK_COMPARE_OP_GREATER_OR_EQUAL  当前深度 ≥ 缓冲区深度 → 通过  反射、门户效果
 *      VK_COMPARE_OP_ALWAYS  永远通过深度测试  UI渲染、天空盒、透明物体
 *
 * vert:"resources/shader/ui/ui-mask.vert.spv"
 * frag:"resources/shader/ui/ui-mask.frag.spv"
 * rasterizerDiscardEnable:0   // 启用光栅化丢弃，将所有片段都丢弃
 * polygonMode:
 *      VK_POLYGON_MODE_FILL   // 填充模式
 *      VK_POLYGON_MODE_LINE（线框模式）
 *      VK_POLYGON_MODE_POINT（点模式）
 * cullMode:
 *      VK_CULL_MODE_BACK_BIT   // 背面剔除
 *      VK_CULL_MODE_FRONT_BIT  // 正面剔除
 *      VK_CULL_MODE_NONE       // 不剔除
 * frontFace:
 *      VK_FRONT_FACE_COUNTER_CLOCKWISE   // 逆时针为正面
 *      VK_FRONT_FACE_CLOCKWISE          // 顺时针为正面
 */