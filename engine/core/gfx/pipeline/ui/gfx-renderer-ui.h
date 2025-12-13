#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cstdint>
#include <unordered_map>
#include <sstream>
#include <vulkan/vulkan_core.h>
#include <shaderc/shaderc.hpp>
#include "../../gfx-renderer.h"


class GfxContext;
class GfxPassUI;
class GfxQueueUI;
class GfxPipelineUI;
class GfxBufferUBO;

struct GfxRenderxDescriptorSet
{
	VkDescriptorSet descriptorSet;
	bool isUsed = false;
};

class GfxRendererUI
{
private:
	/**
	 * @brief 渲染器名称
	 */
	std::string _name;
	uint32_t _samplerCount;
	uint32_t _maxObjectCount;
	uint32_t _currentObjectCount;
	VkDescriptorPool _descriptorPool;
	/**
	 * @brief 描述符集布局
	 * 绑定ubo和采样器
	 */
	VkDescriptorSetLayout _descriptorSetLayout;
	void _initDescriptorSetLayout();
	std::vector<GfxRenderxDescriptorSet> _gfxDescriptorSets;
	void _initDefaultDescriptor();

	GfxPassUI *_pass;
	void _initDefaultRenderPass();
	void _initDefaultShader();
	/**
	 * @brief 渲染管线
	 * 渲染管线
	 */
	std::map<std::string, GfxPipelineUI *> _pipelines;
	void _initDefaultPipeline();

	std::vector<GfxBufferUBO *> _uniformBuffers;
	void _initDefaultUniformBuffer();
	

	/**
	 * @brief 渲染队列
	 * 渲染队列
	 */
	std::map<std::string, GfxQueueUI *> _renderQueues;

	GfxMesh *_uiMesh;
	GfxMaterial *_uiMaterial;

public:
	GfxRendererUI(std::string name);
	void init();
	GfxPassUI *getRenderPass();
	GfxPipelineUI *getPipeline(std::string name);
	VkDescriptorSet &getDescriptorSet();
	GfxBufferUBO *getUniformBuffer();


	void createPipeline(std::string name, GfxPipelineStruct pipelineStruct);

	void initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture);
	void delRenderQueue(std::string renderId);
	void submitRenderMat(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix);
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData);

	void frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers,std::vector<std::string> &pipelineOutds);

	void _cleanRendererState();
	void _resetRendererState();

	~GfxRendererUI();

	// // GfxDescriptor *getDescriptor(std::string name); //{ return this->_descriptors.at(name); }
	// // GfxPass *getPass(std::string name);				// const { return this->_passes.at(name); }
	// // GfxPipeline *getPipeline(std::string name);		// const { return this->_pipelines.at(name); }
	// // GfxTexture *getTexture(std::string uuid);		// const { return this->_textures.at(uuid); }

	// // VkDescriptorSet getDescriptorSet(uint32_t index) const { return this->_descriptorSets[index]; }
	// // /**
	// //  * @brief 获取描述符集布局
	// //  *
	// //  * @return VkDescriptorSetLayout
	// //  */
	// // VkDescriptorSetLayout descriptorSetLayout() const { return this->_descriptorSetLayout; }

	// void createPipeline(std::string name, GfxPipelineStruct pipelineStruct);
	// void createUIPipeline(std::string name, GfxPipelineStruct pipelineStruct);
	// /**
	//  * 创建 or 销毁渲染纹理
	//  */
	// void createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels);
	// void insertTexture(std::string uuid, GfxTexture *texture);
	// void destroyTexture(std::string textureUuid);
	// bool isExistTexture(std::string textureUuid);
	// /**
	//  * @brief 创建着色器
	//  *
	//  * @param shaderName 着色器名称
	//  * @param buffer 着色器字节码
	//  */
	// void createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros);
	// /**
	//  * @brief 编译GLSL着色器到SPIR-V
	//  *
	//  * @param type 着色器类型
	//  * @param cacheKey 缓存键
	//  * @param source GLSL着色器源代码
	//  * @param macros 宏定义
	//  */
	// std::vector<uint32_t> compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, std::string> &macros);
	// /**
	//  * @brief 创建SPIR-V着色器
	//  */
	// void createSpirvShader(const std::string &shaderName, const std::vector<char> &data);

	// /**
	//  * @brief 初始化渲染队列
	//  *
	//  * @param renderId 渲染队列ID
	//  * @param viewMat 视图矩阵
	//  * @param projMat 投影矩阵
	//  */
	// void initRenderQueue(uint32_t renderId, GfxRenderTexture *renderTexture);
	// /**
	//  * @brief 提交渲染对象
	//  *
	//  * @param renderId 渲染队列ID
	//  * @param pass 渲染通道名称
	//  * @param pipeline 管线名称
	//  * @param vertices 顶点数据
	//  * @param indices 索引数据
	//  */
	// void submitRenderObject(uint32_t renderId, GfxMaterial &material, GfxMesh &mesh);

	// void frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
	// void cleanRendererState();
	// void resetRendererState();
	// /**
	//  * @brief 创建模型渲染对象
	//  * 旧的,过时的
	//  * @param id  物体ID
	//  * @param renderPassType 渲染通道类型
	//  * @param pipelineType 管线类型
	//  * @param points 顶点数据
	//  * @param colors 颜色数据
	//  * @param normals 法线数据
	//  * @param uvs uv数据
	//  * @param indices 索引数据
	//  */
	// void createObject(std::string id, std::string renderPassType, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices);
	// /**
	//  * @brief 创建UI渲染对象
	//  *
	//  * @param id 物体ID
	//  * @param pipelineType 管线类型
	//  * @param points 顶点数据
	//  * @param colors 颜色数据
	//  * @param normals 法线数据
	//  * @param uvs uv数据
	//  * @param indices 索引数据
	//  */
	// void createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
	// /**
	//  * @brief 创建UI遮罩渲染对象
	//  * 旧的,过时的
	//  * @param id 物体ID
	//  * @param pipelineType 管线类型
	//  * @param points 顶点数据
	//  * @param colors 颜色数据
	//  * @param normals 法线数据
	//  * @param uvs uv数据
	//  * @param indices 索引数据
	//  */
	// void createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
	// void setObjectPass(std::string id, std::string pass);
	// void setObjectPipeline(std::string id, std::string pipeline);
	// /**
	//  * @brief 设置UI遮罩行为
	//  *
	//  * @param id 物体ID
	//  * @param behavior 行为 0 不遮罩 1 遮罩
	//  */
	// void setObjectUIMaskBehavior(std::string id, uint32_t behavior);
	// void setObjectModelMatrix(std::string id, std::array<float, 16> modelMatrix);
	// void setObjectViewMatrix(std::string id, std::array<float, 16> viewMatrix);
	// void setObjectProjMatrix(std::string id, std::array<float, 16> projMatrix);
	// void setObjectTexture(const std::string &id, const std::string &texture);
	// void setObjectColor(std::string id, float r, float g, float b, float a);
	// void destroyObject(std::string id);

	// /*   // /**
	//   //  * @brief 重置模型渲染对象状态
	//   //  *
	//   //  * @param id 物体ID
	//   //  * @param renderPassType 渲染通道类型
	//   //  * @param pipelineType 管线类型
	//   //  */
	// // void resetGfxObjectRendererState(std::string id, std::string renderPassType, std::string pipelineType); */

	// /**
	//  * @brief 提交渲染对象
	//  * @param id 物体ID
	//  */
	// void submitObjectRender(std::string id);
};