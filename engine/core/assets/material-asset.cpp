#include "material-asset.h"
#include "../gfx/gfx-mgr.h"
#include "../utils/uuid-util.h"
#include "../gfx/base/gfx-pipeline-struct.h"
#include "../gfx/base/gfx-push-constants.h"
#include "../gfx/base/gfx-mesh.h"
#include "../gfx/base/gfx-material.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

MaterialAsset::MaterialAsset() : Asset()
{
    this->_type = AssetType::Material;
    this->_gfxMaterial = new GfxMaterial();
}
MaterialAsset::MaterialAsset(const std::string uuid) : Asset(uuid)
{
    this->_type = AssetType::Material;
    this->_gfxMaterial = new GfxMaterial();
}
void MaterialAsset::create(std::string path)
{
    Asset::create(path);
    json materialJson = FileUtil::readJsonFromText(this->_path);
    // 初始化材质属性
    this->_initProperties();
    // 初始化管线结构体
    this->_initPipelineStruct();
}
void MaterialAsset::create(json &materialData)
{

    // 初始化材质属性
    this->_initProperties();
    // 初始化管线结构体
    this->_initPipelineStruct();
}



void MaterialAsset::_initProperties()
{
  
    // this->_render = materialJson["render"].get<int>();
    // if (materialJson.contains("vert"))
    // {
    //     this->_vert = materialJson["vert"].get<std::string>();
    //     if (this->_vert.empty())
    //     {
    //         // this->_vert = this->_render == 0 ? "builtin-ui.vert" : "builtin-vert.vert";
    //         this->_vert = "builtin-ui.vert";
    //     }
    // }
    // if (materialJson.contains("frag"))
    // {
    //     this->_frag = materialJson["frag"].get<std::string>();
    //     if (this->_frag.empty())
    //     {
    //         this->_frag = this->_render == 0 ? "builtin-ui.frag" : "builtin-frag.frag";
    //     }
    // }
    // // 多边形模式
    // if (materialJson.contains("polygonMode"))
    // {
    //     this->polygonMode = materialJson["polygonMode"].get<int>();
    // }
    // else
    // {
    //     this->polygonMode = static_cast<int>(GfxPipelinePolygonMode::Fill);
    // }
    // // 剔除模式
    // if (materialJson.contains("cullMode"))
    // {
    //     this->cullMode = materialJson["cullMode"].get<int>();
    // }
    // else
    // {
    //     this->cullMode = 0;
    // }
    // // 深度测试
    // if (materialJson.contains("depthTest"))
    // {
    //     this->depthTest = materialJson["depthTest"].get<int>();
    // }
    // else
    // {
    //     this->depthTest = 0;
    // }
    // // 深度写入
    // if (materialJson.contains("depthWrite"))
    // {
    //     this->depthWrite = materialJson["depthWrite"].get<int>();
    // }
    // else
    // {
    //     this->depthWrite = 0;
    // }
    // // 深度比较操作
    // if (materialJson.contains("depthCompareOp"))
    // {
    //     this->depthCompareOp = materialJson["depthCompareOp"].get<int>();
    // }
    // else
    // {
    //     this->depthCompareOp = static_cast<int>(GfxPipelineCompareOp::Always);
    // }

    // if (materialJson.contains("stencilTest"))
    // {
    //     this->stencilTest = materialJson["stencilTest"].get<int>();
    // }
    // else
    // {
    //     this->stencilTest = 0;
    // }
    // // 模版测试 比较操作
    // if (materialJson.contains("stencilFrontCompareOp"))
    // {
    //     this->stencilFrontCompareOp = materialJson["stencilFrontCompareOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilFrontCompareOp = static_cast<int>(GfxPipelineCompareOp::Equal);
    // }
    // // 模版测试 失败操作
    // if (materialJson.contains("stencilFrontFailOp"))
    // {
    //     this->stencilFrontFailOp = materialJson["stencilFrontFailOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilFrontFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    // }
    // // 模版测试 深度失败操作
    // if (materialJson.contains("stencilFrontDepthFailOp"))
    // {
    //     this->stencilFrontDepthFailOp = materialJson["stencilFrontDepthFailOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilFrontDepthFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    // }
    // // 模版测试 通过操作
    // if (materialJson.contains("stencilFrontPassOp"))
    // {
    //     this->stencilFrontPassOp = materialJson["stencilFrontPassOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilFrontPassOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    // }
    // // 模版测试 比较掩码
    // if (materialJson.contains("stencilBackCompareOp"))
    // {
    //     this->stencilBackCompareOp = materialJson["stencilBackCompareOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilBackCompareOp = static_cast<int>(GfxPipelineCompareOp::Equal);
    // }
    // // 模版测试 写入掩码
    // if (materialJson.contains("stencilBackFailOp"))
    // {
    //     this->stencilBackFailOp = materialJson["stencilBackFailOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilBackFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    // }
    // // 模版测试 深度失败操作
    // if (materialJson.contains("stencilBackDepthFailOp"))
    // {
    //     this->stencilBackDepthFailOp = materialJson["stencilBackDepthFailOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilBackDepthFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    // }
    // // 模版测试 通过操作
    // if (materialJson.contains("stencilBackPassOp"))
    // {
    //     this->stencilBackPassOp = materialJson["stencilBackPassOp"].get<int>();
    // }
    // else
    // {
    //     this->stencilBackPassOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    // }
}
void MaterialAsset::_initPipelineStruct()
{
    GfxPipelineStruct uiPipeline = {};
    uiPipeline.render = this->_render;
    uiPipeline.vert = this->_vert;
    uiPipeline.frag = this->_frag;
    // 多边形模式 填充
    uiPipeline.polygonMode = static_cast<GfxPipelinePolygonMode>(this->polygonMode);
    // 剔除模式 背面
    uiPipeline.cullMode = static_cast<GfxPipelineCullMode>(this->cullMode);
    // 深度测试 开启
    uiPipeline.depthTest = this->depthTest;
    // 深度写入 开启
    uiPipeline.depthWrite = this->depthWrite;
    // 深度比较操作 小于等于
    uiPipeline.depthCompareOp = static_cast<GfxPipelineCompareOp>(this->depthCompareOp);

    // 模版测试 启用（用于UI遮罩）
    uiPipeline.stencilTest = this->stencilTest;
    uiPipeline.stencilFrontCompareOp = static_cast<GfxPipelineCompareOp>(this->stencilFrontCompareOp);     // 只在模板值相等时绘制
    uiPipeline.stencilFrontFailOp = static_cast<GfxPipelineStencilOp>(this->stencilFrontFailOp);           // 测试失败：保持
    uiPipeline.stencilFrontDepthFailOp = static_cast<GfxPipelineStencilOp>(this->stencilFrontDepthFailOp); // 深度失败：保持
    uiPipeline.stencilFrontPassOp = static_cast<GfxPipelineStencilOp>(this->stencilFrontPassOp);           // 测试通过：保持（不修改模板值）
    uiPipeline.stencilBackCompareOp = static_cast<GfxPipelineCompareOp>(this->stencilBackCompareOp);
    uiPipeline.stencilBackFailOp = static_cast<GfxPipelineStencilOp>(this->stencilBackFailOp);
    uiPipeline.stencilBackDepthFailOp = static_cast<GfxPipelineStencilOp>(this->stencilBackDepthFailOp);
    uiPipeline.stencilBackPassOp = static_cast<GfxPipelineStencilOp>(this->stencilBackPassOp);
    // 颜色混合 开启
    uiPipeline.colorBlend = this->colorBlend;
    uiPipeline.srcColorBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->srcColorBlendFactor);
    uiPipeline.dstColorBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->dstColorBlendFactor);
    uiPipeline.colorBlendOp = static_cast<GfxPipelineColorBlendOp>(this->colorBlendOp);
    uiPipeline.srcAlphaBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->srcAlphaBlendFactor);
    uiPipeline.dstAlphaBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->dstAlphaBlendFactor);
    uiPipeline.alphaBlendOp = static_cast<GfxPipelineColorBlendOp>(this->alphaBlendOp);
    // 推送常量 开启
    uiPipeline.pushConstant = 1;
    uiPipeline.pushConstantSize = 0;

    this->_gfxMaterial->setPipelineStruct(uiPipeline);
    // std::vector<std::string> aaas;
    // aaas.push_back("_private/default.png");
    // this->_gfxMaterial->setTextures(aaas);
}

// void MaterialAsset::createUITest()
// {
//     GfxPipelineStruct uiPipeline = {};
//     uiPipeline.render = 1;
//     uiPipeline.vert = "builtin-ui.vert";
//     uiPipeline.frag = "builtin-ui.frag";
//     // 多边形模式 填充
//     uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
//     // 剔除模式 背面
//     uiPipeline.cullMode = GfxPipelineCullMode::None;
//     // 深度测试 开启
//     uiPipeline.depthTest = 0;
//     // 深度写入 开启
//     uiPipeline.depthWrite = 0;
//     // 深度比较操作 小于等于
//     uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

//     // 模版测试 启用（用于UI遮罩）
//     uiPipeline.stencilTest = 1;
//     uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
//     uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
//     uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
//     uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
//     uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
//     uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Keep;
//     // 颜色混合 开启
//     uiPipeline.colorBlend = 1;
//     uiPipeline.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
//     uiPipeline.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.colorBlendOp = GfxPipelineColorBlendOp::Add;
//     uiPipeline.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
//     uiPipeline.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.alphaBlendOp = GfxPipelineColorBlendOp::Add;
//     // 推送常量 开启
//     uiPipeline.pushConstant = 1;
//     uiPipeline.pushConstantSize = 0;

//     this->_gfxMaterial = new GfxMaterial();
//     this->_gfxMaterial->setPipelineStruct(uiPipeline);
//     std::vector<std::string> aaas;
//     aaas.push_back("_private/default.png");
//     this->_gfxMaterial->setTextures(aaas);
// }
// void MaterialAsset::createUIMaskTest(int maskType)
// {
//     GfxPipelineStruct uiPipeline = {};
//     uiPipeline.render = 1;
//     uiPipeline.vert = "builtin-ui-mask.vert";
//     uiPipeline.frag = "builtin-ui-mask.frag";
//     // 多边形模式 填充
//     uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
//     // 剔除模式 背面
//     uiPipeline.cullMode = GfxPipelineCullMode::None;
//     // 深度测试 开启
//     uiPipeline.depthTest = 0;
//     // 深度写入 开启
//     uiPipeline.depthWrite = 0;
//     // 深度比较操作 小于等于
//     uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

//     // 模版测试 启用（用于UI遮罩）
//     uiPipeline.stencilTest = 1;
//     uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Always; // 只在模板值相等时绘制
//     uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
//     uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度：保持
//     if (maskType == 0)
//     {
//         uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add; // 测试通过：保持（不修改模板值）
//     }
//     else
//     {
//         uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Decrement_Subtract;
//     }
//     uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Always;
//     uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     if (maskType == 0)
//     {
//         uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
//     }
//     else
//     {
//         uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Decrement_Subtract;
//     }
//     // 颜色混合 开启
//     uiPipeline.colorBlend = 0;
//     // 推送常量 开启
//     uiPipeline.pushConstant = 1;
//     uiPipeline.pushConstantSize = 0;

//     this->_gfxMaterial = new GfxMaterial();
//     this->_gfxMaterial->setPipelineStruct(uiPipeline);
//     std::vector<std::string> aaas;
//     aaas.push_back("_private/default.png");
//     this->_gfxMaterial->setTextures(aaas);
// }
// void MaterialAsset::createMaskUITest()
// {
//     GfxPipelineStruct uiPipeline = {};
//     uiPipeline.render = 1;
//     uiPipeline.vert = "builtin-ui-mask-test.vert";
//     uiPipeline.frag = "builtin-ui-mask-test.frag";
//     // 多边形模式 填充
//     uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
//     // 剔除模式 背面
//     uiPipeline.cullMode = GfxPipelineCullMode::None;
//     // 深度测试 开启
//     uiPipeline.depthTest = 0;
//     // 深度写入 开启
//     uiPipeline.depthWrite = 0;
//     // 深度比较操作 小于等于
//     uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

//     // 模版测试 启用（用于UI遮罩）
//     uiPipeline.stencilTest = 1;
//     uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
//     uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
//     uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
//     uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
//     uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
//     uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Keep;
//     // 颜色混合 开启
//     uiPipeline.colorBlend = 1;
//     uiPipeline.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
//     uiPipeline.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.colorBlendOp = GfxPipelineColorBlendOp::Add;
//     uiPipeline.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
//     uiPipeline.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.alphaBlendOp = GfxPipelineColorBlendOp::Add;
//     // 推送常量 开启
//     uiPipeline.pushConstant = 1;
//     uiPipeline.pushConstantSize = 0;

//     this->_gfxMaterial = new GfxMaterial();
//     this->_gfxMaterial->setPipelineStruct(uiPipeline);
//     std::vector<std::string> aaas;
//     aaas.push_back("_private/default.png");
//     this->_gfxMaterial->setTextures(aaas);
// }

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
