
#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "gfx-pipeline-struct.h"

class GfxContext;
class GfxPass;
class GfxShader;
class GfxDescriptor;

class GfxPipeline
{
protected:
    /**
     * @brief 管道名称
     */
    std::string _name;
    /**
     * @brief 渲染通道
     */
    GfxPass *_pass;
    /**
     * @brief 顶点着色器
     */
    GfxShader *_vertexShader;
    /**
     * @brief 片段着色器
     */
    GfxShader *_fragmentShader;

    //  管道状态描述
    GfxPipelineStruct _pipelineStruct;

    VkPipelineShaderStageCreateInfo _vertShaderStageInfo;
    VkPipelineShaderStageCreateInfo _fragShaderStageInfo;
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    /**
     * @brief 初始化着色器状态
     */
    virtual void _initShaderState();
    //  顶点坐标输入属性描述
    VkVertexInputAttributeDescription _vInputAttribDescriptionPos;
    //  顶点颜色输入属性描述
    VkVertexInputAttributeDescription _vInputAttribDescriptionColor;
    //  顶点法线输入属性描述
    VkVertexInputAttributeDescription _vInputAttribDescriptionNormal;
    //  顶点纹理坐标输入属性描述
    VkVertexInputAttributeDescription _vInputAttribDescriptionTexCoord;
    //  顶点输入属性描述集合
    std::vector<VkVertexInputAttributeDescription> _vertexInputAttributes;

    //  顶点输入绑定描述
    VkVertexInputBindingDescription _vInputBindDescription;
    //  顶点输入绑定描述集合
    std::vector<VkVertexInputBindingDescription> _vertexInputBindings;
    //  顶点输入状态描述
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    virtual void _initVertexInputState();

    //  输入装配状态描述
    VkPipelineInputAssemblyStateCreateInfo _inputAssemblyInfo;
    virtual void _initInputAssemblyState();

    //  视图端口状态描述
    std::vector<VkDynamicState> _dynamicStates;
    VkPipelineDynamicStateCreateInfo _dynamicStateInfo;
    virtual void _initDynamicState();

    VkViewport _viewport;
    VkRect2D _scissor;
    //  视图端口状态描述
    VkPipelineViewportStateCreateInfo _viewportInfo;
    virtual void _initViewportState();

    //  光栅化状态描述
    VkPipelineRasterizationStateCreateInfo _rasterizationInfo;
    VkPolygonMode _getPolygonMode(GfxPipelinePolygonMode polygonMode);
    VkCullModeFlags _getCullMode(GfxPipelineCullMode cullMode);
    virtual void _initRasterizationState();

    //  多样本状态描述
    VkPipelineMultisampleStateCreateInfo _multisampleInfo;
    virtual void _initMultisampleState();

    //  深度测试状态描述
    VkPipelineDepthStencilStateCreateInfo _depthStencilInfo;
    VkCompareOp _getCompareOp(GfxPipelineCompareOp compareOp);
    VkStencilOp _getStencilOp(GfxPipelineStencilOp stencilOp);
    virtual void _initDepthStencilState();

    //  颜色混合状态描述
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo _colorBlendInfo;
    VkBlendFactor _getBlendFactor(GfxPipelineColorBlendFactor blendFactor);
    VkBlendOp _getBlendOp(GfxPipelineColorBlendOp blendOp);
    virtual void _initColorBlendState();
    /**
     * @brief Vulkan 管道布局
     */
    VkPushConstantRange _pushConstantRange;
    GfxDescriptor *_descriptor;
    std::vector<VkDescriptorSetLayout> _setLayouts;
    VkPipelineLayoutCreateInfo _pipelineLayoutInfo;
    VkPipelineLayout _vkPipelineLayout;
    virtual void _initPipelineLayout();

    /**
     * @brief Vulkan 管道
     * 包含了渲染管线的所有状态，如顶点输入、绑定点、动态状态、渲染目标、深度测试、模板测试、颜色混合等
     */
    VkGraphicsPipelineCreateInfo _pipelineInfo{};
    VkPipeline _vkPipeline;
    virtual void _initPipeline();

    void _createPipeline();

public:
    GfxPipeline(const std::string &name);
    const std::string &getName();
    VkPipeline vkPipeline() { return _vkPipeline; }
    VkPipelineLayout getVKPipelineLayout() { return _vkPipelineLayout; }

    GfxDescriptor *getDescriptor() { return _descriptor; }

    void create(GfxPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, GfxPipelineStruct pipelineStruct);
    void clear();
    void reset();
    virtual ~GfxPipeline();
};