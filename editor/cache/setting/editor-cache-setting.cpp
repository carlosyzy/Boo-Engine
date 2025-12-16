#include "editor-cache-setting.h"
EditorCacheSetting::EditorCacheSetting()
{
}
void EditorCacheSetting::init(std::filesystem::path root)
{
    this->_settingPath = (root / "setting.bin").generic_string();
    // 直接读取文件,解析文件
    if (std::filesystem::exists(this->_settingPath))
    {
        this->_settingConfig = FileUtil::readJsonFromBinary(this->_settingPath);
    }
    else
    {
        this->_settingConfig = json::object();
        FileUtil::saveJsonToBinary(this->_settingPath, this->_settingConfig);
    }
    std::cout << "EditorCacheSetting::init: " << this->_settingConfig << std::endl;
}
json &EditorCacheSetting::getSettingConfig()
{
    std::cout << "EditorCacheSetting::getSettingConfig: " << this->_settingConfig << std::endl;
    return this->_settingConfig;
}
void EditorCacheSetting::update(float deltaTime)
{
}
EditorCacheSetting::~EditorCacheSetting()
{
}