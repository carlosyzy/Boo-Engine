// #pragma once
// #include <vector>
// #include <string>
// #include <unordered_map>
// #include <vulkan/vulkan_core.h>
// #include "gfx-struct.h"
// #include "gfx-pipeline-struct.h"


// class GfxRenderPass;
// class GfxShader;
// class GfxContext;
// // struct GfxPipelineStruct;
// // enum class GfxPipelinePolygonMode;
// // enum class GfxPipelineCullMode;
// // enum class GfxPipelineCompareOp;
// // enum class GfxPipelineStencilOp;
// // enum class GfxPipelineColorBlendFactor;
// // enum class GfxPipelineColorBlendOp;

// class GfxPipeline
// {
// private:
//     std::string _name;
//     GfxContext *_context;
//     GfxRenderPass *_pass;
//     GfxShader *_vertexShader;
//     GfxShader *_fragmentShader;
//     GfxPipelineStruct _pipelineStruct;


//     VkPipelineLayout _vkPipelineLayout;
//     VkPipeline _vkPipeline;
//     /**
//      * @brief 管道
//      */
//     void _createPipeline();
    
//     VkDescriptorSetLayout _descriptorSetLayout;
//     /**
//      * 初始化描述符集布局
//      */
//     void _createDescriptorSetLayout();

//     // void _initShaderStage(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages);
//     // void _initPipelineDepth(VkPipelineDepthStencilStateCreateInfo &depthStencil);
//     // void _initPipelineColorBlend(VkPipelineColorBlendAttachmentState &colorBlendAttachment);
// public:
//     GfxPipeline(std::string name, GfxContext *context);
//     //旧的create方法，保留兼容性
//     void create(GfxRenderPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, std::unordered_map<std::string, std::string> pipelineState);
//     //新的create方法，使用GfxPipelineStruct
//     void create(GfxRenderPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, GfxPipelineStruct pipelineStruct);
//     void clear();
//     void reset();

//     std::string getName()
//     {
//         return this->_name;
//     }
//     bool isTransparent(){
//         return true;
//     }
//     VkDescriptorSetLayout getDescriptorSetLayout()
//     {
//         return this->_descriptorSetLayout;
//     }
//     VkPipeline getVKPipeline()
//     {
//         return this->_vkPipeline;
//     }
//     VkPipelineLayout getVKPipelineLayout()
//     {
//         return this->_vkPipelineLayout;
//     }

//     VkPolygonMode getPolygonMode(GfxPipelinePolygonMode polygonMode);
//     VkCullModeFlags getCullMode(GfxPipelineCullMode cullMode);
//     VkCompareOp getCompareOp(GfxPipelineCompareOp compareOp);
//     VkStencilOp getStencilOp(GfxPipelineStencilOp stencilOp);
//     VkBlendFactor getBlendFactor(GfxPipelineColorBlendFactor blendFactor);
//     VkBlendOp getBlendOp(GfxPipelineColorBlendOp blendOp);

//     ~GfxPipeline();
// };
