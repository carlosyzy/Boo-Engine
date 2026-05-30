#pragma once
#include <iostream>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "gfx-struct.h"
#include "../../platforms/windows/windows.h"
#include "../../platforms/android/android.h"
#include "../../platforms/macos/macos.h"
#include "../../platforms/harmonyos/harmonyos.h"

#define VK_VALIDATION_LAYER_ENABLE 0

class GfxRenderTexture;
class GfxMaterial;
class GfxMesh;
class GfxShader;
class GfxTexture;
class GfxManager
{
private:
	GfxManager(const GfxManager &) = delete;
	GfxManager &operator=(const GfxManager &) = delete;
	GfxManager();
	~GfxManager();
	size_t _currentFrame = 0;
	void _initRenderer();

public:
	static GfxManager *getInstance();
	void init(Windows *windows);
	void init(Android *android);
	void init(MacOS *macos);
	void init(HarmonyOS *harmonyos);

	void update(float dt);
	void resize(int width, int height);
	void resetSwapChain();

	void createPipeline(std::string pipelineName, GfxRendererState rendererState);
	GfxTexture *createTexture(std::string uuid, uint32_t width, uint32_t height, uint32_t channels,int textureType, const std::vector<uint8_t> *pixels, GfxTextureFormat format);
	void destroyTexture(GfxTexture *texture);


	GfxShader *createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros);
	GfxShader *createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data);
	void destroyShader(GfxShader *shader);
	GfxMesh *createMesh(std::string meshUuid, int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
	GfxMesh *createUIMesh(std::string meshUuid, int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
	void destroyMesh(GfxMesh *mesh);
	void createRenderQueue(std::string renderId, int priority, uint32_t width, uint32_t height);
	void resizeRenderQueue(std::string renderId, uint32_t width, uint32_t height);
	GfxRenderTexture *getRenderQueueRT(std::string renderId);
	void setRenderQueuePriority(std::string renderId, int priority);
	void delRenderQueue(std::string renderId);

	void setMainLitDir(float x, float y, float z, float w);
	void setMainLitColor(float r, float g, float b, float a);

	void submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition);
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh);

	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, const void *data=nullptr);
	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);
	std::vector<char> readShaderFile(const std::string &filename);
};