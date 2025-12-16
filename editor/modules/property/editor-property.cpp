// #include "editor-property.h"
// #include "editor-property-layout.h"
// #include "../../engine/core/renderer/node/node.h"
// #include "../../engine/core/renderer/node/node-2d.h"

// EditorProperty::EditorProperty(Node2D *root)
// {
//     this->_layout = new EditorPropertyLayout(root);
//     // 初始化事件
//     this->_layout->onRootInputEndEvent([this](NodeInputResult &result)
//                                        { this->_onRootInputEndEvent(result); });

//     EditorIpc::getInstance()->on(IpcEvent::FOCUS, &EditorProperty::_onFocusEvent, this);
//     EditorIpc::getInstance()->on(IpcEvent::SELECT_NODE, &EditorProperty::_onSelectNode, this);
// }
// void EditorProperty::_onFocusEvent(const EventValue &value)
// {
//     if (std::holds_alternative<std::string>(value))
//     {
//         std::string focus = std::get<std::string>(value);
//         std::cout << "EditorProperty Focus: " << focus << std::endl;
//         if (focus != "EditorProperty")
//         {
//         }
//     }
// }
// void EditorProperty::_onRootInputEndEvent(NodeInputResult &result)
// {
//     EditorIpc::getInstance()->send(IpcEvent::FOCUS, std::string("EditorProperty"));
// }
// void EditorProperty::_onSelectNode(const EventValue &value)
// {
//     if (std::holds_alternative<std::string>(value))
//     {
//         std::string nodeUid = std::get<std::string>(value);
//         std::cout << "EditorProperty SelectNode: " << nodeUid << std::endl;
//         if (nodeUid != "")
//         {
//             this->_layout->setNode(nullptr);
//         }
//     }
// }
// Node *EditorProperty::_getSelectNode(std::string uid)
// {
//     if (uid.length() <= 0)
//     {
//         return nullptr;
//     }
// }
// void EditorProperty::update(float dt)
// {
// }

// EditorProperty::~EditorProperty()
// {
// }