#include "gfx.h"
#include "gfx-context.h"
#include "gfx-renderer.h"
#include "gfx-shader.h"
#include "gfx-texture.h"

namespace Gfx
{
    GfxContext *context = nullptr;
    GfxRenderer *renderer = nullptr;
    float time = 0.0f;
    std::map<std::string, GfxShader *> shaders;
    std::map<std::string, GfxTexture *> textures;
}