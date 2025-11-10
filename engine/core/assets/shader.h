#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
class Shader : public Asset
{
private:
	std::string _shaderData = "";
	std::string _shaderType = "";

protected:
	void _load() override;

public:
	Shader(const std::string uuid, const std::string path);
	/**
	 * @brief 创建图形资源
	 * 参数 宏定义
	 */
	void createGfxShader(const std::map<std::string, std::string> &macros);
	virtual void destroy() override;
	~Shader() {}
};