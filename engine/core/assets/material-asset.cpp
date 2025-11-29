#include "material-asset.h"
#include "../gfx/gfx-mgr.h"
#include "../utils/uuid-util.h"

MaterialAsset::MaterialAsset() : Asset()
{
	this->_type = AssetType::Material;
	this->_pass = "built-ui";
	this->_pipeline = "built-ui";
	
}
MaterialAsset::MaterialAsset(const std::string uuid) : Asset(uuid)
{
	this->_type = AssetType::Material;
	this->_pass = "built-ui";
	this->_pipeline = "built-ui";
	
}
void MaterialAsset::create(std::string path) 
{
    Asset::create(path);
}
void MaterialAsset::destroy()
{
}
