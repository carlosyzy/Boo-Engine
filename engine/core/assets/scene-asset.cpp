#include "scene-asset.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"
#include "../scene/scene.h"
#include "../component/component-property-reflect.h"

SceneAsset::SceneAsset() : Asset()
{
    this->_type = AssetType::Scene;
}
SceneAsset::SceneAsset(const std::string uuid) : Asset(uuid)
{
    this->_type = AssetType::Scene;
}
void SceneAsset::create(std::string path)
{
    this->_path = path;
    // 获取资源文件名作为资源名
    this->_name = std::filesystem::path(path).stem().string();
    // 加载场景数据
    this->_sceneData = FileUtil::readJsonFromBinary(this->_path);
    // 反序列化场景数据
    this->_deserializeScene();
}
void SceneAsset::_deserializeScene()
{
    std::cout << "SceneAsset::_deserializeScene:" << this->_sceneData << std::endl;
    // std::function<void(Node *, json &)> _deserializeNodes = [&](Node *node, json &_childData)
    std::function<void(Node *, json &)> _deserializeNodes = [&](Node *node, json &_nodeData)
    {
        this->_deserializeNode(node, _nodeData);


        // for (json &childData : _childData)
        // {
        //     if (childData["_layer"] == NodeLayer::Node2D)
        //     {
        //         Node2D *child2D = new Node2D(childData["_name"].get<std::string>(), childData["_uuid"].get<std::string>());
        //         node->addChild(child2D);
        //         this->_deserializeNode(child2D, childData["_children"]);
        //         this->_deserializeComponent(child2D, childData);

        //         std::cout << "SceneAsset::_deserializeScene: " << child2D->getName() << std::endl;
        //         _deserializeNodes(child2D, childData["_children"]);
        //     }
        //     else if (childData["_layer"] == NodeLayer::Node3D)
        //     {
        //         Node3D *child3D = new Node3D(childData["_name"].get<std::string>(), childData["_uuid"].get<std::string>());
        //         node->addChild(child3D);
        //         this->_deserializeNode(child3D, childData["_children"]);
        //         this->_deserializeComponent(child3D, childData);
        //         std::cout << "SceneAsset::_deserializeScene: " << child3D->getName() << std::endl;
        //         _deserializeNodes(child3D, childData["_children"]);
        //     }
        // }
    };
    json &sceneData = this->_sceneData["_scene"];
    this->_scene = new Scene(sceneData["_name"].get<std::string>(), sceneData["_uuid"].get<std::string>());
    // root3d
    Node3D *root3d = this->_scene->getRoot3D();
    _deserializeNodes(root3d, sceneData["_root3d"]);
   
    // root2d
    Node2D *root2d = this->_scene->getRoot2D();
    _deserializeNodes(root2d, sceneData["_root2d"]);
    



    _deserializeNodes(this->_scene, sceneData["_scene"]["_children"]);
    std::cout << "SceneAsset::_deserializeScene: " << this->_scene << std::endl;
}
void SceneAsset::_deserializeNode(Node *node, json &_nodeData)
{
    // // name
    // if (_nodeData.contains("_name"))
    // {
    //     node->setName(_nodeData["_name"].get<std::string>());
    // }
    // // uuid
    // if (_nodeData.contains("_uuid"))
    // {
    //     node->setUuid(_nodeData["_uuid"].get<std::string>());
    // }
    // // layer
    // if (_nodeData.contains("_layer"))
    // {
    //     node->setLayer((NodeLayer)_nodeData["_layer"].get<int>());
    // }


    // position
    if (_nodeData.contains("_position"))
    {
        json &position = _nodeData["_position"];
        if (position.size() == 3)
        {
            node->setPosition(position[0].get<float>(), position[1].get<float>(), position[2].get<float>());
        }
        else if (position.size() == 2)
        {
            node->setPosition(position[0].get<float>(), position[1].get<float>(), 0.0f);
        }
        else if (position.size() == 1)
        {
            node->setPosition(position[0].get<float>(), 0.0f, 0.0f);
        }
        else
        {
            node->setPosition(0.0f, 0.0f, 0.0f);
        }
    }
    // rotation
    if (_nodeData.contains("_rotation"))
    {
        json &rotation = _nodeData["_rotation"];
        if (rotation.size() == 3)
        {
            node->setEulerAngles(rotation[0].get<float>(), rotation[1].get<float>(), rotation[2].get<float>());
        }
        else if (rotation.size() == 2)
        {
            node->setEulerAngles(rotation[0].get<float>(), rotation[1].get<float>(), 0.0f);
        }
        else if (rotation.size() == 1)
        {
            node->setEulerAngles(rotation[0].get<float>(), 0.0f, 0.0f);
        }
        else
        {
            node->setEulerAngles(0.0f, 0.0f, 0.0f);
        }
    }
    // scale
    if (_nodeData.contains("_scale"))
    {
        json &scale = _nodeData["_scale"];
        if (scale.size() == 3)
        {
            node->setScale(scale[0].get<float>(), scale[1].get<float>(), scale[2].get<float>());
        }
        else if (scale.size() == 2)
        {
            node->setScale(scale[0].get<float>(), scale[1].get<float>(), 0.0f);
        }
        else if (scale.size() == 1)
        {
            node->setScale(scale[0].get<float>(), 0.0f, 0.0f);
        }
        else
        {
            node->setScale(0.0f, 0.0f, 0.0f);
        }
    }
    // visibility
    if (_nodeData.contains("_visibility"))
    {
        node->setVisibility(_nodeData["_visibility"].get<int>());
    }
    // active
    if (_nodeData.contains("_active"))
    {
        node->setActive(_nodeData["_active"].get<bool>());
    }
    if (node->getLayer() == NodeLayer::Node2D)
    {
        // size
        if (_nodeData.contains("_size"))
        {
            json &size = _nodeData["_size"];
            Node2D *node2d = static_cast<Node2D *>(node);
            if (size.size() == 2)
            {
                node2d->setSize(size[0].get<float>(), size[1].get<float>());
            }
            else if (size.size() == 1)
            {
                node2d->setSize(size[0].get<float>(), 0.0f);
            }
            else
            {
                node2d->setSize(0.0f, 0.0f);
            }
        }
    }
}
void SceneAsset::_deserializeComponent(Node *node, json &_nodeData)
{
    if (!_nodeData.contains("_component"))
    {
        return;
    }
    json &components = _nodeData["_components"];
    if (!components.is_array() || components.size() <= 0)
    {
        return;
    }
    for (json &component : components)
    {
        if (component.contains("_type"))
        {
            std::string name = component["_name"].get<std::string>();
            std::string uuid = component["_uuid"].get<std::string>();
            Component *comp = node->addComponent(name, uuid);
            if (!comp)
            {
                std::cout << "component " << name << " not found" << std::endl;
            }
            if (component.contains("_enabled"))
            {
                int enabled = component["_enabled"].get<int>();
                comp->setEnabled(enabled == 1);
            }
            if (component.contains("_properties"))
            {
                json &properties = component["_properties"];
                for (json &property : properties)
                {
                    ReflectionRegistry::getInstance().deserializeFromJson((void *)comp, comp->getName(), property);
                }
            }
        }
    }
}
Scene *SceneAsset::getScene()
{
    return this->_scene;
}

SceneAsset::~SceneAsset()
{
}
