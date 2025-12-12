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

#include "gfx-struct.h"
#include "gfx-pipeline-struct.h"
#include "../assets/texture-asset.h"

class GfxContext;
class GfxRendererBuiltin;
class GfxTexture;
class GfxPass;
class GfxShader;
class GfxPipeline;

class GfxRenderer
{
private:
	GfxRendererBuiltin *_renderPipelineBuiltin;

public:
	GfxRenderer();
	void init();
	/**
	 * 创建 or 销毁渲染纹理
	 */
	void createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels);
	void insertTexture(std::string uuid, GfxTexture *texture);
	void destroyTexture(std::string textureUuid);
	bool isExistTexture(std::string textureUuid);
	GfxTexture *getTexture(std::string uuid);
	/**
	 * @brief 创建着色器
	 *
	 * @param shaderName 着色器名称
	 * @param buffer 着色器字节码
	 */
	void createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros);
	/**
	 * @brief 编译GLSL着色器到SPIR-V
	 *
	 * @param type 着色器类型
	 * @param cacheKey 缓存键
	 * @param source GLSL着色器源代码
	 * @param macros 宏定义
	 */
	std::vector<uint32_t> compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, std::string> &macros);
	/**
	 * @brief 创建SPIR-V着色器
	 */
	void createSpirvShader(const std::string &shaderName, const std::vector<char> &data);

	void initRenderQueue(std::string pipelineName, uint32_t renderId);
	void submitRenderObject(const std::string &pipelineName, uint32_t renderId, const GfxMaterial &material, const GfxMesh &mesh);
	void frameRenderer( uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);

	void _cleanRendererState();
	void _resetRendererState();

	~GfxRenderer();
};
// private:
// 	float _time;
// 	std::string _name;

// 	std::map<std::string, GfxDescriptor *> _descriptors;

//
// 	/**
// 	 * @brief 渲染通道
// 	 * 渲染通道
// 	 */
// 	std::map<std::string, GfxPass *> _passes;

// 	/**
// 	 * @brief 渲染队列
// 	 * 渲染队列-一个摄像机一个队列
// 	 * uint32_t 摄像机的可见id
// 	 */
// 	GfxRenderQueue *_defaultQueue;
// 	std::map<uint32_t, GfxRenderQueue *> _queues;

// 	/**
// 	 * @brief 渲染缓冲区
// 	 * 渲染缓冲区
// 	 */
// 	std::map<VkDeviceSize, std::vector<GfxBufferUBO *>> _uboBuffers;
// 	std::map<VkDeviceSize, std::vector<GfxBufferSSBO *>> _ssboBuffers;

// 	TextureAsset *_textTexture;

// 	/**
// 	 * 描述符相关
// 	 */
// 	void _initDescriptor();
// 	void _initDescriptorPool();
// 	void _initDescriptorSetLayout();
// 	void _initDescriptorSets();

// 	/**
// 	 * @brief 初始化默认通道
// 	 */
// 	void _initDefaultPasses();
// 	void _initDefaultShaders();
//
// 	// void _initDefaultUIMaskPipeline();
// 	// // 初始化默认渲染队列
// 	void _initDefaultRenderQueue();
// 	// // 帧缓冲区:它连接了渲染通道（Render Pass） 和交换链图像（Swap Chain Images）
// 	// std::vector<VkFramebuffer> _framebuffers;
// 	// // 命令缓冲区是用于记录和执行 GPU 命令的内存块。在 Vulkan 中，几乎所有渲染操作都需要通过命令缓冲区来执行。
// 	// std::vector<VkCommandBuffer> _commandBuffers;
// 	// void _createBuffers();
// 	// void _createFramebuffers();
// 	// void _createCommandBuffers();

// public:
// 	GfxRenderer(std::string name);
// 	void init();

// 	GfxDescriptor *getDescriptor(std::string name); //{ return this->_descriptors.at(name); }
// 	GfxPass *getPass(std::string name);				// const { return this->_passes.at(name); }
// 		// const { return this->_pipelines.at(name); }
// 		// const { return this->_textures.at(uuid); }

// 	// VkDescriptorSet getDescriptorSet(uint32_t index) const { return this->_descriptorSets[index]; }
// 	// /**
// 	//  * @brief 获取描述符集布局
// 	//  *
// 	//  * @return VkDescriptorSetLayout
// 	//  */
// 	// VkDescriptorSetLayout descriptorSetLayout() const { return this->_descriptorSetLayout; }

//
// 	void createUIPipeline(std::string name, GfxPipelineStruct pipelineStruct);

// 	// /**
// 	//  * @brief 创建模型渲染对象
// 	//  * 旧的,过时的
// 	//  * @param id  物体ID
// 	//  * @param renderPassType 渲染通道类型
// 	//  * @param pipelineType 管线类型
// 	//  * @param points 顶点数据
// 	//  * @param colors 颜色数据
// 	//  * @param normals 法线数据
// 	//  * @param uvs uv数据
// 	//  * @param indices 索引数据
// 	//  */
// 	// void createObject(std::string id, std::string renderPassType, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices);
// 	// /**
// 	//  * @brief 创建UI渲染对象
// 	//  *
// 	//  * @param id 物体ID
// 	//  * @param pipelineType 管线类型
// 	//  * @param points 顶点数据
// 	//  * @param colors 颜色数据
// 	//  * @param normals 法线数据
// 	//  * @param uvs uv数据
// 	//  * @param indices 索引数据
// 	//  */
// 	// void createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
// 	// /**
// 	//  * @brief 创建UI遮罩渲染对象
// 	//  * 旧的,过时的
// 	//  * @param id 物体ID
// 	//  * @param pipelineType 管线类型
// 	//  * @param points 顶点数据
// 	//  * @param colors 颜色数据
// 	//  * @param normals 法线数据
// 	//  * @param uvs uv数据
// 	//  * @param indices 索引数据
// 	//  */
// 	// void createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
// 	// void setObjectPass(std::string id, std::string pass);
// 	// void setObjectPipeline(std::string id, std::string pipeline);
// 	// /**
// 	//  * @brief 设置UI遮罩行为
// 	//  *
// 	//  * @param id 物体ID
// 	//  * @param behavior 行为 0 不遮罩 1 遮罩
// 	//  */
// 	// void setObjectUIMaskBehavior(std::string id, uint32_t behavior);
// 	// void setObjectModelMatrix(std::string id, std::array<float, 16> modelMatrix);
// 	// void setObjectViewMatrix(std::string id, std::array<float, 16> viewMatrix);
// 	// void setObjectProjMatrix(std::string id, std::array<float, 16> projMatrix);
// 	// void setObjectTexture(const std::string &id, const std::string &texture);
// 	// void setObjectColor(std::string id, float r, float g, float b, float a);
// 	// void destroyObject(std::string id);

// 	// /*   // /**
// 	//   //  * @brief 重置模型渲染对象状态
// 	//   //  *
// 	//   //  * @param id 物体ID
// 	//   //  * @param renderPassType 渲染通道类型
// 	//   //  * @param pipelineType 管线类型
// 	//   //  */
// 	// // void resetGfxObjectRendererState(std::string id, std::string renderPassType, std::string pipelineType); */

// 	// /**
// 	//  * @brief 提交渲染对象
// 	//  * @param id 物体ID
// 	//  */
// 	// void submitObjectRender(std::string id);

// 	~GfxRenderer();
// };