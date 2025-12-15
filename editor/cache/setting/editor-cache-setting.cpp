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
        this->_settingEditor = FileUtil::readJsonFromBinary(this->_settingPath);
    }
    else
    {
        this->_settingEditor = json::object();
        FileUtil::saveJsonToBinary(this->_settingPath, this->_settingEditor);
    }
    std::cout << "EditorCache::saveAssetsDB: " << this->_settingEditor << std::endl;
}
void EditorCacheSetting::update(float deltaTime)
{
}
EditorCacheSetting::~EditorCacheSetting()
{
}