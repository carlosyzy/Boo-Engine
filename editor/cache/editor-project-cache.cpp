#include "editor-project-cache.h"
#include "../boo-editor.h"

EditorProjectCache::EditorProjectCache()
{
}
void EditorProjectCache::init()
{
    std::filesystem::path projectCachePath = std::filesystem::path(BooEditor::projectPath) / "setting" / "project.bin";
    if (std::filesystem::is_regular_file(projectCachePath))
    {
        this->_projectCache = FileUtil::loadJsonFromBinary(projectCachePath.string());
    }
    else
    {
        this->_projectCache = json::object();
        this->_projectCache["name"] = std::filesystem::path(BooEditor::projectPath).filename().string();
        this->_projectCache["width"] = 1280;
        this->_projectCache["height"] = 720;
        this->_projectCache["launchScene"] = "";
    }
    std::cout << "init:Project Cache: " << this->_projectCache << std::endl;
    this->update();
}
void EditorProjectCache::update()
{

    std::filesystem::path projectCachePath = std::filesystem::path(BooEditor::projectPath) / "setting" / "project.bin";
    std::cout << "update:Project Cache: " << this->_projectCache << std::endl;
    FileUtil::saveJsonToBinary(projectCachePath.string(), this->_projectCache);
}

EditorProjectCache::~EditorProjectCache()
{
}

