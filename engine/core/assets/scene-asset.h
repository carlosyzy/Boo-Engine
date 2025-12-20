#pragma once
#include "asset.h"
#include "../utils/json-util.h"

class Scene;
class Node;
class Node2D;
class Node3D;

class SceneAsset : public Asset
{
private:
    json _sceneData;
    std::string _name;
    Scene *_scene;
    void _deserializeScene();
    void _deserializeNode(Node *node, json &_nodeData);
    void _deserializeComponent(Node *node, json &_nodeData);
protected:
public:
    SceneAsset();
    SceneAsset(const std::string uuid);
    void create(std::string path) override;
    Scene *getScene();
    ~SceneAsset();
};
