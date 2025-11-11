#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <vector>

class Texture : public Asset
{
private:
	int _width = 0;
    int _height = 0;
    int _channels = 0;
	std::vector<uint8_t> _pixelsVector;
protected:
	void _load() override;
public:
	Texture(const std::string key, const std::string path);


	/**
	 * @brief 获取纹理宽度
	 * 
	 * @return int 
	 */
	const int width() const { return _width; }
	/**
	 * @brief 获取纹理高度
	 * 
	 * @return int 
	 */
	const int height() const { return _height; }
	/**
	 * @brief 获取纹理通道数
	 * 
	 * @return int 
	 */
	int channels() const { return _channels; }
	/**
	 * @brief 创建图形资源
	 * 
	 */
	void createGfxTexture();
	/**
	 * @brief 清除缓存
	 * 
	 */	
	void clearCache();
    virtual void destroy() override;
	~Texture() {}
};