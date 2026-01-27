#include "editor-hierarchy.h"
#include "editor-hierarchy-layout.h"

#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/scene/node-2d.h"

EditorHierarchy::EditorHierarchy(Node2D *root)
{
    this->_rootNode = root;
}
void EditorHierarchy::init()
{
    this->_layout = new EditorHierarchyLayout(this->_rootNode);
    this->_layout->init();
}
void EditorHierarchy::setScene(Scene *scene)
{
    if (this->_layout != nullptr)
    {
        this->_layout->setScene(scene);
    }
}
void EditorHierarchy::setNode(Node *node)
{
}

void EditorHierarchy::update(float dt)
{
}

EditorHierarchy::~EditorHierarchy()
{
    this->_rootNode = nullptr;
}

// #include "editor-hierarchy-layout.h"
// EditorHierarchy::EditorHierarchy(Node2D *root)
// {
//     this->_layout = new EditorHierarchyLayout(root);
//     // EditorIpc::getInstance()->on(IpcEvent::FOCUS, &EditorHierarchy::_onFocusEvent, this);
//     // this->_layout->onRootInputStartEvent([this](NodeInputResult &result)
//     //                                      { this->_onRootInputStartEvent(result); });
//     // this->_layout->onNodeTreeSelectEvent([this](UITreeStructure *uiTreeData)
//     //                                      { this->_onNodeTreeSelectEvent(uiTreeData); });
//     // this->_layout->onNodeTreeUpdateEvent([this]()
//     //                                      { this->_onNodeTreeUpdateEvent(); });
// }
// void EditorHierarchy::setScene(Scene *scene)
// {
//     if (this->_layout != nullptr)
//     {
//         this->_layout->setScene(scene);
//     }
// }
// void EditorHierarchy::setNode(Node *node)
// {

// }
// void EditorHierarchy::update(float dt)
// {

// }
// /*
//  * @brief 更新根节点
//  */
// void EditorHierarchy::updateRoot()
// {

// }
// /*
//  * @brief 节点树更新事件
//  */
// void EditorHierarchy::_onNodeTreeUpdateEvent()
// {
//     std::cout << "EditorHierarchy update tree" << std::endl;
// }
// EditorHierarchy::~EditorHierarchy()
// {
// }