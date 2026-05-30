#pragma once
#include <iostream>
#include <string>
namespace Boo
{
    /**
     * @brief 组件层级
     * 与节点层级对应 2d节点对应Layer2D 3d节点对应Layer3D
     */
    enum class EComponentLayer{
        Default,
        Layer2D,
        Layer3D,
    };
}