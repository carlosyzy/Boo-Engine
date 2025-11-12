#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <iostream>
#include <filesystem>
#include "asset.h"
#include "asset-load.h"
#include "asset-cache.h"
#include "texture.h"
#include "material.h"
#include "shader.h"



// 基础图片资源
class AssetsManager
{
private:
	/**
	 * @brief 资产根目录
	 */
	std::string _root;

	/**
	 * @brief 资产加载器
	 */
	AssetLoad *_assetLoad;

	void _initRoot();

public:
	AssetsManager();
	/**
	 * @brief 初始化资产管理器
	 */
	void init();

	const std::string &root()
	{
		return this->_root;
	}
	AssetLoad *assetLoad()
	{
		return this->_assetLoad;
	}
	/**
	 * @brief 加载资产
	 * @param path 资产路径
	 * @return Asset 资产对象
	 */
	Asset *load(const std::string &path);
	template <typename T, typename Func>
	int loadAsync(const std::string &path, Func callback, T *instance)
	{
		return this->_assetLoad->loadAsync(path, callback, instance);
	}
	template <typename T, typename Func>
	int loadListAsync(const std::vector<std::string> &paths, Func callback, T *instance)
	{
		return this->_assetLoad->loadListAsync(paths, callback, instance);
	}
	void setMaxLoadCount(int count);
	void clearLoadCall(const int loadId);

	Asset *get(const std::string &path);

	void update(float deltaTime);

	~AssetsManager();
};