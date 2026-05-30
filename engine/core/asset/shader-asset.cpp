#include "shader-asset.h"
#include <filesystem>
#include "core/gfx/gfx-manager.h"
#include "core/gfx/base/gfx-shader.h"

namespace Boo
{
    ShaderAsset::ShaderAsset()
    {
        this->_type = EAssetType::Shader;
    }
    ShaderAsset::ShaderAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = EAssetType::Shader;
    }

    ShaderAsset::ShaderAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name),
                                                                                     _glslData("")
    {
        this->_type = EAssetType::Shader;
    }

    void ShaderAsset::create(const std::string &type, const std::string &glslData, const std::map<std::string, int> &macros)
    {
        this->_glslData = glslData;
        this->gfxShader = GfxManager::getInstance()->createGlslShader(this->_uuid, type, this->_glslData, macros);
    }
    void ShaderAsset::create(const std::vector<uint32_t> &data)
    {
            this->gfxShader = GfxManager::getInstance()->createSpirvShader(this->_uuid, data);
    }

    void ShaderAsset::destroy()
    {
        GfxManager::getInstance()->destroyShader(this->gfxShader);
        this->gfxShader = nullptr;
    }

} // namespace Boo
