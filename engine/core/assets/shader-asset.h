#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
class ShaderAsset : public Asset
{
private:
	std::string _glslData = "";
	std::vector<uint32_t> _spirvData ;

public:
	ShaderAsset();
	ShaderAsset(const std::string uuid);
	void create(std::string path) override;
	void create(const uint32_t *data);
	void destroy() override;
	~ShaderAsset() {}
};

// /**
//  * @brief 加载GLSL文件
//  */
// void loadGlsl();
// /**
//  * @brief 加载SPIR-V文件
//  */
// void loadSpv();
// /**
//  * @brief 创建图形资源
//  * 参数 宏定义
//  */
// void createGfxShader(const std::map<std::string, std::string> &macros);
// /**
//  * @brief 创建SPIR-V图形资源
//  */
// void createGfxSpirvShader();