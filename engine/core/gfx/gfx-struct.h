#pragma once
#define VK_NO_PROTOTYPES      // 禁止官方头文件定义函数原型
#include <volk/volk.h>  // 替代 vulkan.h
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <shaderc/shaderc.hpp>
#include "../../log.h"
/**
 * @brief 渲染器分类
 */
enum class GfxRendererLayer
{
    _UI = 0,
    _3D = 1,
};
/**
 * @brief 渲染器类型
 */
enum class GfxRendererType
{
    _Opaque = 0,
    _Transparent = 1,
};

enum class GfxTextureFormat
{
    R8G8B8A8_UNORM = 0,
    R8G8B8A8_SRGB = 1,
    R8_UNORM = 2,
};

enum class GfxRendererStatePolygonMode
{
    /**
     * 填充多边形
     */
    Fill = 0,
    /**
     * 线框多边形
     */
    Line = 1,

};
enum class GfxRendererStateCullMode
{
    /**
     * 不剔除任何三角形
     */
    None = 0,
    /**
     * 剔除正面三角形（逆时针）
     */
    Front = 1,
    /**
     * 剔除反面三角形（顺时针）
     */
    Back = 2,
};

/**
 * 深度比较操作
 */
enum class GfxRendererStateCompareOp
{
    /**
     * 从不通过深度测试
     */
    Never = 0,
    /**
     * 仅当新像素的深度值小于当前深度值时通过
     * 特殊效果，如深度相等高亮
     */
    Less = 1,
    /**
     * 仅当新像素的深度值小于等于当前深度值时通过
     * 阴影映射、某些后处理效果
     */
    LessOrEqual = 2,
    /**
     * 仅当新像素的深度值大于当前深度值时通过
     * 反转深度测试、特殊效果
     */
    Greater = 3,
    /**
     * 仅当新像素的深度值大于等于当前深度值时通过
     * 通过  反射、门户效果
     */
    GreaterOrEqual = 4,
    /**
     * 仅当新像素的深度值等于当前深度值时通过
     * 特殊效果，如深度相等高亮
     */
    Equal = 5,
    /**
     * 仅当新像素的深度值不等于当前深度值时通过
     * 轮廓效果、剔除特定深度
     */
    NotEqual = 6,
    /**
     * 总是通过深度测试
     */
    Always = 7,
};
enum class GfxRendererStateStencilOp
{
    /**
     * 保持原值
     */
    Keep = 0,
    /**
     * 测试通过：值+1
     */
    Increment_Add = 3,
    /**
     * 测试通过：值-1
     */
    Decrement_Subtract = 4,
    /**
     * 测试通过：替换为参考值
     */
    Replace = 5,

};
enum class GfxRendererStateColorBlendFactor
{
    /**
     * 加法混合
     */
    One = 0,
    /**
     * 减法混合
     */
    Zero = 1,
    /**
     * 源颜色
     */
    SrcColor = 2,
    /**
     * 目标颜色
     */
    DstColor = 3,
    /**
     * 源alpha
     */
    SrcAlpha = 4,
    /**
     * 目标alpha
     */
    DstAlpha = 5,
    /**
     * 源1-minus alpha
     */
    OneMinusSrcAlpha = 6,
    /**
     * 目标1-minus alpha
     */
    OneMinusDstAlpha = 7,
};
enum class GfxRendererStateColorBlendOp
{
    /**
     * 加法混合
     */
    Add = 0,
    /**
     * 减法混合
     */
    Subtract = 1,
};

/**
 * @brief 渲染状态
 * 用于存储渲染相关的状态，如渲染类型、着色器、多边形模式、剔除模式、深度测试、深度写入、深度比较操作、模板测试、模板操作、颜色缓和、混合模式、源因子、目标因子等
 */
struct GfxRendererState
{
    /**
     * 渲染器分类
     * 用于确定渲染器的渲染目标，如UI渲染器、3D渲染器等
     */
    GfxRendererLayer layer = GfxRendererLayer::_UI;
    /**
     * 渲染器类型
     * 用于确定渲染器的渲染模式，如透明渲染、不透明渲染等
     */
    GfxRendererType type = GfxRendererType::_Opaque;
    std::string vert = "";
    std::string geom = "";
    std::string frag = "";

    /**
     * 多边形模式
     */
    GfxRendererStatePolygonMode polygonMode = GfxRendererStatePolygonMode::Fill;
    /**
     * 剔除模式
     */
    GfxRendererStateCullMode cullMode = GfxRendererStateCullMode::Back;

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
    GfxRendererStateCompareOp depthCompareOp = GfxRendererStateCompareOp::Less;

    /**
     * 模板测试
     * 开关
     */
    int stencilTest = 0;
    /**
     * 正面三角形（逆时针）的Stencil操作
     */
    GfxRendererStateCompareOp stencilFrontCompareOp = GfxRendererStateCompareOp::Equal;
    GfxRendererStateStencilOp stencilFrontFailOp = GfxRendererStateStencilOp::Keep;
    GfxRendererStateStencilOp stencilFrontDepthFailOp = GfxRendererStateStencilOp::Keep;
    GfxRendererStateStencilOp stencilFrontPassOp = GfxRendererStateStencilOp::Keep;
    uint32_t stencilFrontCompareMask = 0xFFFFFFFF;
    uint32_t stencilFrontWriteMask = 0xFFFFFFFF;
    uint32_t stencilFrontRreference = 0;
    /**
     * 反面三角形（顺时针）的Stencil操作
     */
    GfxRendererStateCompareOp stencilBackCompareOp = GfxRendererStateCompareOp::Equal;
    GfxRendererStateStencilOp stencilBackFailOp = GfxRendererStateStencilOp::Keep;
    GfxRendererStateStencilOp stencilBackDepthFailOp = GfxRendererStateStencilOp::Keep;
    GfxRendererStateStencilOp stencilBackPassOp = GfxRendererStateStencilOp::Keep;
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
    GfxRendererStateColorBlendFactor srcColorBlendFactor = GfxRendererStateColorBlendFactor::SrcAlpha;
    /**
     * 目标颜色混合因子
     */
    GfxRendererStateColorBlendFactor dstColorBlendFactor = GfxRendererStateColorBlendFactor::OneMinusSrcAlpha;
    /**
     * 颜色混合操作
     */
    GfxRendererStateColorBlendOp colorBlendOp = GfxRendererStateColorBlendOp::Add;
    /**
     * 源alpha混合因子
     */
    GfxRendererStateColorBlendFactor srcAlphaBlendFactor = GfxRendererStateColorBlendFactor::One;
    /**
     * 目标alpha混合因子
     */
    GfxRendererStateColorBlendFactor dstAlphaBlendFactor = GfxRendererStateColorBlendFactor::OneMinusSrcAlpha;
    /**
     * alpha混合操作
     */
    GfxRendererStateColorBlendOp alphaBlendOp = GfxRendererStateColorBlendOp::Add;
    /**
     * 推送常量键
     */
    std::string pushConstantKey = "";
    /**
     * 推送常量大小
     */
    VkDeviceSize pushConstantSize = 0;
    

    std::string generateKey() const
    {
        std::stringstream ss;
        ss << std::hex;
        ss << "Layer:" << int(layer) << "|";
        ss << "Vert:" << vert << "|";
        ss << "Frag:" << frag << "|";
        ss << "PolygonMode:" << static_cast<uint32_t>(polygonMode) << "|";
        ss << "CullMode:" << static_cast<uint32_t>(cullMode) << "|";
        ss << "DepthTest:" << depthTest << "|";
        ss << "DepthWrite:" << depthWrite << "|";
        ss << "DepthCompareOp:" << static_cast<uint32_t>(depthCompareOp) << "|";
        ss << "StencilTest:" << stencilTest << "|";
        ss << "StencilFrontCompareOp:" << static_cast<uint32_t>(stencilFrontCompareOp) << "|";
        ss << "StencilFrontFailOp:" << static_cast<uint32_t>(stencilFrontFailOp) << "|";
        ss << "StencilFrontDepthFailOp:" << static_cast<uint32_t>(stencilFrontDepthFailOp) << "|";
        ss << "StencilFrontPassOp:" << static_cast<uint32_t>(stencilFrontPassOp) << "|";
        ss << "StencilFrontCompareMask:" << stencilFrontCompareMask << "|";
        ss << "StencilFrontWriteMask:" << stencilFrontWriteMask << "|";
        ss << "StencilFrontRreference:" << stencilFrontRreference << "|";
        ss << "StencilBackCompareOp:" << static_cast<uint32_t>(stencilBackCompareOp) << "|";
        ss << "StencilBackFailOp:" << static_cast<uint32_t>(stencilBackFailOp) << "|";
        ss << "StencilBackDepthFailOp:" << static_cast<uint32_t>(stencilBackDepthFailOp) << "|";
        ss << "StencilBackPassOp:" << static_cast<uint32_t>(stencilBackPassOp) << "|";
        ss << "StencilBackCompareMask:" << stencilBackCompareMask << "|";
        ss << "StencilBackWriteMask:" << stencilBackWriteMask << "|";
        ss << "StencilBackRreference:" << stencilBackRreference << "|";
        ss << "ColorBlend:" << colorBlend << "|";
        ss << "SrcColorBlendFactor:" << static_cast<uint32_t>(srcColorBlendFactor) << "|";
        ss << "DstColorBlendFactor:" << static_cast<uint32_t>(dstColorBlendFactor) << "|";
        ss << "ColorBlendOp:" << static_cast<uint32_t>(colorBlendOp) << "|";
        ss << "SrcAlphaBlendFactor:" << static_cast<uint32_t>(srcAlphaBlendFactor) << "|";
        ss << "DstAlphaBlendFactor:" << static_cast<uint32_t>(dstAlphaBlendFactor) << "|";
        ss << "AlphaBlendOp:" << static_cast<uint32_t>(alphaBlendOp) << "|";
        ss << "PushConstantKey:" << pushConstantKey;
        return ss.str();
    }
};