#pragma once
#include <map>
#include <vulkan/vulkan_core.h>

class GfxContext;
class GfxRenderer;
class GfxShader;
class GfxTexture;

namespace Gfx
{
	extern GfxContext *context;
	extern GfxRenderer *renderer;
	extern float time;
	extern std::map<std::string, GfxShader *> shaders;
	extern std::map<std::string, GfxTexture *> textures;
}