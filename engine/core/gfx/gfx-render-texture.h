#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <functional>
#include <vulkan/vulkan_core.h>

class GfxTexture;
class GfxPass;

class GfxRenderTexture
{
private:
	std::string _uuid;
	uint32_t _width;
	uint32_t _height;

	std::string _colorUuid;
	std::string _depthUuid;
	GfxTexture* _colorTexture;
	GfxTexture* _depthTexture;

	GfxPass* _pass;
	VkFramebuffer _framebuffer;
	VkCommandBuffer _commandBuffer;
	std::string _generateUUID();
	virtual void _createTextures();
	virtual void _createFramebuffer();
	virtual void _createCommandBuffer();

public:
	GfxRenderTexture(std::string uuid);
	VkFramebuffer& getFramebuffer();
	VkCommandBuffer& getCommandBuffer();
	const uint32_t& getWidth() const;
	const uint32_t& getHeight() const;
	void bindRenderPass(GfxPass* pass);

	void resize(uint32_t width, uint32_t height);
	void _clear();
	void _reset();
	bool saveToFile1(std::string filePath);
	~GfxRenderTexture();
};