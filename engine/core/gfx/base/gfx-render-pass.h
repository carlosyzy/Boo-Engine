#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>
class GfxContext;

class GfxRenderPass
{
private:
protected:
	std::string _name;
	VkRenderPass _vkRenderPass = VK_NULL_HANDLE;
	virtual void _create();
public:
	GfxRenderPass(std::string name);
	const std::string name();
	VkRenderPass getVKRenderPass();
	void _clear();
	void _reset();
	virtual ~GfxRenderPass();
};
