#pragma once
#include <string>
#include <filesystem>
#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/json-util.h"


class EditorCacheSetting
{
private:
    std::string _settingPath;
    json _settingEditor;
public:
    EditorCacheSetting();
    void init(std::filesystem::path root);
    void update(float deltaTime);
    ~EditorCacheSetting();
};