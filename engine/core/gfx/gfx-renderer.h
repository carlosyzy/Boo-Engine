#pragma once
#include <iostream>
#include <regex>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <set>
#include <map>
#include <cstdint>
#include <unordered_map>
#include <sstream>
#include "gfx-struct.h"

class GfxContext;
class GfxDefaultRenderer;
class GfxTexture;
class GfxRenderPass;
class GfxShader;
class GfxPipeline;
class GfxRenderTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuiltinRenderer;

class GfxRenderer
{
private:
	GfxDefaultRenderer *_defaultRenderer;
	GfxBuiltinRenderer *_builtinRenderer;
	std::vector<GfxTexture *> _destroyTextureCaches;
	std::vector<GfxShader *> _destroyShaderCaches;
	std::vector<GfxRenderTexture *> _destroyRenderTextureCaches;
	std::vector<GfxMesh *> _destroyMeshCaches;
	/**
	 * @brief 编译GLSL着色器到SPIR-V
	 *
	 * @param type 着色器类型
	 * @param cacheKey 缓存键
	 * @param source GLSL着色器源代码
	 * @param macros 宏定义
	 */
	std::vector<uint32_t> _compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, int> &macros);
	
public:
	GfxRenderer();
	void init();

	void createPipeline(std::string name, GfxRendererState rendererState);

	/**
	 * 创建 or 销毁渲染纹理
	 */
	GfxTexture *createTexture(std::string uuid, uint32_t width, uint32_t height, uint32_t channels,int textureType, const std::vector<uint8_t> *pixels, GfxTextureFormat format);
	void insertTexture(GfxTexture *texture);
	void destroyTexture(GfxTexture *texture);
	
	/**
	 * @brief 创建SPIR-V着色器
	 */
	GfxShader *createSpirvShader(const std::string &uuid, const std::vector<uint32_t> &data);
	/**
	 * @brief 创建着色器
	 *
	 * @param shaderName 着色器名称
	 * @param buffer 着色器字节码
	 */
	GfxShader *createGlslShader(const std::string &uuid, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros);
	
	/**
	 * @brief 销毁着色器
	 */
	void destroyShader(GfxShader *shader);
	/**
	 * @brief 创建网格
	 */
	GfxMesh *createMesh(std::string meshUuid, int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
	/**
	 * @brief 创建UI网格
	 */
	GfxMesh *createUIMesh(std::string meshUuid,int meshType,  const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
	/**
	 * @brief 销毁网格
	 */
	void destroyMesh(GfxMesh *mesh);

	void createRenderQueue(std::string renderId, int priority, uint32_t width, uint32_t height);
	void setRenderQueuePriority(std::string renderId, int priority);
	GfxRenderTexture *getRenderQueueRT(std::string renderId);
	void resizeRenderQueue(std::string renderId, uint32_t width, uint32_t height);
	void delRenderQueue(std::string renderId);

	void submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition);
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh);
	
	void frameRendererBefore();
	void frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
	void frameRendererAfter();

	void _clearDestroyCaches();
	void _cleanRendererState();
	void _resetRendererState();
	

	~GfxRenderer();
};