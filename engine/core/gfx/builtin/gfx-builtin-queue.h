#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <array>
#include <cstdint>
#include "../gfx-struct.h"

class GfxPipeline;
class GfxBuiltinRenderer;
class GfxBuiltinBatch;
class GfxBuiltinBatchUI;
class GfxBuiltinBatch3D;
class GfxBuiltinPipeline;
class GfxRenderTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuffer;

class GfxBuiltinQueue
{
private:
	std::string _rendererId;
	GfxBuiltinRenderer *_renderer;
	GfxRenderTexture *_renderTexture;
	int _priority;
	bool _isOnScreen;
	std::array<float, 16> _viewMatrix;
	std::array<float, 16> _projMatrix;
	std::array<float, 4> _cameraPosition;

	std::vector<GfxBuiltinBatchUI *> _uiBatches;

	std::vector<GfxMesh *> _3dOpaqueMeshes;
	std::vector<GfxMaterial *> _3dOpaqueMaterials;
	std::unordered_map<std::string, GfxBuiltinBatch3D *> _3dOpaqueBatches;
	std::vector<GfxBuiltinBatch3D *> _3dTransparentBatches;
	void _submitObjectUI(GfxMaterial *material, GfxMesh *mesh);
	void _submitObject3DOpaque(GfxMaterial *material, GfxMesh *mesh);
	void _submitObject3DTransparent(GfxMaterial *material, GfxMesh *mesh);

	/**
	 * @brief 重置命令缓冲区
	 * 渲染第一步
	 */
	void _resetCommandBuffer();
	/**
	 * @brief 开始命令缓冲区
	 * 渲染第二步
	 */
	void _beginCommandBuffer();
	/**
	 * @brief 开始渲染传递
	 * 渲染第三步
	 */
	void _beginRenderPass();

public:
	GfxBuiltinQueue(std::string renderId, uint32_t width, uint32_t height, GfxBuiltinRenderer *renderer);
	void init();
	void setPriority(int priority);
	int getPriority() const;
	void resize(uint32_t width, uint32_t height);
	void submitData(const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition);
	void submitObject(GfxMaterial *material, GfxMesh *mesh);
	bool getIsOnScreen();
	void render(std::vector<VkCommandBuffer> &commandBuffers);
	GfxRenderTexture *getRenderTexture();
	void _clean();
	void _reset();
	void destroy();
	~GfxBuiltinQueue();
};