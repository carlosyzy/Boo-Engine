#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
#include "gfx-struct.h"
/**
 * 深度比较操作
 */
enum class GfxPipelineDepthCompareOp
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
enum class GfxPipelineStencilOp
{
    /**
     * 保持原值
     */
    Keep = 0,


    Increment_Add=3

};
enum class GfxPipelineStencilOp
{
    /**
     * 保持原值
     */
    Keep = 0,

};

struct GfxPipelineStruct
{
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
    GfxPipelineDepthCompareOp depthCompareOp = GfxPipelineDepthCompareOp::LessOrEqual;

    /**
     * 模板测试
     * 开关
     */
    int stencilTest = 0;
    /**
     * 正面三角形（逆时针）的Stencil操作
     */
    GfxPipelineDepthCompareOp stencilFrontCompareOp = GfxPipelineDepthCompareOp::Never;
    GfxPipelineStencilOp stencilFrontFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add;

    uint32_t stencilFrontCompareMask = 0xFFFFFFFF;
    uint32_t stencilFrontWriteMask= 0xFFFFFFFF;
    uint32_t stencilFrontRreference= 0;
    /**
     * 反面三角形（顺时针）的Stencil操作
     */
    GfxPipelineDepthCompareOp stencilBackCompareOp = GfxPipelineDepthCompareOp::Never;
    GfxPipelineStencilOp stencilBackFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    GfxPipelineStencilOp stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
    uint32_t stencilBackCompareMask = 0xFFFFFFFF;
    uint32_t stencilBackWriteMask= 0xFFFFFFFF;
    uint32_t stencilBackRreference= 0;

    /**
     * 颜色缓和
     * 开关
     */
    int colorBlend = 0;
    /**
     * 混合模式
     */
    int colorBlendMode=0;


    int polygonMode = 0;
    int cullMode = 0;
};