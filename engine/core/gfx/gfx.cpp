#include "gfx.h"
#include "gfx-context.h"
#include "gfx-renderer.h"
#include "base/gfx-shader.h"
#include "base/gfx-texture.h"
#include "base/gfx-mesh.h"
#include "base/gfx-buffer-ubo.h"
#include "base/gfx-buffer-instance.h"


namespace Gfx
{
    GfxContext *context = nullptr;
    GfxRenderer *renderer = nullptr;
    float time = 0.0f;
    std::map<std::string, GfxShader *> shaders;
    std::map<std::string, GfxTexture *> textures;
    GfxBufferUBO *bufferUBO = nullptr;
    GfxBufferInstance *bufferInstance = nullptr;
    GfxMesh *uiTestMesh = nullptr;
}