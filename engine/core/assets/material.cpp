#include "material.h"
#include "../gfx/gfx-mgr.h"

Material::Material(const std::string uuid) : Asset(uuid),
											 _vert("resources/effects/ui/ui.vert"),
											 _frag("resources/effects/ui/ui.frag")
{
	this->_type = AssetType::Material;
}
Material::Material(const std::string uuid, const std::string path) : Asset(uuid, path),
																	 _vert("resources/effects/ui/ui.vert"),
																	 _frag("resources/effects/ui/ui.frag")
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
