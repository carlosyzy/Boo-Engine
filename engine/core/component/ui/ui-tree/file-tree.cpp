// #include "file-tree.h"
// #include <iostream>

// FileTree::FileTree(Node *node, std::string uuid) : UITree(node, uuid)
// {
    
// }
// void FileTree::onLoad()
// {
//     UITree::onLoad();
// }
// void FileTree::onEnable()
// {
//     UITree::onEnable();
// }
// void FileTree::setRoot(std::string root)
// {
//     this->_isDirty = true;
//     this->root = root;
//     this->_uiTreeData.uuid = root;
//     this->_uiTreeData.pathBind = root;
//     this->_uiTreeData.layer = 0;
//     this->_uiTreeData.icon = "resources/textures/ic-project.png";
//     this->_uiTreeData.isFold = false;
//     this->_uiTreeData.name = std::filesystem::path(root).filename().string();
//     std::vector<std::filesystem::directory_entry> entries;
//     for (const auto &entry : std::filesystem::directory_iterator(this->_uiTreeData.pathBind))
//     {
//         entries.push_back(entry);
//     }
//     this->_uiTreeData.children.resize(entries.size());
//     for (int i = 0; i < entries.size(); i++)
//     {
//         this->_setTrees(entries[i], this->_uiTreeData.children[i], this->_uiTreeData.layer + 1);
//     }
// }
// void FileTree::updateTree()
// {
//     this->setRoot(this->root);
//     UITree::updateTree();
// }
// void FileTree::_setTrees(std::filesystem::directory_entry entry, UITreeStructure &uiTreeData, int layer)
// {
//     if (entry.is_directory())
//     {
//         // 文件夹
//         uiTreeData.uuid = entry.path().string();
//         uiTreeData.pathBind = entry.path().string();
//         uiTreeData.layer = layer;
//         uiTreeData.icon = "resources/textures/ic-folder.png";
//         uiTreeData.isFold = false;
//         uiTreeData.name = entry.path().filename().string();
//         std::vector<std::filesystem::directory_entry> entries;
//         for (const auto &entry : std::filesystem::directory_iterator(uiTreeData.uuid))
//         {
//             entries.push_back(entry);
//         }
//         uiTreeData.children.resize(entries.size());
//         // 文件夹目录
//         for (int i = 0; i < entries.size(); i++)
//         {
//             this->_setTrees(entries[i], uiTreeData.children[i], uiTreeData.layer + 1);
//         }
//     }
//     else if (entry.is_regular_file())
//     {
//         // 文件
//         uiTreeData.uuid = entry.path().string();
//         uiTreeData.pathBind = entry.path().string();
//         uiTreeData.layer = layer;
//         // 文件类型
//         std::string extension = entry.path().extension().string();
//         if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG")
//         {
//             uiTreeData.icon = "resources/textures/ic-image.png";
//         }
//         else if (extension == ".vert")
//         {
//             uiTreeData.icon = "resources/textures/ic-shader-v.png";
//         }
//         else if (extension == ".frag")
//         {
//             uiTreeData.icon = "resources/textures/ic-shader-f.png";
//         }
//         else if (extension == ".mtl")
//         {
//             uiTreeData.icon = "resources/textures/ic-material.png";
//         }
//         else if (extension == ".md")
//         {
//             uiTreeData.icon = "resources/textures/ic-readme.png";
//         }
//         else
//         {
//             uiTreeData.icon = "resources/textures/ic-2d.png";
//         }
//         uiTreeData.isFold = false;
//         uiTreeData.name = entry.path().filename().string();
//     }
// }

// void FileTree::update(float deltaTime)
// {
//     UITree::update(deltaTime);
// }
// void FileTree::onDisable()
// {
//     UITree::onDisable();
// }
// void FileTree::onDestroy()
// {
//     UITree::onDestroy();
// }
// FileTree::~FileTree()
// {
// }
