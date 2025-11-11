#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
enum class ShaderFormat
{
	None,
	GLSL,
	SPIRV,
};
class Shader : public Asset
{
private:
	ShaderFormat _shaderFormat;
	std::string _glslData = "";
	std::string _glslType = "";
	std::vector<char> _spirvData ;

protected:
	void _load() override;

public:
	Shader(const std::string uuid, const std::string path);
	/**
	 * @brief 加载GLSL文件
	 */
	void loadGlsl();
	/**
	 * @brief 加载SPIR-V文件
	 */
	void loadSpv();
	/**
	 * @brief 创建图形资源
	 * 参数 宏定义
	 */
	void createGfxShader(const std::map<std::string, std::string> &macros);
	// /**
	//  * @brief 创建SPIR-V图形资源
	//  */
	// void createGfxSpirvShader();
	virtual void destroy() override;
	~Shader() {}
};