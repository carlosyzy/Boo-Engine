#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "../../engine/core/utils/json-util.h"

class Scene;

class EditorSceneCache
{
private:
    std::string _scenePath;
    std::string _sceneName;
    Scene *_scene;
    /**
     * @brief 场景缓存保存标志
     * 
     * 0 无可保存操作
     * 1 有可保存操作
     */
    int _saveFlag;

    /**
     * @brief 预处理保存路径
     * 
     * @return std::string 预处理后的保存路径
     */
    std::string _preSavePath();
    /**
     * @brief 序列化场景数据
     * 
     * @param scene 场景指针
     * @return json 序列化后的场景数据
     */
    void _serializeSceneData(Scene *scene, json &sceneData);
public:
    EditorSceneCache();
    void init();
    /**
     * 打开一个新的场景
     */
    void newScene();
    /**
     * 打开一个已经存在的场景
     */
    void openScene(std::string sceneName, std::string scenePath, Scene *scene);

    std::string getSceneName();
   

    void update();
    void save();
    void clear();
    ~EditorSceneCache();
};