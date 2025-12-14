#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "../base/gfx-render-pass.h"

class GfxRenderPassDefault : public GfxRenderPass
{
private:
protected:
	virtual void _create();

public:
	GfxRenderPassDefault(std::string name);
	~GfxRenderPassDefault() override;
};
