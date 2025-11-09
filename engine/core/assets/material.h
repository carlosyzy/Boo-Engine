#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
class Material : public Asset
{
private:
	/**
     * @brief 顶点着色器路径
     */
	std::string _vert;
     /**
     * @brief 片段着色器路径
     */
	std::string _frag;

protected:
	void _load() override;

public:
	Material(const std::string key);
	Material(const std::string key, const std::string path);
	/**
	 * @brief 创建图形资源
	 * 参数 宏定义
	 */
	virtual void destroy() override;
	~Material() {}
};