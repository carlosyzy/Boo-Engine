#include "editor-cache-scene.h"

// 引用初始化
EditorCacheScene::EditorCacheScene(json &settingConfig) : _settingConfig(settingConfig)
{
    std::cout << "EditorCacheScene::EditorCacheScene" << std::endl;
    this->_initSceneConfig();
}
void EditorCacheScene::_initSceneConfig()
{
    std::cout << "EditorCacheScene::_initSceneConfig:" <<this->_settingConfig<< std::endl;
    if (!this->_settingConfig.contains("scene"))
    {
        this->_settingConfig["scene"] = ""; //
    }
}
/**
 * @brief 打开场景
 * @param sceneName 场景名称
 */
void EditorCacheScene::openScene(std::string sceneName)
{
    this->_currentScene = sceneName;
    if (sceneName.empty())
    {
        if (this->_settingConfig["scene"].empty())
        {
            this->_currentScene = ""; // 打开一个空场景
        }
    }
   
}

// /**
//  * @brief 获取当前场景
//  * @return std::string 当前场景
//  */
// std::string EditorCacheScene::getCurrentScene()
// {

//     return this->_settingConfig["scene"]["current"];
// }
// /**
//  * @brief 设置当前场景
//  * @param scene 场景
//  */
// void EditorCacheScene::setCurrentScene(std::string scene)
// {
//     this->_settingConfig["scene"]["current"] = scene;
// }
EditorCacheScene::~EditorCacheScene()
{
}
