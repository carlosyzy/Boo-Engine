#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
class Shader : public Asset
{
private:
	std::string _data = "";
protected:
	void _load() override;
public:
	Shader(const std::string key, const std::string path);
	/**
	 * @brief 清除缓存
	 * 
	 */	
	void clearCache();
    virtual void destroy() override;
	~Shader() {}
};