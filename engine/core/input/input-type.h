#pragma once
#include <functional>

namespace Boo
{
    class Node2D;

    struct FInputScrollEvent
    {
        int id;
        float offsetX;
        float offsetY;
    };
    struct FInputScrollStruct
    {
        int id;
        FInputScrollEvent event;
        std::function<void(FInputScrollEvent &)> func;
    };
    struct FInputMouseEvent
    {
        int id;
        /**
         * @brief 鼠标按钮 0: 左键 1: 右键 2: 中键
         */
        int button;
        /**
         * @brief 按键修饰符 0:无 1:Shift 2:Ctrl 3:Alt
         */
        int mods;
        /**
         * @brief 鼠标X坐标
         */
        float x;
        /**
         * @brief 鼠标Y坐标
         */
        float y;
    };
    struct FInputMouseStruct
    {
        int id;
         /**
         * @brief 滚动事件类型 1: 按下 0: 释放 2: 移动
         */
        int type;
        FInputMouseEvent event;
        std::function<void(FInputMouseEvent &)> func;
    };
    struct FNodeTouchEvent
    {
        int id;
        /**
         * @brief UI坐标X
         */
        float x;
        /**
         * @brief UI坐标Y
         */
        float y;
        /**
         * @brief UI世界坐标X
         */
        float worldX;
        /**
         * @brief UI世界坐标Y
         */
        float worldY;
        /**
         * @brief 视图坐标X
         */
        float viewX;
        /**
         * @brief 视图坐标Y
         */ 
        float viewY;
    };
    struct FNodeTouchStruct
    {
        int id;
        Node2D *node;
        FNodeTouchEvent event;
        /**
         * @brief 节点输入事件类型 1: 按下 0: 释放 2: 移动
         */
        int type;
        /**
         * @brief 是否拦截
         * 默认不拦截
         */
        bool isIntercept;
        std::function<void(FNodeTouchEvent &)> func;
    };
}
