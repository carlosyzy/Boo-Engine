#include "material-asset.h"
#include "../gfx/gfx-mgr.h"
#include "../utils/uuid-util.h"
#include "../gfx/base/gfx-pipeline-struct.h"
#include "../gfx/base/gfx-push-constants.h"
#include "../gfx/base/gfx-mesh.h"
#include "../gfx/base/gfx-material.h"


MaterialAsset::MaterialAsset() : Asset()
{
	this->_type = AssetType::Material;
	
}
MaterialAsset::MaterialAsset(const std::string uuid) : Asset(uuid)
{
	this->_type = AssetType::Material;
}
void MaterialAsset::create(std::string path) 
{
    Asset::create(path);
	//读取材质文件，解析材质信息
}
void MaterialAsset::createUITest()
{
	GfxPipelineStruct uiPipeline = {};
    uiPipeline.render = 1;
    uiPipeline.vert = "builtin-ui.vert";
    uiPipeline.frag = "builtin-ui.frag";
    // 多边形模式 填充
    uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    uiPipeline.cullMode = GfxPipelineCullMode::None;
    // 深度测试 开启
    uiPipeline.depthTest = 0;
    // 深度写入 开启
    uiPipeline.depthWrite = 0;
    // 深度比较操作 小于等于
    uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

    // 模版测试 启用（用于UI遮罩）
    uiPipeline.stencilTest = 1;
    uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
    uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
    uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
    uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
    uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
    uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Keep;
    // 颜色混合 开启
    uiPipeline.colorBlend = 1;
    uiPipeline.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
    uiPipeline.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    uiPipeline.colorBlendOp = GfxPipelineColorBlendOp::Add;
    uiPipeline.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
    uiPipeline.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    uiPipeline.alphaBlendOp = GfxPipelineColorBlendOp::Add;
    // 推送常量 开启
    uiPipeline.pushConstant = 1;
    uiPipeline.pushConstantSize = 0;

	this->_gfxMaterial = new GfxMaterial();
    this->_gfxMaterial->setPipelineStruct(uiPipeline);
    std::vector<std::string> aaas;
    aaas.push_back("550e8400-e29b-41d4-a716-446655440000");
    this->_gfxMaterial->setTextures(aaas);
}
void MaterialAsset::createUIMaskTest(int maskType){
    GfxPipelineStruct uiPipeline = {};
    uiPipeline.render = 1;
    uiPipeline.vert = "builtin-ui-mask.vert";
    uiPipeline.frag = "builtin-ui-mask.frag";
    // 多边形模式 填充
    uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    uiPipeline.cullMode = GfxPipelineCullMode::None;
    // 深度测试 开启
    uiPipeline.depthTest = 0;
    // 深度写入 开启
    uiPipeline.depthWrite = 0;
    // 深度比较操作 小于等于
    uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

    // 模版测试 启用（用于UI遮罩）
    uiPipeline.stencilTest = 1;
    uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Always;  // 只在模板值相等时绘制
    uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
    uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度：保持
    if(maskType == 0){
        uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add;      // 测试通过：保持（不修改模板值）
    }else{
        uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Decrement_Subtract;
    }
    uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Always;
    uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    if(maskType == 0){
        uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
    }else{
        uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Decrement_Subtract;
    }
    // 颜色混合 开启
    uiPipeline.colorBlend = 0;
    // 推送常量 开启
    uiPipeline.pushConstant = 1;
    uiPipeline.pushConstantSize = 0;

	this->_gfxMaterial = new GfxMaterial();
    this->_gfxMaterial->setPipelineStruct(uiPipeline);
    std::vector<std::string> aaas;
    aaas.push_back("550e8400-e29b-41d4-a716-446655440000");
    this->_gfxMaterial->setTextures(aaas);
}

void MaterialAsset::setTextures(const std::vector<std::string> textures)
{
    this->_gfxMaterial->setTextures(textures);
}
void MaterialAsset::setTexture(int index, std::string texture)
{
   this->_gfxMaterial->setTextures({texture});
}



void MaterialAsset::destroy()
{
}
