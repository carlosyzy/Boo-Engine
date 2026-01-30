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
	GfxMaterial *_gfxMaterial;

private:
	// UI和3D材质
	int _render = 0;
	/**
	 * @brief 顶点着色器id
	 */
	std::string _vert;
	/**
	 * @brief 片段着色器id
	 */
	std::string _frag;
	/**
     * 多边形模式
     */
    int polygonMode = 0;
    /**
     * 剔除模式
     */
    int cullMode = 0;
    /**
     * 深度测试
     * 开关
     */
    int depthTest = 0;
    /**
     * 深度写入
     * 开关
     */
    int depthWrite = 0;
    /**
     * 深度比较操作
     */
    int depthCompareOp = 0;

    /**
     * 模板测试
     * 开关
     */
    int stencilTest = 0;
    /**
     * 正面三角形（逆时针）的Stencil操作
     */
    int stencilFrontCompareOp = 0;
    int stencilFrontFailOp = 0;
    int stencilFrontDepthFailOp = 0;
    int stencilFrontPassOp = 0;
    uint32_t stencilFrontCompareMask = 0xFFFFFFFF;
    uint32_t stencilFrontWriteMask = 0xFFFFFFFF;
    uint32_t stencilFrontRreference = 0;
    /**
     * 反面三角形（顺时针）的Stencil操作
     */
    int stencilBackCompareOp = 0;
    int stencilBackFailOp = 0;
    int stencilBackDepthFailOp = 0;
    int stencilBackPassOp = 0;
    uint32_t stencilBackCompareMask = 0xFFFFFFFF;
    uint32_t stencilBackWriteMask = 0xFFFFFFFF;
    uint32_t stencilBackRreference = 0;

    /**
     * 颜色缓和
     * 开关
     */
    int colorBlend = 0;
    /**
     * 混合模式
     */
    /**
     * 源颜色混合因子
     */
    int srcColorBlendFactor = 0;
    /**
     * 目标颜色混合因子
     */
    int dstColorBlendFactor = 0;
    /**
     * 颜色混合操作
     */
    int colorBlendOp = 0;
    /**
     * 源alpha混合因子
     */
    int srcAlphaBlendFactor = 0;
    /**
     * 目标alpha混合因子
     */
    int dstAlphaBlendFactor = 0;
    /**
     * alpha混合操作
     */
    int alphaBlendOp = 0;



	// /**
	//  * @brief 管线状态
	//  */
	// json _pipeline = json::object();
	// /**
	//  * @brief 存储纹理
	//  * 键值对 纹理名称-纹理id
	//  * {
	//  *  "index": 0,
	//  *  "key": "texture0"
	//  *  "value": "texture0"
	//  * }
	//  */
	// json _textures = json::array();
	// /**
	//  * @brief 推送常量
	//  * {
	//  *  "index": 0,
	//  *  "key": "pushConstant0"
	//  *  "value": "pushConstant0"
	//  * }
	//  */
	// json _pushConstants = json::array();

	void _initProperties();
	void _initPipelineStruct();

protected:
public:
	MaterialAsset();
	MaterialAsset(const std::string uuid);

	void create(std::string path) override;
    void create(json &materialData);
	// void createUITest();
	// void createUIMaskTest(int maskType);
	// void createMaskUITest();
		
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