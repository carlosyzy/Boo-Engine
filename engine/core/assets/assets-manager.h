#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
enum class AssetType;
class Asset;
class AssetCache;
class AssetLoad;

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

	const  std::string& root() {
		return this->_root;
	}
	AssetLoad *assetLoad() {
		return this->_assetLoad;
	}
	/**
	 * @brief 加载资产
	 * @param path 资产路径
	 * @return Asset 资产对象
	 */
	Asset* load(const std::string &path);
	template <typename T, typename Func>
	void loadAsync(const std::string &path, Func func, T *instance);
	/**
	 * @brief 异步加载资产列表
	 * @param paths 资产路径列表
	 */
	void loadList(const std::vector<std::string> &paths);
	template <typename T, typename Func>
	void loadListAsync(const std::vector<std::string> &paths,Func func, T *instance);

	Asset *get(const std::string &path);
	// /**
	//  * @brief 异步加载资产
	//  * @param path 资产路径
	//  */
	// 
	~AssetsManager();
};