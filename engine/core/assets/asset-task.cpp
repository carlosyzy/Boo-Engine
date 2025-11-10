
#include "asset-task.h"
#include <filesystem>
#include <iostream>
#include "texture.h"
#include "shader.h"
#include "assets-manager.h"
#include "asset-cache.h"

AssetTask::AssetTask(AssetsManager *mgr, AssetCache *cache)
{
	this->_mgr = mgr;
	this->_cache = cache;
}
Asset *AssetTask::load(const std::string &path)
{
	std::cout << "AssetTask:load:" << path << std::endl;
	this->_path = path;
	return this->run();
}
void AssetTask::load(const std::string path, AssetLoadResult *result)
{
	this->_path = path;
}

Asset *AssetTask::run()
{
	std::filesystem::path fullPath = std::filesystem::path(this->_mgr->root()) / this->_path;
	std::cout << "AssetTask:Path:" << fullPath << std::endl;
	if (!std::filesystem::exists(fullPath))
	{
		std::cerr << "AssetLoad:No such file or directory:" << fullPath << std::endl;
		return nullptr;
	}
	if (!std::filesystem::is_regular_file(fullPath))
	{
		std::cerr << "AssetLoad:Not a regular file:" << fullPath << std::endl;
		return nullptr;
	}
	std::filesystem::path key = std::filesystem::relative(fullPath, std::filesystem::path(this->_mgr->root()));
	std::cout << "AssetTask:Key:" << key.generic_string() << std::endl;
	std::string extension = std::filesystem::path(fullPath).extension().string();
	if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG")
	{
		Texture *texture = new Texture(key.generic_string(), fullPath.generic_string());
		this->_cache->addAsset(key.generic_string(), texture);
		return texture;
	}
	else if (extension == ".vert" || extension == ".frag")
	{
		Shader *shader = new Shader(key.generic_string(), fullPath.generic_string());
		this->_cache->addAsset(key.generic_string(), shader);
		return shader;
	}
	return nullptr;
}

AssetTask::~AssetTask()
{
}