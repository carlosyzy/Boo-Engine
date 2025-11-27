#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
class MaterialAsset : public Asset
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
	/**
     * @brief 渲染通道
     */
	std::string _pass;
	/**
     * @brief 渲染管线
     */
	std::string _pipeline;
protected:
	void _load() override;

public:
	// MaterialAsset(const std::string key);
	MaterialAsset(const std::string uuid, const std::string path);

	const std::string &getPass() { return this->_pass; };
	/**
     * @brief 获取渲染管线
     *
     * @return const std::string&
     */
	const std::string &getPipeline() { return this->_pipeline; };
	/**
	 * @brief 创建图形资源
	 * 参数 宏定义
	 */
	virtual void destroy() override;
	~MaterialAsset() {}
};