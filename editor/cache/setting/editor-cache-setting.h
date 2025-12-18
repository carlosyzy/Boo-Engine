#pragma once
#include <string>
#include <filesystem>
#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/json-util.h"


class EditorCacheSetting
{
private:
    std::string _settingPath;
    json _settingConfig;
public:
    EditorCacheSetting();
    void init(std::filesystem::path root);
    json *getSettingConfig();
    void update(float deltaTime);
    void saveSetting();
    ~EditorCacheSetting();
};