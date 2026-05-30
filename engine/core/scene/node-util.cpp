#include "node-util.h"
#include "core/scene/node-2d.h"
#include "core/scene/node-3d.h"
#include "core/scene/scene.h"
#include "boo.h"
namespace Boo
{
    Node2D *NodeUtil::add2DNode(Node2D *root,json &data)
    {
        Node2D *parent = root;
        if (parent == nullptr)
        {
            Scene *scene = game->getScene();
            if (!scene)
            {
                return nullptr;
            }
            parent = scene->getRoot2D();
        }
        std::string name = "2DNode" + std::to_string(parent->getChildren().size());
        Node2D *node = new Node2D(name);
        parent->addChild(node);
        NodeUtil::_setNodeProperty(node, data);
        return node;
    }
    Node3D *NodeUtil::add3DNode(Node3D *root,json &data)
    {
        Node3D *parent = root;
        if (parent == nullptr)
        {
            Scene *scene = game->getScene();
            if (!scene)
            {
                return nullptr;
            }
            parent = scene->getRoot3D();
        }
        std::string name = "3DNode" + std::to_string(parent->getChildren().size());
        Node3D *node = new Node3D(name);
        parent->addChild(node);
        NodeUtil::_setNodeProperty(node, data);
        return node;
    }
    void NodeUtil::set2DNodeProperty(Node2D *node, json &data)
    {
        NodeUtil::_setNodeProperty(node, data);
        if (data.contains("size"))
        {
            NodeUtil::_setSize(node, data["size"]);
        }
        else if (data.contains("anchor"))
        {
            NodeUtil::_setAnchor(node, data["anchor"]);
        }
    }

    void NodeUtil::set3DNodeProperty(Node3D *node, json &data)
    {
        NodeUtil::_setNodeProperty(node, data);
    }
    void NodeUtil::_setNodeProperty(Node *node, json &data)
    {
        if (node == nullptr)
        {
            return;
        }
        if (!data.is_object())
        {
            return;
        }
        for (auto &property : data.items())
        {
            std::string key = property.key();
            json &value = property.value();
            if (key == "name")
            {
                NodeUtil::_setName(node, value);
            }
            else if (key == "uuid")
            {
                NodeUtil::_setUuid(node, value);
            }
            else if (key == "active")
            {
                NodeUtil::_setActive(node, value);
            }
            else if (key == "position")
            {
                NodeUtil::_setPosition(node, value);
            }
            else if (key == "worldPosition")
            {
                NodeUtil::_setWorldPosition(node, value);
            }
            else if (key == "scale")
            {
                NodeUtil::_setScale(node, value);
            }
            else if (key == "worldScale")
            {
                NodeUtil::_setWorldScale(node, value);
            }
            else if (key == "rotation")
            {
                NodeUtil::_setRotation(node, value);
            }
            else if (key == "worldRotation")
            {
                NodeUtil::_setWorldRotation(node, value);
            }
            else if (key == "eulerAngle")
            {
                NodeUtil::_setEulerAngle(node, value);
            }
            else if (key == "groupId")
            {
                NodeUtil::_setGroupId(node, value);
            }
        }
    }
    void NodeUtil::_setName(Node *node, json &data)
    {
        if (!data.is_string())
        {
            return;
        }
        if (data.get<std::string>().empty())
        {
            return;
        }
        std::string name = data.get<std::string>();
        node->setName(name);
    }
    void NodeUtil::_setUuid(Node *node, json &data)
    {
        if (!data.is_string())
        {
            return;
        }
        if (data.get<std::string>().empty())
        {
            return;
        }
        std::string uuid = data.get<std::string>();
        node->setUuid(uuid);
    }
    void NodeUtil::_setActive(Node *node, json &data)
    {
        if (!data.is_boolean())
        {
            return;
        }
        bool active = data.get<bool>();
        node->setActive(active);
    }
    void NodeUtil::_setPosition(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        node->setPosition(x, y, z);
    }
    void NodeUtil::_setWorldPosition(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        node->setWorldPosition(x, y, z);
    }
    void NodeUtil::_setScale(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        node->setScale(x, y, z);
    }
    void NodeUtil::_setWorldScale(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        node->setWorldScale(x, y, z);
    }
    void NodeUtil::_setRotation(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        if (data.contains("w") && data["w"].is_number())
        {
            w = data["w"].get<float>();
        }
        node->setRotation(x, y, z, w);
    }
    void NodeUtil::_setWorldRotation(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        if (data.contains("w") && data["w"].is_number())
        {
            w = data["w"].get<float>();
        }
        node->setWorldRotation(x, y, z, w);
    }
    void NodeUtil::_setEulerAngle(Node *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        if (data.contains("z") && data["z"].is_number())
        {
            z = data["z"].get<float>();
        }
        node->setEulerAngles(x, y, z);
    }
    /**
     * @brief 设置节点组ID
     */
    void NodeUtil::_setGroupId(Node *node, json &data)
    {
        if (!data.is_number())
        {
            return;
        }
        int groupId = data.get<int>();
        node->setGroupId(groupId);
    }
    /**
     * @brief 设置节点锚点
     */
    void NodeUtil::_setAnchor(Node2D *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        if (data.contains("x") && data["x"].is_number())
        {
            x = data["x"].get<float>();
        }
        if (data.contains("y") && data["y"].is_number())
        {
            y = data["y"].get<float>();
        }
        node->setAnchor(x, y);
    }
    /**
     * @brief 设置节点大小
     */
    void NodeUtil::_setSize(Node2D *node, json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        float x = 0.0f;
        float y = 0.0f;
        if (data.contains("width") && data["width"].is_number())
        {
            x = data["width"].get<float>();
        }
        if (data.contains("height") && data["height"].is_number())
        {
            y = data["height"].get<float>();
        }
        node->setSize(x, y);
    }

}
