#pragma once

#include "../../../engine/core/utils/json-util.h"

class Scene;

class EditorCacheScene
{
private:   
    int _saveFlag = 0;
    Scene* _scene;
    json* _currentSceneAssetMate;
   
    void _create();
    void _open(json *sceneMate);
    /**
     * @brief 获取场景保存路径
     * @return std::string 场景保存路径
     */
    std::string _preSavePath();
    void _serializeSceneData(Scene* scene, json& sceneData);
public:
    EditorCacheScene(json* settingConfig);
    void openScene(std::string sceneName);
    void saveScene();
    Scene* getScene();
    ~EditorCacheScene();
};