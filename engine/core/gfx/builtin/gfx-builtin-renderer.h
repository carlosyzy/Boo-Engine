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
#include "../gfx-struct.h"

class GfxContext;
class GfxBuiltinRenderPass;
class GfxBuiltinQueue;
class GfxBuiltinPipeline;
class GfxBufferUBO;
class GfxBufferInstance;
class GfxRenderTexture;
class GfxMesh;
class GfxMaterial;

struct GfxRenderxUIDescriptorSet
{
	VkDescriptorSet descriptorSet;
	bool isOccupied = false;
};
struct GfxRenderx3DDescriptorSet
{
	VkDescriptorSet descriptorSet;
	bool isOccupied = false;
};

class GfxBuiltinRenderer
{
public:
	static uint32_t StencilRef;

private:
	/**
	 * @brief 渲染器名称
	 */
	std::string _name;

	/**
	 * @brief 描述符集布局
	 * 绑定ubo和采样器
	 */
	VkDescriptorSetLayout _uiDescriptorSetLayout;
	VkDescriptorSetLayout _3dDescriptorSetLayout;
	void _initDescriptorSetLayout();
	void _initUIDescriptorSetLayout();
	void _init3DDescriptorSetLayout();

	// UI渲染器描述符池
	VkDescriptorPool _uiDescriptorPool;
	// 3d渲染器描述符池
	VkDescriptorPool _3dDescriptorPool;

	std::vector<GfxRenderxUIDescriptorSet> _gfxUIDescriptorSets;
	std::vector<GfxRenderx3DDescriptorSet> _gfx3dDescriptorSets;

	void _initDescriptorSets();
	void _initUIDescriptorSets();
	void _init3DDescriptorSets();

	/**
	 * @brief 渲染传递
	 * 渲染传递
	 */
	GfxBuiltinRenderPass *_pass;
	void _initDefaultRenderPass();
	/**
	 * @brief 渲染管线
	 * 渲染管线
	 */
	std::map<std::string, GfxBuiltinPipeline *> _pipelines;

	/**
	 * @brief 渲染队列
	 * 渲染队列
	 */
	std::map<std::string, GfxBuiltinQueue *> _renderQueues;
	std::vector<GfxBuiltinQueue *> _destroyQueueCaches;

public:
	GfxBuiltinRenderer(std::string name);
	void init();
	GfxBuiltinRenderPass *getRenderPass();
	GfxBuiltinPipeline *getPipeline(const GfxRendererState &pipelineState);
	VkDescriptorSet getUIDescriptorSet();
	VkDescriptorSet get3DDescriptorSet();
	GfxBufferUBO *getUniformBuffer();

	void createPipeline(std::string name, GfxRendererState rendererState);

	void createRenderQueue(std::string renderId, int priority, uint32_t width, uint32_t height);
	void setRenderQueuePriority(std::string renderId, int priority);
	GfxRenderTexture *getRenderQueueRT(std::string renderId);
	void resizeRenderQueue(std::string renderId, uint32_t width, uint32_t height);
	void delRenderQueue(std::string renderId);

	void submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition);
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh);
	void getOffScreenOutds(std::vector<std::string> &pipelineOutds);
	void frameRendererBefore();
	void frameRenderer(std::vector<VkCommandBuffer> &commandBuffers);
	void frameRendererAfter();

	void _cleanRendererState();
	void _resetRendererState();

	~GfxBuiltinRenderer();
};