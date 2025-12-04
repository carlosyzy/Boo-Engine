// #pragma once
// #include <vector>
// #include <vulkan/vulkan_core.h>
// #include <array>
// #include <map>
// #include "gfx-struct.h"

// class GfxPipeline;
// class GfxTexture;
// class GfxContext;
// class GfxPass;
// enum class GfxObjectType;
// /**
//  * 渲染对象包含
//  * UI 对象
//  *   UI_MASK
//  *   UI_DEFAULT
//  * 3D对象
//  */

// class GfxObject
// {
// private:
//     /**
//      * @brief 渲染对象唯一标识
//      */
//     std::string _uuid;
//     /**
//      * @brief 渲染对象类型
//      */
//     GfxObjectType _type;
//     GfxContext *_context;
//     GfxPipeline *_pipeline;
//     GfxPass *_pass;
//     GfxTexture *_texture;

//     /**
//      * @brief UI遮罩行为 0 不遮罩 1 遮罩
//      */
//     uint32_t _uiMaskBehavior = 0;

//     float _color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
//     /**
//      * @brief 模型矩阵
//      */
//     std::array<float, 16> _modelMatrix;
//     /**
//      * @brief 视图矩阵
//      */
//     std::array<float, 16> _viewMatrix;
//     /**
//      * @brief 投影矩阵
//      */
//     std::array<float, 16> _projMatrix;
    
//     int _indexSize;
//     VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
//     VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
//     VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
//     VkDeviceMemory _indexMemory = VK_NULL_HANDLE;

//     /*  // 现有的成员变量... */
//     std::vector<VkBuffer> _uniformBuffers;
//     std::vector<VkDeviceMemory> _uniformBuffersMemory;
//     std::vector<void *> _uniformBuffersMapped;
//     /*  // 现有的方法... */
//     void _createUniformBuffers();
//     void _cleanUniformBuffers();
//     uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

//     std::vector<VkDescriptorSet> _descriptorSets;
//     void _updateDescriptorSet();

//     /* //更新UBO中的模型矩阵 */
//     void _updateModelMatUniformBuffer();
//     /* //更新UBO中的视图矩阵 */
//     void _updateViewMatUniformBuffer();
//     /*  //更新UBO中的投影矩阵 */
//     void _updateProjMatUniformBuffer();

//     void _Log(std::string msg);

// public:
//     /**
//      * @brief 渲染对象构造函数
//      *
//      * @param uuid 渲染对象唯一标识
//      * @param type 渲染对象类型
//      * @param context 渲染上下文
//      */
//     GfxObject(std::string uuid, GfxObjectType type, GfxContext *context);
//     void setVertexs(std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
//     void setPass(GfxPass *pass);
//     void setPipeline(GfxPipeline *pipeline);
//     void setUIMaskPipeline(GfxPipeline *pipelineAdd, GfxPipeline *pipelineSub);
//     /**
//      * @brief 设置UI遮罩行为
//      *
//      * @param behavior 行为 0 不遮罩 1 遮罩
//      */
//     void setUIMaskBehavior(uint32_t behavior);

//     void setColor(float r, float g, float b, float a);
//     void setTexture(GfxTexture *texture);
//     void setModelMatrix(std::array<float, 16> &mat);
//     void setViewMatrix(std::array<float, 16> &mat);
//     void setProjMatrix(std::array<float, 16> &mat);

//     void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
//     /**
//      * @brief 提交绑定顶点,索引命令
//      */
//     void submitCmdBindVertexAndIndex();
//     /**
//      * @brief 提交绑定描述符集命令
//      */
//     void submitCmdBindPipeline();
//     /**
//      * @brief 提交绑定描述符集命令
//      */
//     void submitCmdBindDescriptorSet();
//     /**
//      * @brief 提交推送常量命令
//      */
//     void submitCmdPushConstants();
//     /**
//      * @brief 提交绘制命令
//      */
//     void submitCmdDraw();

//     void clear();
//     void reset();

//     /**
//      * @brief 获取渲染对象唯一标识
//      *
//      * @return std::string 渲染对象唯一标识
//      */
//     std::string getUuid()
//     {
//         return this->_uuid;
//     }
//     GfxPipeline *getPipeline()
//     {
//         return this->_pipeline;
//     }
//     GfxTexture *getTexture()
//     {
//         return this->_texture;
//     }
//     GfxPass *getPass()
//     {
//         return this->_pass;
//     }
//     GfxObjectType getType()
//     {
//         return this->_type;
//     }
//     uint32_t getUIMaskBehavior()
//     {
//         return this->_uiMaskBehavior;
//     }

//     /**
//      * @brief 销毁GfxObject
//      */
//     void destroy();
//     ~GfxObject();
// };
