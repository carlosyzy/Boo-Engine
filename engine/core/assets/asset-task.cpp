
#include "asset-task.h"
#include <filesystem>
#include <iostream>
#include "texture-asset.h"
#include "shader-asset.h"
#include "scene-asset.h"
#include "assets-manager.h"

#include "../utils/time-util.h"

AssetTask::AssetTask(AssetsManager *mgr, int id)
{
	this->_mgr = mgr;
	this->_isComplete = false;
	this->_id = id;
}
Asset *AssetTask::load(const std::string &path)
{
	// this->_type = AssetTaskType::Sync;
	this->_assetPath = path;
	this->_assetExtension = std::filesystem::path(path).extension().string();
	if (this->_assetExtension == ".png" || this->_assetExtension == ".PNG" || this->_assetExtension == ".jpg" || this->_assetExtension == ".JPG" || this->_assetExtension == ".jpeg" || this->_assetExtension == ".JPEG")
	{
		return this->_createTexture();
	}
	else if (this->_assetExtension == ".scene" || this->_assetExtension == ".SCENE" || this->_assetExtension == ".Scene")
	{
		return this->_createScene();
	}

	return nullptr;
}

Asset *AssetTask::_createTexture()
{
	std::filesystem::path fullPath = (std::filesystem::path(this->_mgr->getAssetsRoot()) / this->_assetPath).generic_string();
	if (!std::filesystem::exists(fullPath))
	{
		std::cerr << "AssetLoad:No such file or directory:" << fullPath << std::endl;
		this->_loadError();
		return nullptr;
	}
	if (!std::filesystem::is_regular_file(fullPath))
	{
		std::cerr << "AssetLoad:Not a regular file:" << fullPath << std::endl;
		this->_loadError();
		return nullptr;
	}
	TextureAsset *texture = new TextureAsset(this->_assetPath);
	texture->create(fullPath.string());
	return texture;
}
Asset *AssetTask::_createScene()
{
	std::filesystem::path fullPath = (std::filesystem::path(this->_mgr->getAssetsRoot()) / this->_assetPath).generic_string();
	if (!std::filesystem::exists(fullPath))
	{
		std::cerr << "AssetLoad:No such file or directory:" << fullPath << std::endl;
		this->_loadError();
		return nullptr;
	}
	if (!std::filesystem::is_regular_file(fullPath))
	{
		std::cerr << "AssetLoad:Not a regular file:" << fullPath << std::endl;
		this->_loadError();
		return nullptr;
	}
	SceneAsset *scene = new SceneAsset(this->_assetPath);
	scene->create(fullPath.string());
	return scene;
}

// void AssetTask::run()
// {
// 	std::string file=this->_assetDB.uuid + this->_assetDB.extension;
// 	std::filesystem::path fullPath = (std::filesystem::path(this->_mgr->getAssetsRoot()) / file).generic_string();
// 	if (!std::filesystem::exists(fullPath))
// 	{
// 		std::cerr << "AssetLoad:No such file or directory:" << fullPath << std::endl;
// 		this->_loadError();
// 		return;
// 	}
// 	if (!std::filesystem::is_regular_file(fullPath))
// 	{
// 		std::cerr << "AssetLoad:Not a regular file:" << fullPath << std::endl;
// 		this->_loadError();
// 		return;
// 	}
// 	long long time = TimeUtil::nowTime();
// 	std::filesystem::path path = std::filesystem::relative(fullPath, std::filesystem::path(this->_mgr->getAssetsRoot()));
// 	std::string resKey = path.generic_string();

// 	std::string extension = std::filesystem::path(fullPath).extension().string();
// 	if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG")
// 	{
// 		this->_createTexture(resKey, fullPath.generic_string());
// 	}
// 	else if (extension == ".vert" || extension == ".frag")
// 	{
// 		this->_createGlslShader(resKey, fullPath.generic_string());
// 	}
// 	else if (extension == ".spv")
// 	{
// 		this->_createSpirvShader(resKey, fullPath.generic_string());
// 	}
// 	else if (extension == ".scene")
// 	{
// 		this->_createScene(resKey, fullPath.generic_string());
// 	}
// 	else
// 	{
// 		std::cerr << "AssetLoad:Unknown file extension:" << extension << std::endl;
// 		this->_loadError();
// 	}
// 	std::cout << "load asset " << resKey << " cost :" << TimeUtil::nowTime() - time << " ms" << std::endl;
// }
// void AssetTask::_createTexture(const std::string uuid, const std::string fullPath)
// {
// 	/*TextureAsset *texture = new TextureAsset(uuid);
// 	texture->create(fullPath);
// 	this->_cache->addAssetByUuid(uuid, texture);*/

// }
// void AssetTask::_createGlslShader(const std::string resKey, const std::string fullPath)
// {
// 	// Shader *shader = new Shader(resKey, fullPath);
// 	// shader->loadGlsl();
// 	// this->_cache->addAsset(resKey, shader);
// 	// this->_loadComplete();
// }
// void AssetTask::_createSpirvShader(const std::string resKey, const std::string fullPath)
// {
// 	// Shader *shader = new Shader(resKey, fullPath);
// 	// shader->loadSpv();
// 	// this->_cache->addAsset(resKey, shader);
// 	// this->_loadComplete();
// }
// void AssetTask::_createScene(const std::string resKey, const std::string fullPath)
// {
// 	// SceneAsset *scene = new SceneAsset(resKey, fullPath);
// 	// this->_cache->addAsset(resKey, scene);		// 添加到普通资产缓存
// 	// this->_cache->addSceneAsset(resKey, scene); // 添加到场景资产缓存
// 	// this->_loadComplete();
// }

/**
 * @brief 清除资产任务回调
 */
void AssetTask::clearCallback()
{
	this->_callbackOnce = nullptr;
	this->_callbackList = nullptr;
}

void AssetTask::_loadComplete()
{
	// this->_isComplete = true;
	// if (this->_type == AssetTaskType::Sync)
	// {
	// 	return;
	// }
	// else if (this->_type == AssetTaskType::AsyncOnce)
	// {
	// 	if (this->_callbackOnce != nullptr)
	// 	{
	// 		this->_callbackOnce();
	// 	}
	// }
	// else if (this->_type == AssetTaskType::AsyncList)
	// {
	// 	if (this->_result == nullptr)
	// 	{
	// 		return;
	// 	}
	// 	this->_result->complete++;
	// 	int complete = this->_result->complete;
	// 	int all = this->_result->all;
	// 	float progress = (float)complete / (float)all;
	// 	if (this->_callbackList != nullptr)
	// 	{
	// 		this->_callbackList(complete, all, progress);
	// 	}
	// 	if (complete >= all)
	// 	{
	// 		delete this->_result;
	// 		this->_result = nullptr;
	// 	}
	// }
}
void AssetTask::_loadError()
{
	// this->_isComplete = true;
	// if (this->_type == AssetTaskType::Sync)
	// {
	// 	return;
	// }
	// else if (this->_type == AssetTaskType::AsyncOnce)
	// {
	// 	if (this->_callbackOnce != nullptr)
	// 	{
	// 		this->_callbackOnce();
	// 	}
	// }
	// else if (this->_type == AssetTaskType::AsyncList)
	// {
	// 	if (this->_result == nullptr)
	// 	{
	// 		return;
	// 	}
	// 	this->_result->complete++;
	// 	int complete = this->_result->complete;
	// 	int all = this->_result->all;
	// 	float progress = (float)complete / (float)all;
	// 	if (this->_callbackList != nullptr)
	// 	{
	// 		this->_callbackList(complete, all, progress);
	// 	}
	// 	if (complete >= all)
	// 	{
	// 		delete this->_result;
	// 		this->_result = nullptr;
	// 	}
	// }
}

AssetTask::~AssetTask()
{
}
