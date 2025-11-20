#pragma once
enum class GfxObjectType;
/**
 * 渲染队列状态
 */
struct GfxQueueRendererStatus
{
    /**
     * @brief 渲染对象类型
     */
    GfxObjectType objectType;
    /**
     * @brief 渲染管线名称
     */
    std::string pipelineName;
    
};