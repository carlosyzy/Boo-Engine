
#include "asset-task.h"
#include <filesystem>
#include <iostream>
#include "texture.h"
#include "shader.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "../utils/time-util.h"

AssetTask::AssetTask(AssetsManager *mgr, AssetCache *cache, int id)
{
	this->_mgr = mgr;
	this->_cache = cache;
	this->_isComplete = false;
	this->_id = id;
}
Asset *AssetTask::load(const std::string &path)
{
	this->_path = path;
	this->_type = AssetTaskType::Sync;
	this->run();
	return this->_cache->getAsset(this->_path);
}


void AssetTask::run()
{
	std::filesystem::path fullPath = std::filesystem::path(this->_mgr->root()) / this->_path;
	// std::cout << "load asset " << this->_path << " fullPath:" << fullPath << std::endl;
	if (!std::filesystem::exists(fullPath))
	{
		std::cerr << "AssetLoad:No such file or directory:" << fullPath << std::endl;
		this->_loadError();
		return;
	}
	if (!std::filesystem::is_regular_file(fullPath))
	{
		std::cerr << "AssetLoad:Not a regular file:" << fullPath << std::endl;
		this->_loadError();
		return;
	}
	std::filesystem::path path = std::filesystem::relative(fullPath, std::filesystem::path(this->_mgr->root()));
	std::string resKey = path.generic_string();

	std::string extension = std::filesystem::path(fullPath).extension().string();
	if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG")
	{
		this->_createTexture(resKey, fullPath.generic_string());
	}
	else if (extension == ".vert" || extension == ".frag")
	{
		this->_createGlslShader(resKey, fullPath.generic_string());
	}
	else if (extension == ".spv")
	{
		this->_createSpirvShader(resKey, fullPath.generic_string());
	}
	else
	{
		std::cerr << "AssetLoad:Unknown file extension:" << extension << std::endl;
		this->_loadError();
	}
}
void AssetTask::_createTexture(const std::string resKey, const std::string fullPath)
{
	Texture *texture = new Texture(resKey, fullPath);
	this->_cache->addAsset(resKey, texture);
	this->_loadComplete();
}
void AssetTask::_createGlslShader(const std::string resKey, const std::string fullPath)
{
	Shader *shader = new Shader(resKey, fullPath);
	shader->loadGlsl();
	this->_cache->addAsset(resKey, shader);
	this->_loadComplete();
}
void AssetTask::_createSpirvShader(const std::string resKey, const std::string fullPath)
{
	Shader *shader = new Shader(resKey, fullPath);
	shader->loadSpv();
	this->_cache->addAsset(resKey, shader);
	this->_loadComplete();
}
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
	this->_isComplete = true;
	if (this->_type == AssetTaskType::Sync)
	{
		return;
	}
	else if (this->_type == AssetTaskType::AsyncOnce)
	{
		if (this->_callbackOnce != nullptr)
		{
			this->_callbackOnce();
		}
	}
	else if (this->_type == AssetTaskType::AsyncList)
	{
		if (this->_result == nullptr)
		{
			return;
		}
		this->_result->complete++;
		int complete = this->_result->complete;
		int all = this->_result->all;
		float progress = (float)complete / (float)all;
		if (this->_callbackList != nullptr)
		{
			this->_callbackList(complete, all, progress);
		}
		if (complete >= all)
		{
			delete this->_result;
			this->_result = nullptr;
		}
	}
}
void AssetTask::_loadError()
{
	this->_isComplete = true;
	if (this->_type == AssetTaskType::Sync)
	{
		return;
	}
	else if (this->_type == AssetTaskType::AsyncOnce)
	{
		if (this->_callbackOnce != nullptr)
		{
			this->_callbackOnce();
		}
	}
	else if (this->_type == AssetTaskType::AsyncList)
	{
		if (this->_result == nullptr)
		{
			return;
		}
		this->_result->complete++;
		int complete = this->_result->complete;
		int all = this->_result->all;
		float progress = (float)complete / (float)all;
		if (this->_callbackList != nullptr)
		{
			this->_callbackList(complete, all, progress);
		}
		if (complete >= all)
		{
			delete this->_result;
			this->_result = nullptr;
		}
	}
}

AssetTask::~AssetTask()
{
}
