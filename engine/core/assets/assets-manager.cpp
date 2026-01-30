#include "assets-manager.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#include <mach-o/dyld.h> // 必需的头文件
#endif
#include "../utils/time-util.h"
#include "asset.h"

AssetsManager::AssetsManager()
{
	this->_assetsCache = new AssetCache();
	this->_assetsLoad = new AssetLoad(this);
	this->_initDefaultBuiltinAssets();
}
void AssetsManager::init()
{
}
void AssetsManager::_initDefaultBuiltinAssets()
{
	//默认纹理
	TextureAsset *texture = new TextureAsset("_private/default.png");
	texture->create(GfxTextureDefault, sizeof(GfxTextureDefault));
	this->_assetsCache->addAsset("_private/default.png", texture);
	TextureAsset *logo = new TextureAsset("_private/logo.png");
	logo->create(GfxTextureLogo, sizeof(GfxTextureLogo));
	this->_assetsCache->addAsset("_private/logo.png", logo);
	// 默认材质
	// MaterialAsset *material = new MaterialAsset("_private/default");
	// material->create();
	// this->_assetsCache->addAsset("_private/default", material);

}
void AssetsManager::setAssetsRoot(const std::string &root)
{
	this->_assetsRoot = root;
}
const std::string &AssetsManager::getAssetsRoot()
{
	return this->_assetsRoot;
}

AssetLoad *AssetsManager::getAssetsLoad()
{
	return this->_assetsLoad;
}
AssetCache *AssetsManager::getAssetsCache()
{
	return this->_assetsCache;
}
void AssetsManager::setMaxLoadCount(int count)
{
	this->_assetsLoad->setMaxLoadCount(count);
}
Asset *AssetsManager::loadAsset(const std::string &path)
{
	return this->_assetsLoad->loadAsset(path);
}

Asset *AssetsManager::getAsset(const std::string &path)
{
	return this->_assetsCache->getAsset(path);
}
void AssetsManager::update(float deltaTime)
{
	this->_assetsLoad->update(deltaTime);
}

AssetsManager::~AssetsManager()
{
}