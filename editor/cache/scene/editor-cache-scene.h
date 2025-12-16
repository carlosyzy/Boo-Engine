#pragma once

#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/json-util.h"

class Scene;

class EditorCacheScene
{
private:   
    json& _settingConfig;
    void _initSceneConfig();

    std::string _currentScene;
public:
    EditorCacheScene(json& settingConfig);
    void openScene(std::string sceneName);
    // std::string getCurrentScene();
    // void setCurrentScene(std::string scene);
    ~EditorCacheScene();
};