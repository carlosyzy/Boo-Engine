#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
#include "gfx-struct.h"

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
