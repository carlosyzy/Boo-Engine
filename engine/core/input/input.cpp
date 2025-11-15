#include "input.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"

Input::Input()
{
}
void Input::init()
{
}
void Input::setRoot(Node2D *root)
{
    this->_root = root;
}
/**
 * @brief 鼠标按钮事件
 * @param button 鼠标按钮
 * @param action 事件动作
 * @param mods 按键修饰符
 */
void Input::onMouseButton(int button, int action, int mods)
{
    std::cout << "Input::onMouseButton  button: " << button << " action: " << action << " mods: " << mods << std::endl;
    // if (this->_root == nullptr)
    // {
    //     return;
    // }
    // // std::cout << "Input::onMouseButton 1" << std::endl;
    // // 正向遍历，拿到所有的节点
    // std::vector<Node *> _nodes;
    // // 首先收集所有节点（深度优先）
    // std::function<void(Node *)> collectNodes = [&](Node *node)
    // {
    //     Node2D *node2d = dynamic_cast<Node2D *>(node);
    //     // std::cout << "Input::onMouseButton 11 " << node2d->name() << std::endl;
    //     if (node2d != nullptr && node2d->active())
    //     {
    //         // std::cout << "Input::onMouseButton 12 " << node2d->name() << std::endl;
    //         // 点击的时候进行范围检测.
    //         if (action == 1)
    //         {
    //             // std::cout << "Input::onMouseButton 13 " << node2d->name() << std::endl;
    //             if (node2d->inMaskRect(this->_cursorX, this->_cursorY))
    //             {
    //                 // std::cout << "Input::onMouseButton ok 14 " << node2d->name() << std::endl;
    //                 _nodes.push_back(node2d);
    //                 for (auto &child : node2d->children())
    //                 {
    //                     collectNodes(child);
    //                 }
    //             }
    //             else
    //             {
    //                 // std::cout << "Input::onMouseButton fail 14 " << node2d->name() << std::endl;
    //             }
    //         }
    //         else
    //         {
    //             _nodes.push_back(node2d);
    //             for (auto &child : node2d->children())
    //             {
    //                 collectNodes(child);
    //             }
    //         }
    //     }
    // };
    // // std::cout << "Input::onMouseButton 2" << std::endl;
    // collectNodes(this->_root);
    // // std::cout << "Input::onMouseButton 3:   len" << _nodes.size() << std::endl;
    // // 反向遍历，处理事件
    // for (int i = _nodes.size() - 1; i >= 0; i--)
    // {
    //     Node *node = _nodes[i];
    //     // std::cout << "touchstart node:" << node->uuid() << std::endl;
    //     bool isIntercept = this->_propagateEvent(node, button, action);
    //     if (isIntercept)
    //     {
    //         break;
    //     }
    // }
    // // std::cout << "Input::onMouseButton 4:   len" << std::endl;
}
/**
 * @brief 鼠标移动事件
 * @param xpos 鼠标X坐标
 * @param ypos 鼠标Y坐标
 */
void Input::onCursorPos(double xpos, double ypos)
{
    // View &view = Game::getInstance()->view();
    // this->_cursorX = xpos - view.width / 2;
    // this->_cursorY = view.height / 2 - ypos;

    // if (this->_cursorX < -view.width / 2 || this->_cursorX > view.width / 2 || this->_cursorY < -view.height / 2 || this->_cursorY > view.height / 2)
    // {
    //     // 超出范围，不处理
    //     // 触发所有节点的touchEnd事件
    //     return;
    // }
    // // 触发所有节点的touchMove事件
    // // 正向遍历，拿到所有的节点
    // std::vector<Node *> _nodes;
    // // 首先收集所有节点（深度优先）
    // std::function<void(Node *)> collectNodes = [&](Node *node)
    // {
    //     Node2D *node2d = dynamic_cast<Node2D *>(node);
    //     if (node2d != nullptr)
    //     {
    //         _nodes.push_back(node2d);
    //         for (auto &child : node2d->children())
    //         {
    //             collectNodes(child);
    //         }
    //     }
    // };
    // collectNodes(this->_root);
    // // 反向遍历，处理事件
    // for (int i = _nodes.size() - 1; i >= 0; i--)
    // {
    //     Node *node = _nodes[i];
    //     bool isIntercept = this->_propagateEvent(node, 0, 99);
    //     if (isIntercept)
    //     {
    //         break;
    //     }
    // }
}

bool Input::_propagateEvent(Node *node, int button, int action)
{
    // Node2D *node2d = dynamic_cast<Node2D *>(node);
    // if (node2d == nullptr)
    // {
    //     return false;
    // }
    // //
    // if (this->_nodeInputMap.find(node2d->uuid()) != this->_nodeInputMap.end())
    // {
    //     // 点击
    //     if (action == 1)
    //     {
    //         if (node2d->inHitOnNode(this->_cursorX, this->_cursorY))
    //         {
    //             this->_nodeInputMap[node2d->uuid()].status = 1;
    //             // std::cout << "点击:" << node2d->name() << ",    " << this->_nodeInputMap[node2d->uuid()].status << ",    " << node2d->uuid() << std::endl;
    //             for (auto &touchStart : this->_nodeInputMap[node2d->uuid()].touchStarts)
    //             {
    //                 // 点击
    //                 this->_nodeInputMap[node2d->uuid()].touchResult.worldX = this->_cursorX;
    //                 this->_nodeInputMap[node2d->uuid()].touchResult.worldY = this->_cursorY;
    //                 this->_nodeInputMap[node2d->uuid()].touchResult.button = button;
    //                 touchStart(this->_nodeInputMap[node2d->uuid()].touchResult);
    //                 // 选中状态
    //                 return this->_nodeInputMap[node2d->uuid()].isIntercept;
    //             }
    //         }
    //     }
    //     else if (action == 0)
    //     {
    //         if (this->_nodeInputMap[node->uuid()].status == 1)
    //         {
    //             this->_nodeInputMap[node->uuid()].status = 0;
    //             if (node2d->inHitOnNode(this->_cursorX, this->_cursorY))
    //             {
    //                 for (auto &touchEnd : this->_nodeInputMap[node->uuid()].touchEnds)
    //                 {
    //                     this->_nodeInputMap[node->uuid()].touchResult.worldX = this->_cursorX;
    //                     this->_nodeInputMap[node->uuid()].touchResult.worldY = this->_cursorY;
    //                     this->_nodeInputMap[node->uuid()].touchResult.button = button;
    //                     touchEnd(this->_nodeInputMap[node->uuid()].touchResult);
    //                     // 取消选中状态
    //                     return this->_nodeInputMap[node->uuid()].isIntercept;
    //                 }
    //             }
    //             else
    //             {
    //                 for (auto &touchCancel : this->_nodeInputMap[node->uuid()].touchCancels)
    //                 {
    //                     this->_nodeInputMap[node->uuid()].touchResult.worldX = this->_cursorX;
    //                     this->_nodeInputMap[node->uuid()].touchResult.worldY = this->_cursorY;
    //                     this->_nodeInputMap[node->uuid()].touchResult.button = button;
    //                     touchCancel(this->_nodeInputMap[node->uuid()].touchResult);
    //                     // 取消选中状态
    //                     return this->_nodeInputMap[node->uuid()].isIntercept;
    //                 }
    //             }
    //         }
    //     }
    //     else if (action == 99)
    //     {
    //         // 移动
    //         if (this->_nodeInputMap[node->uuid()].status == 1)
    //         {
    //             // std::cout << "移动:" << node2d->name() << ",    " << this->_nodeInputMap[node->uuid()].status << std::endl;
    //             for (auto &touchMove : this->_nodeInputMap[node->uuid()].touchMoves)
    //             {
    //                 this->_nodeInputMap[node->uuid()].touchResult.worldX = this->_cursorX;
    //                 this->_nodeInputMap[node->uuid()].touchResult.worldY = this->_cursorY;
    //                 // this->_nodeInputMap[node->uuid()].touchResult.button = button;
    //                 touchMove(this->_nodeInputMap[node->uuid()].touchResult);
    //                 return this->_nodeInputMap[node->uuid()].isIntercept;
    //             }
    //         }
    //     }
    // }
    return false;
}
template <typename T, typename Func>
int Input::onNodeInputEvent(Node2D *node, NodeInput input, Func func, T *instance, bool isIntercept)
{

    int id = this->_nodeInputCallId++;
    if (this->_nodeInputMap.find(node->getUuid()) == this->_nodeInputMap.end())
    {
        NodeInputStruct nodeInputStruct;
        nodeInputStruct.node = node;
        nodeInputStruct.isIntercept = isIntercept;
        nodeInputStruct.touchResult.node = node;
        nodeInputStruct.status = 0;
        this->_nodeInputMap.emplace(node->getUuid(), nodeInputStruct);
    }
    auto callback = [func, instance](NodeInputResult &result)
    {
        if (instance != nullptr && func != nullptr)
        {
            (instance->*func)(result);
        }
    };
    if (input == NodeInput::TOUCH_START)
    {
        this->_nodeInputMap[node->getUuid()].touchStarts.emplace(id, callback);
    }
    else if (input == NodeInput::TOUCH_END)
    {
        this->_nodeInputMap[node->getUuid()].touchEnds.emplace(id, callback);
    }
    else if (input == NodeInput::TOUCH_MOVE)
    {
        this->_nodeInputMap[node->getUuid()].touchMoves.emplace(id, callback);
    }
    else if (input == NodeInput::TOUCH_CANCEL)
    {
        this->_nodeInputMap[node->getUuid()].touchCancels.emplace(id, callback);
    }
    return id;
}

void Input::offNodeInputEvent(Node2D *node, int inputID)
{
    // 遍历所有节点输入事件_nodeInputMap，在所有监听的事件中，找出指定的di，删除
    if (this->_nodeInputMap.find(node->getUuid()) != this->_nodeInputMap.end())
    {
        this->_nodeInputMap[node->getUuid()].touchStarts.erase(inputID);
        this->_nodeInputMap[node->getUuid()].touchEnds.erase(inputID);
        this->_nodeInputMap[node->getUuid()].touchMoves.erase(inputID);
        this->_nodeInputMap[node->getUuid()].touchCancels.erase(inputID);
    }
}
/**
 * @brief 取消所有节点输入事件
 * @param node 节点
 */
void Input::offAllNodeInputEvent(Node2D *node)
{
    if (this->_nodeInputMap.find(node->getUuid()) == this->_nodeInputMap.end())
    {
        this->_nodeInputMap[node->getUuid()].touchStarts.clear();
        this->_nodeInputMap[node->getUuid()].touchEnds.clear();
        this->_nodeInputMap[node->getUuid()].touchMoves.clear();
        this->_nodeInputMap[node->getUuid()].touchCancels.clear();
        this->_nodeInputMap[node->getUuid()].touchResult.node = nullptr;
    }
    this->_nodeInputMap.erase(node->getUuid());
}

Input::~Input()
{
}

// void Input::offNodeInputEvent(Node *node, NodeInput input, int inputID)
// {
// }
// void Input::offAllNodeInputEvent(Node *node)
// {
// }

// void Input::onCursorPos(double xpos, double ypos)
// {
//     if (this->_scene == nullptr)
//     {
//         return;
//     }
//     this->_cursorX = xpos;
//     this->_cursorY = ypos;
//     if (this->_cursorX < 0 || this->_cursorX > Global::viewWidth || this->_cursorY < 0 || this->_cursorY > Global::viewHeight)
//     {
//         // 超出范围，不处理
//         //触发所有节点的touchEnd事件
//         return;
//     }
//     // Node *root2d = this->_scene->root2D();
//     // if (root2d == nullptr)
//     // {
//     //     return;
//     // }
//     // 正向遍历，拿到所有的节点
//     std::vector<Node *> _nodes;
//     // 首先收集所有节点（深度优先）
//     std::function<void(Node *)> collectNodes = [&](Node *node)
//     {
//         if (node->active())
//         {
//             _nodes.push_back(node);
//             for (auto &child : node->children())
//             {
//                 collectNodes(child);
//             }
//         }
//     };
//     // collectNodes(root2d);
//     // for (int i = _nodes.size() - 1; i >= 0; i--)
//     // {
//     //     Node *node = _nodes[i];
//     //     bool isIntercept = this->_propagateEvent(node, 0, 99);
//     //     if (isIntercept)
//     //     {
//     //         break;
//     //     }
//     // }
// }
// /
// // 左键点击
// if (button == 0)
// {
//     for (auto &touchStart : this->_nodeInputMap[node->uuid()].touchStarts)
//     {
//         if (this->_isHitOnUI(node, this->_cursorX, this->_cursorY))
//         {
//             this->_nodeInputMap[node->uuid()].touchResult.worldX = this->_cursorX - Global::viewWidth / 2.0;
//             this->_nodeInputMap[node->uuid()].touchResult.worldY = Global::viewHeight / 2.0 - this->_cursorY;
//             touchStart(this->_nodeInputMap[node->uuid()].touchResult);
//             // 选中状态
//             this->_nodeInputMap[node->uuid()].status = 1;
//             return this->_nodeInputMap[node->uuid()].isIntercept;
//         }
//     }
// }
// else
// {
//     // 右键点击
// }
// bool Input::_isHitOnUI(Node *node, float touchX, float touchY)
// {
//     // Mat4 &uiMat = node->uiWorldMatrix();
//     // float x = uiMat.m12() + Global::viewWidth / 2.0;
//     // float y = Global::viewHeight - (uiMat.m13() + Global::viewHeight / 2.0);
//     // float width = uiMat.m00();
//     // float height = uiMat.m05();
//     // // std::cout << "Input::_isHitOnUI:" << node->name() << " x:" << x << " y:" << y << " width:" << width << " height:" << height << std::endl;
//     // if (touchX >= x - width / 2.0 && touchX <= x + width / 2.0 && touchY >= y - height / 2.0 && touchY <= y + height / 2.0)
//     // {
//     //     return true;
//     // }
//     return false;
// }

// void Input::onNodeInputEvent(Node *node, NodeInput input, std::function<void(NodeInputResult &)> callback, bool isIntercept)
// {
//     if (this->_nodeInputMap.find(node->uuid()) == this->_nodeInputMap.end())
//     {
//         NodeInputStruct nodeInputStruct;
//         nodeInputStruct.node = node;
//         nodeInputStruct.isIntercept = isIntercept;
//         nodeInputStruct.touchResult.node = node;
//         this->_nodeInputMap.emplace(node->uuid(), nodeInputStruct);
//     }
//     if (input == NodeInput::TOUCH_START)
//     {
//         this->_addInpotEvent(node, callback, this->_nodeInputMap[node->uuid()].touchStarts);
//     }
//     else if (input == NodeInput::TOUCH_END)
//     {
//         this->_addInpotEvent(node, callback, this->_nodeInputMap[node->uuid()].touchEnds);
//     }
//     else if (input == NodeInput::TOUCH_MOVE)
//     {
//         this->_addInpotEvent(node, callback, this->_nodeInputMap[node->uuid()].touchMoves);
//     }
// }
// void Input::_addInpotEvent(Node *node, std::function<void(NodeInputResult &)> touchEvent, std::vector<std::function<void(NodeInputResult &)>> &eventList)
// {
//     if (touchEvent == nullptr)
//     {
//         return;
//     }
//     // 检查是否已经存在相同的事件
//     for (auto &event : eventList)
//     {
//         if (event.target_type() == touchEvent.target_type() &&
//             event.target<void>() == touchEvent.target<void>())
//         {
//             return;
//         }
//     }
//     // 添加事件
//     eventList.push_back(touchEvent);
// }
// void Input::offNodeInputEvent(Node *node, NodeInput input, int inputID)
// {
//     if (input == NodeInput::TOUCH_START)
//     {
//         this->_removeInputEvent(node, callback, this->_nodeInputMap[node->uuid()].touchStarts);
//     }
//     else if (input == NodeInput::TOUCH_END)
//     {
//         this->_removeInputEvent(node, callback, this->_nodeInputMap[node->uuid()].touchEnds);
//     }
//     else if (input == NodeInput::TOUCH_MOVE)
//     {
//         this->_removeInputEvent(node, callback, this->_nodeInputMap[node->uuid()].touchEnds);
//     }
// }
// void Input::_removeInputEvent(Node *node, std::function<void(NodeInputResult &)> callback, std::vector<std::function<void(NodeInputResult &)>> &eventList)
// {
//     // 查找要删除的元素
//     auto it = std::find_if(eventList.begin(), eventList.end(),
//                            [&callback](const auto &func)
//                            {
//                                // 自定义比较逻辑
//                                if (!func && !callback)
//                                    return true;
//                                if (!func || !callback)
//                                    return false;

//                                // 比较类型和地址
//                                return func.target_type() == callback.target_type() &&
//                                       func.template target<void>() == callback.template target<void>();
//                            });

//     // 如果找到，删除它
//     if (it != eventList.end())
//     {
//         eventList.erase(it);
//     }
// }

// void Input::offAllNodeInputEvent(Node *node)
// {
//     if (this->_nodeInputMap.find(node->uuid()) != this->_nodeInputMap.end())
//     {
//         this->_nodeInputMap.erase(node->uuid());
//     }
// }