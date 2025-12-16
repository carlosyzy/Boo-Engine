#pragma once
#include "asset.h"
#include "../utils/json-util.h"

class Scene;
class SceneAsset : public Asset
{
private:
    json _sceneData;
    std::string _name;
    Scene *_scene;
    void _deserializeScene();
protected:
public:
    SceneAsset();
    SceneAsset(const std::string uuid);
    void create(std::string path) override;
    Scene *getScene() { return this->_scene; }
    ~SceneAsset();
};
