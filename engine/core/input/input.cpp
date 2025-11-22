#include "input.h"
#include "../../boo.h"
#include "../game.h"
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
    // std::cout << "Input::onMouseButton  button: " << button << " action: " << action << " mods: " << mods << std::endl;
    if (this->_root == nullptr)
    {
        return;
    }
    // 正向遍历，拿到所有的节点
    std::vector<Node *> _nodes;
    // 首先收集所有节点（深度优先）
    std::function<void(Node *)> collectNodes = [&](Node *node)
    {
        Node2D *node2d = dynamic_cast<Node2D *>(node);
        if (node2d != nullptr && node2d->isActive())
        {
            if ((action == 1 && node2d->inHitMask(this->_cursorX, this->_cursorY)) || action == 0)
            {
                _nodes.push_back(node2d);
                for (auto &child : node2d->getChildren())
                {
                    collectNodes(child);
                }
            }
        }
    };
    collectNodes(this->_root);
    std::cout << "Input::_propagateEvent: x: " << this->_cursorX << " y: " << this->_cursorY << " is pressed" << std::endl;
    // 反向遍历，处理事件
    for (int i = _nodes.size() - 1; i >= 0; i--)
    {
        Node *node = _nodes[i];
        bool isIntercept = this->_propagateEvent(node, button, action);
        if (isIntercept)
        {
            break;
        }
    }
}
/**
 * @brief 鼠标移动事件
 * @param xpos 鼠标X坐标
 * @param ypos 鼠标Y坐标
 */
void Input::onCursorPos(double xpos, double ypos)
{
    View *view = Boo::game->view();
    this->_cursorX = xpos - view->width / 2;
    this->_cursorY = view->height / 2 - ypos;
    if (this->_cursorX < -view->width / 2 || this->_cursorX > view->width / 2 || this->_cursorY < -view->height / 2 || this->_cursorY > view->height / 2)
    {
        // 超出范围，不处理
        return;
    }
    // std::cout << "Input::onCursorPos  cursorX: " << this->_cursorX << " cursorY: " << this->_cursorY << std::endl;
    // 触发所有节点的touchMove事件
    // 正向遍历，拿到所有的节点
    std::vector<Node *> _nodes;
    // 首先收集所有节点（深度优先）
    std::function<void(Node *)> collectNodes = [&](Node *node)
    {
        Node2D *node2d = dynamic_cast<Node2D *>(node);
        if (node2d != nullptr && node2d->isActive())
        {
            _nodes.push_back(node2d);
            for (auto &child : node2d->getChildren())
            {
                collectNodes(child);
            }
        }
    };
    collectNodes(this->_root);
    // 反向遍历，处理事件
    for (int i = _nodes.size() - 1; i >= 0; i--)
    {
        Node *node = _nodes[i];
        bool isIntercept = this->_propagateEvent(node, 0, 99);
        if (isIntercept)
        {
            break;
        }
    }
}

bool Input::_propagateEvent(Node *node, int button, int action)
{
    Node2D *node2d = dynamic_cast<Node2D *>(node);
    if (node2d == nullptr)
    {
        return false;
    }
    if (this->_nodeInputMap.find(node2d->getUuid()) == this->_nodeInputMap.end())
    {
        return false;
    }
    if (action == 1) // 按下
    {

        bool isIn = node2d->inHitOnNode(this->_cursorX, this->_cursorY);
        if (isIn)
        {
            NodeInputStruct &nodeInput = this->_nodeInputMap[node2d->getUuid()];
            nodeInput.status = 1;
            nodeInput.touchResult.worldX = this->_cursorX;
            nodeInput.touchResult.worldY = this->_cursorY;
            nodeInput.touchResult.button = button;
            std::map<int, std::function<void(NodeInputResult &)>> &touchStarts = this->_nodeInputMap[node2d->getUuid()].touchStarts;
            for (auto it = touchStarts.begin(); it != touchStarts.end(); ++it)
            {
                if (it->second)
                {
                    it->second(this->_nodeInputMap[node2d->getUuid()].touchResult);
                }
            }
            return nodeInput.isIntercept;
        }
    }
    else if (action == 0) // 松开
    {
        NodeInputStruct &nodeInput = this->_nodeInputMap[node2d->getUuid()];
        nodeInput.status = 0;
        nodeInput.touchResult.worldX = this->_cursorX;
        nodeInput.touchResult.worldY = this->_cursorY;
        nodeInput.touchResult.button = button;
        bool isIn = node2d->inHitOnNode(this->_cursorX, this->_cursorY);
        if (isIn)
        {
            std::map<int, std::function<void(NodeInputResult &)>> &touchEnds = this->_nodeInputMap[node2d->getUuid()].touchEnds;
            for (auto it = touchEnds.begin(); it != touchEnds.end(); ++it)
            {
                if (it->second)
                {
                    it->second(this->_nodeInputMap[node2d->getUuid()].touchResult);
                }
            }
            return nodeInput.isIntercept;
        }
        else
        {
            if (nodeInput.status == 1)
            {
                std::map<int, std::function<void(NodeInputResult &)>> &touchCancels = this->_nodeInputMap[node2d->getUuid()].touchCancels;
                for (auto it = touchCancels.begin(); it != touchCancels.end(); ++it)
                {
                    if (it->second)
                    {
                        it->second(this->_nodeInputMap[node2d->getUuid()].touchResult);
                    }
                }
                return nodeInput.isIntercept;
            }
        }
    }
    else if (action == 99) // 移动
    {
        NodeInputStruct &nodeInput = this->_nodeInputMap[node2d->getUuid()];
        if (nodeInput.status == 1) // touch move事件触发
        {
            nodeInput.touchResult.worldX = this->_cursorX;
            nodeInput.touchResult.worldY = this->_cursorY;
            std::map<int, std::function<void(NodeInputResult &)>> &touchMoves = this->_nodeInputMap[node2d->getUuid()].touchMoves;
            for (auto it = touchMoves.begin(); it != touchMoves.end(); ++it)
            {
                if (it->second)
                {
                    it->second(this->_nodeInputMap[node2d->getUuid()].touchResult);
                }
            }
            return nodeInput.isIntercept;
        }
        else if (nodeInput.status == 0) // 不在节点范围内，触发touch cancel事件
        {
            bool isIn = node2d->inHitOnNode(this->_cursorX, this->_cursorY);
            if (!isIn)
            {
                return false;
            }
            nodeInput.touchResult.worldX = this->_cursorX;
            nodeInput.touchResult.worldY = this->_cursorY;
            std::map<int, std::function<void(NodeInputResult &)>> &touchHover = this->_nodeInputMap[node2d->getUuid()].cursorHovers;
            for (auto it = touchHover.begin(); it != touchHover.end(); ++it)
            {
                if (it->second)
                {
                    it->second(this->_nodeInputMap[node2d->getUuid()].touchResult);
                }
            }
            return true;
        }
    }
    else
    {
    }
    return false;
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