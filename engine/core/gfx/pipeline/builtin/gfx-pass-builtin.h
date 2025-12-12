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
	virtual void _create();
public:
	GfxPassBuiltin(std::string name);
	~GfxPassBuiltin() override;
};
