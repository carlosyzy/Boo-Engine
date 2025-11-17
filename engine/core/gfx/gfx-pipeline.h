#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
#include "gfx-struct.h"

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


class GfxPass;
class GfxShader;
class GfxContext;

class GfxPipeline
{
private:
    std::string _name;
    GfxContext *_context;
    GfxPass *_pass;
    GfxShader *_vertexShader;
    GfxShader *_fragmentShader;
    std::unordered_map<std::string, std::string> _pipelineState;

    VkPipelineLayout _vkPipelineLayout;
    VkPipeline _vkPipeline;
    /**
     * @brief 管道
     */
    void _createPipeline();
    
    VkDescriptorSetLayout _descriptorSetLayout;
    /**
     * 初始化描述符集布局
     */
    void _createDescriptorSetLayout();

    void _initShaderStage(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages);
    void _initPipelineDepth(VkPipelineDepthStencilStateCreateInfo &depthStencil);
    void _initPipelineColorBlend(VkPipelineColorBlendAttachmentState &colorBlendAttachment);
    void _Log(std::string msg);

public:
    GfxPipeline(std::string name, GfxContext *context);
    void create(GfxPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, std::unordered_map<std::string, std::string> pipelineState);
    void clear();
    void reset();

    std::string name()
    {
        return this->_name;
    }

    VkDescriptorSetLayout getDescriptorSetLayout()
    {
        return this->_descriptorSetLayout;
    }
    VkPipeline getVKPipeline()
    {
        return this->_vkPipeline;
    }
    VkPipelineLayout getVKPipelineLayout()
    {
        return this->_vkPipelineLayout;
    }
    bool isTransparent();

    ~GfxPipeline();
};
