#pragma once
/**
 * @brief 渲染对象类型
 */
enum class GfxObjectType
{
    /**
     * @brief 无类型对象
     */
    NONE = 0,
    /**
     * @brief UI对象
     */
    UI = 1,
    /**
     * @brief UI遮罩对象
     */
    UI_MASK = 2,
    /**
     * @brief 3D模型对象
     */
    MODEL = 3,
};