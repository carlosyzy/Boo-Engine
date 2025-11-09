#include "Material.h"
#include "../gfx/gfx-mgr.h"

Material::Material(const std::string key) : Asset(key),
                                            _shaderVert("resources/effects/ui/ui.vert"),
                                            _shaderFrag("resources/effects/ui/ui.frag")
{
    this->_type = AssetType::Material;
}
Material::Material(const std::string key, const std::string path) : Asset(key, path),
                                                                    _shaderVert("resources/effects/ui/ui.vert"),
                                                                    _shaderFrag("resources/effects/ui/ui.frag")
{
    this->_type = AssetType::Material;
    this->_load();
}
void Material::_load()
{
}
void Material::destroy()
{
}
