#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
#include <array>
#include "gfx-struct.h"

class GfxPipeline;
class GfxTexture;
class GfxContext;
class GfxPass;

class GfxObject
{
private:
    GfxContext *_context;
    GfxPipeline *_pipeline;
    GfxPass *_pass;
    GfxTexture *_texture;
    
    
    float _color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    /**
     * @brief 模型矩阵
     */
    std::array<float, 16> _modelMatrix;
    /**
     * @brief 视图矩阵
     */
    std::array<float, 16> _viewMatrix;
    /**
     * @brief 投影矩阵
     */
    std::array<float, 16> _projMatrix;
    //UI对象的裁剪区域
    std::array<float, 5> _uiMask;


    int _indexSize;
    VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;

    /*  // 现有的成员变量... */
    std::vector<VkBuffer> _uniformBuffers;
    std::vector<VkDeviceMemory> _uniformBuffersMemory;
    std::vector<void*> _uniformBuffersMapped;
   /*  // 现有的方法... */
    void _createUniformBuffers();
    void _cleanUniformBuffers();
    uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


    std::vector<VkDescriptorSet> _descriptorSets;
    void _updateDescriptorSet();

    /* //更新UBO中的模型矩阵 */
    void _updateModelMatUniformBuffer();
    /* //更新UBO中的视图矩阵 */
    void _updateViewMatUniformBuffer();
   /*  //更新UBO中的投影矩阵 */
    void _updateProjMatUniformBuffer();

    void _Log(std::string msg);

public:
    GfxObject(GfxContext *context);
    void setPass(GfxPass *pass);
    void setVertexs(std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices);
    void setPipeline(GfxPipeline *pipeline);
    void setColor(float r, float g, float b, float a);
    void setTexture(GfxTexture *texture);
    void setModelMatrix(std::array<float, 16>& mat);
    void setViewMatrix(std::array<float, 16>& mat);
    void setProjMatrix(std::array<float, 16>& mat);
    void setUIMask(float x, float y, float width, float height, float angle);

     void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
    /**
     * @brief 提交绑定顶点,索引命令
     */
    void submitCmdBindVertexAndIndex();
    /**
     * @brief 提交绑定描述符集命令
     */
    void submitCmdBindPipeline(); 
    /**
     * @brief 提交绑定描述符集命令
     */
    void submitCmdBindDescriptorSet();
    /**
     * @brief 提交推送常量命令
     */
    void submitCmdPushConstants();
    /**
     * @brief 提交绘制命令
     */
    void submitCmdDraw();

    void clear();
    void reset();
    

    GfxPipeline *pipeline(){
        return this->_pipeline;
    }
    GfxPass *pass(){
        return this->_pass;
    }

    /**
     * @brief 销毁GfxObject
     */
    void destroy();
    ~GfxObject();
};
