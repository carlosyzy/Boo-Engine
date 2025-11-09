#include "Material.h"
#include "../gfx/gfx-mgr.h"

Material::Material(const std::string uuid) : Asset(uuid),
                                            _shaderVert("resources/effects/ui/ui.vert"),
                                            _shaderFrag("resources/effects/ui/ui.frag")
{
    this->_type = AssetType::Material;
}
Material::Material(const std::string uuid, const std::string path) : Asset(uuid, path),
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
