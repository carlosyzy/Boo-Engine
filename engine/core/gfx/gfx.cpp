#include "gfx.h"
#include "gfx-context.h"
#include "gfx-renderer.h"
#include "gfx-shader.h"
#include "gfx-texture.h"
#include "gfx-buffer-ubo.h"
#include "gfx-buffer-instance.h"

namespace Gfx
{
    GfxContext *context = nullptr;
    GfxRenderer *renderer = nullptr;
    float time = 0.0f;
    std::map<std::string, GfxShader *> shaders;
    std::map<std::string, GfxTexture *> textures;
    GfxBufferUBO *bufferUBO = nullptr;
    GfxBufferInstance *bufferInstance = nullptr;
}