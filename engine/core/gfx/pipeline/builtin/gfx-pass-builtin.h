#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "../../gfx-pass.h"


class GfxPassBuiltin : public GfxPass
{
private:
protected:
	std::string _name;
	VkRenderPass _vkRenderPass = VK_NULL_HANDLE;
	virtual void _create();
public:
	GfxPassBuiltin(std::string name);
	virtual ~GfxPassBuiltin();
};
