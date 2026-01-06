#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "../utils/json-util.h"

class GfxMaterial;

class MaterialAsset : public Asset
{
private:
	// UI和3D材质
	std::string _render = "UI";
	/**
	 * @brief 顶点着色器id
	 */
	std::string _vert;
	/**
	 * @brief 片段着色器id
	 */
	std::string _frag;

	/**
	 * @brief 管线状态
	 */
	json _pipeline = json::object();
	/**
	 * @brief 存储纹理
	 * 键值对 纹理名称-纹理id
	 * {
	 *  "index": 0,
	 *  "key": "texture0"
	 *  "value": "texture0"
	 * }
	 */
	json _textures = json::array();
	/**
	 * @brief 推送常量
	 * {
	 *  "index": 0,
	 *  "key": "pushConstant0"
	 *  "value": "pushConstant0"
	 * }
	 */
	json _pushConstants = json::array();


	GfxMaterial *_gfxMaterial;

protected:
public:
	MaterialAsset();
	MaterialAsset(const std::string uuid);

	void create(std::string path) override;
	void createUITest();
	void createUIMaskTest(int maskType);
	void createMaskUITest();
		
	void setTextures(const std::vector<std::string> textures);
	void setTexture(int index, std::string texture);


	GfxMaterial *getGfxMaterial() { return _gfxMaterial; }
	/**
	 * @brief 创建图形资源
	 * 参数 宏定义
	 */
	virtual void destroy() override;
	~MaterialAsset() {}
};