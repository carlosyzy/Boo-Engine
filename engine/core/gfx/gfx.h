#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan_core.h>

class GfxContext;
class GfxRenderer;
class GfxShader;
class GfxTexture;
class GfxBufferUBO;
class GfxBufferInstance;
class GfxMesh;

namespace Gfx
{
	extern GfxContext *context;
	extern GfxRenderer *renderer;
	extern float time;
	extern std::map<std::string, GfxShader *> shaders;
	extern std::map<std::string, GfxTexture *> textures;
	extern GfxBufferUBO *bufferUBO;
	extern GfxBufferInstance *bufferInstance;
	extern GfxMesh *uiTestMesh;
}