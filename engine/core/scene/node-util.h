#pragma once
#include <string>
#include "core/util/json-util.h"
namespace Boo
{
    class Node;
    class Node2D;
    class Node3D;
    class NodeUtil
    {
    private:
        static void _setNodeProperty(Node *node, json &data);
        /**
         * @brief 设置节点名称
         */
        static void _setName(Node *node, json &data);
        /**
         * @brief 设置节点 UUID
         */
        static void _setUuid(Node *node, json &data);
        /**
         * @brief 设置节点可见性
         */
        static void _setActive(Node *node, json &data);
        /**
         * @brief 设置节点位置
         */
        static void _setPosition(Node *node, json &data);
        /**
         * @brief 设置节点世界位置
         */
        static void _setWorldPosition(Node *node, json &data);
        /**
         * @brief 设置节点缩放
         */
        static void _setScale(Node *node, json &data);
        /**
         * @brief 设置节点世界缩放
         */
        static void _setWorldScale(Node *node, json &data);
        /**
         * @brief 设置节点旋转
         */
        static void _setRotation(Node *node, json &data);
        /**
         * @brief 设置节点世界旋转
         */
        static void _setWorldRotation(Node *node, json &data);
        /**
         * @brief 设置节点欧拉角度
         */
        static void _setEulerAngle(Node *node, json &data);
        /**
         * @brief 设置节点组ID
         */
        static void _setGroupId(Node *node, json &data);
        /**
         * @brief 设置节点锚点
         */
        static void _setAnchor(Node2D *node, json &data);
        /**
         * @brief 设置节点大小
         */
        static void _setSize(Node2D *node, json &data);

    public:
        /**
         * 创建 2d 节点
         * @return Node2D*
         */
        static Node2D *add2DNode(Node2D *root,json &data);
        /**
         * 创建 3d 节点
         * @return Node3D*
         */
        static Node3D *add3DNode(Node3D *root,json &data);
        /**
         * 设置 2d 节点的属性
         */
        static void set2DNodeProperty(Node2D *node, json &data);
        /**
         * 设置 3d 节点的属性
         */
        static void set3DNodeProperty(Node3D *node, json &data);
    };
}
