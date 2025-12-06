#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>
class GfxContext;

class GfxPass
{
private:
protected:
	std::string _name;
	VkRenderPass _vkRenderPass = VK_NULL_HANDLE;
	virtual void _create();
public:
	GfxPass(std::string name);
	const std::string name()
	{
		return this->_name;
	}
	VkRenderPass vkRenderPass()
	{
		return this->_vkRenderPass;
	}
	void clear();
	void reset();
	virtual ~GfxPass();
};
