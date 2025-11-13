#include "editor-hierarchy.h"
#include "editor-hierarchy-layout.h"
// #include "../../engine/core/renderer/node/node.h"
// #include "../../engine/core/renderer/node/node-2d.h"
// #include "../../engine/core/renderer/scene.h"
// #include "../../engine/core/ui/ui-tree/node-tree.h"
// #include "../../engine/core/input/input-mgr.h"

EditorHierarchy::EditorHierarchy(Node2D *root)
{
    this->_layout = new EditorHierarchyLayout(root);
    // EditorIpc::getInstance()->on(IpcEvent::FOCUS, &EditorHierarchy::_onFocusEvent, this);
    // this->_layout->onRootInputStartEvent([this](NodeInputResult &result)
    //                                      { this->_onRootInputStartEvent(result); });
    // this->_layout->onNodeTreeSelectEvent([this](UITreeStructure *uiTreeData)
    //                                      { this->_onNodeTreeSelectEvent(uiTreeData); });
    // this->_layout->onNodeTreeUpdateEvent([this]()
    //                                      { this->_onNodeTreeUpdateEvent(); });
}
void EditorHierarchy::setScene(Scene *scene)
{
    // this->_layout->setScene(scene);
    // this->_rootScene = scene;
    // this->_rootNode = nullptr;
}
void EditorHierarchy::setNode(Node *node)
{
    // this->_layout->setNode(node);
    // this->_rootNode = node;
    // this->_rootScene = nullptr;
}
void EditorHierarchy::update(float dt)
{
    
}
/*
 * @brief 更新根节点
 */
void EditorHierarchy::updateRoot()
{
    // this->_layout->updateTree();
}
// void EditorHierarchy::_onFocusEvent(const EventValue &value)
// {
//     if (std::holds_alternative<std::string>(value))
//     {
//         std::string focus = std::get<std::string>(value);
//         std::cout << "EditorHierarchy Focus: " << focus << std::endl;
//         if (focus != "EditorHierarchy")
//         {
//             this->_layout->loseFocus();
//         }
//     }
// }
// void EditorHierarchy::_onRootInputStartEvent(NodeInputResult &result)
// {
//     std::cout << "EditorHierarchy Lock the focus" << std::endl;
//     EditorIpc::getInstance()->send(IpcEvent::FOCUS, std::string("EditorHierarchy"));
//     EditorIpc::getInstance()->send(IpcEvent::SELECT_NODE, std::string(""));
//     this->_layout->clearNodeTreeSelect();
// }
/*
 * @brief 节点树选择事件
 */
// void EditorHierarchy::_onNodeTreeSelectEvent(UITreeStructure *uiTreeData)
// {
//     EditorIpc::getInstance()->send(IpcEvent::FOCUS, std::string("EditorHierarchy"));
//     if (uiTreeData != nullptr)
//     {
//         std::cout << "EditorHierarchyLayout select node:" << uiTreeData->name << ": uuid:" << uiTreeData->uuid << std::endl;
//         EditorIpc::getInstance()->send(IpcEvent::SELECT_NODE, uiTreeData->uuid);
//     }
//     else
//     {
//         EditorIpc::getInstance()->send(IpcEvent::SELECT_NODE, std::string(""));
//         this->_layout->clearNodeTreeSelect();
//         std::cout << "EditorHierarchyLayout select node: nullptr" << std::endl;
//     }
// }
/*
 * @brief 节点树更新事件
 */
void EditorHierarchy::_onNodeTreeUpdateEvent()
{
    std::cout << "EditorHierarchy update tree" << std::endl;
}
EditorHierarchy::~EditorHierarchy()
{
}