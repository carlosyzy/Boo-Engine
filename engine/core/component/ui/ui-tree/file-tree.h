// #pragma once

// #include <string>
// #include <unordered_map>
// #include <filesystem>
// #include "ui-tree.h"
// #include "../component/component-register.h"
// /**
//  * @brief 文件树
//  * 用于显示项目中的文件和文件夹
//  * 可以展开和折叠文件夹
//  * 可以选择文件
//  */
// class FileTree : public UITree
// {
// private:
//     std::string root;

//     void _setTrees(std::filesystem::directory_entry entry, UITreeStructure &uiTreeData, int layer);
// public:
//     FileTree(std::string name, Node *node, std::string uuid = "");
//     void setRoot(std::string root);
//     void updateTree() override;
//     void Update(float deltaTime) override;
//     void LateUpdate(float deltaTime) override;
//     void Render() override;
//     void destroy() override;
//     ~FileTree();
// };
// REGISTER_COMPONENT(FileTree, "FileTree", "文件树组件")
