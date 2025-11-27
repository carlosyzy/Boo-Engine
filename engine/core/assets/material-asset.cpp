#include "material-asset.h"
#include "../gfx/gfx-mgr.h"

MaterialAsset::MaterialAsset(const std::string uuid, const std::string path) : Asset(uuid, path)
{
	this->_type = AssetType::Material;
	this->_pass = "built-ui";
	this->_pipeline = "built-ui";
	this->_load();
}
void MaterialAsset::_load()
{

}
void MaterialAsset::destroy()
{
}
