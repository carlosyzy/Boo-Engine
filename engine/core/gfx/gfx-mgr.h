#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "base/gfx-pipeline-struct.h"

class GfxRenderTexture;
class GfxMaterial;
class GfxMesh;

class GfxMgr
{
private:
	GfxMgr(const GfxMgr &) = delete;			/* // 禁用拷贝构造 */
	GfxMgr &operator=(const GfxMgr &) = delete; /* // 禁用赋值操作符 */
	GfxMgr();
	~GfxMgr();
	size_t _currentFrame = 0;

public:
	static GfxMgr *getInstance();
	/**
	 * 初始化
	 */
	void init();

	void update(float dt);
	void resetSwapChain();

	/**
	 * @brief 创建纹理
	 *
	 * @param textureUuid 纹理UUID
	 * @param width 宽度
	 * @param height 高度
	 * @param channels 通道数
	 * @param pixels 像素数据
	 */
	void createTexture(std::string texture, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels);
	/**
	 * @brief 销毁纹理
	 *
	 * @param textureUuid 纹理UUID
	 */
	void destroyTexture(std::string texture);
	/**
	 * @brief 判断纹理是否存在
	 *
	 * @param texture 纹理UUID
	 * @return true 存在
	 * @return false 不存在
	 */
	bool isExistTexture(std::string texture);

	/**
	 * @brief 创建着色器
	 *
	 * @param shaderName 着色器名称
	 * @param buffer 着色器字节码
	 */
	void createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros);
	/**
	 * @brief 创建SPIR-V着色器
	 */
	void createSpirvShader(const std::string &shaderName, const std::vector<char> &data);

	/**
	 * @brief 初始化渲染队列
	 *
	 * @param renderId 渲染队列ID
	 * @param viewMat 视图矩阵
	 * @param projMat 投影矩阵
	 */
	void initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture);
	/**
	 * @brief 销毁渲染队列
	 *
	 * @param renderId 渲染队列ID
	 */
	void delRenderQueue(std::string renderId);
	
	void submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen);
	/**
	 * @brief 提交渲染对象
	 *
	 * @param renderId 渲染队列ID
	 * @param pass 渲染通道名称
	 * @param pipeline 管线名称
	 * @param vertices 顶点数据
	 * @param indices 索引数据
	 */
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData);

	// void initTestInfo();
	// /*  // 采样次数 */
	// void _testMSAASample();
	// VkSampleCountFlagBits _getMaxMSAAUsableSampleCount();
	// /**
	//  * @brief 测试Bindless
	//  */
	// void _testBindless();

	// /**
	//  * 创建渲染通道
	//  */
	// void createRenderPass(std::string name, GfxPassStruct passStruct);
	// /**
	//  * 创建管线
	//  */
	// void createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct);

	// /**
	//  * @brief 创建模型渲染对象
	//  *
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
	//  *
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
	// /**
	//  * @brief 销毁模型渲染对象
	//  *
	//  * @param id 物体ID
	//  * @param renderPassType 渲染通道类型
	//  * @param pipelineType 管线类型
	//  */
	// void destroyObject(std::string id);
	// void setObjectModelMatrix(std::string id, const std::array<float, 16> &modelMatrix);
	// void setObjectViewMatrix(std::string id, const std::array<float, 16> &viewMatrix);
	// void setObjectProjMatrix(std::string id, const std::array<float, 16> &projMatrix);
	// void setObjectTexture(const std::string &id, const std::string &texture);
	// void setObjectColor(std::string id, float r, float g, float b, float a);

	// void addUIObjectMask(std::string id, std::string maskId, std::vector<float> mask);
	// void submitUIObjectMask(std::string id);

	/**
	 * @brief 提交渲染对象
	 * @param id 物体ID
	 */

	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, void *data);
	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);
	std::vector<char> readShaderFile(const std::string &filename);
};