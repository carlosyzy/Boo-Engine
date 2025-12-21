#include "input.h"
#include "../../boo.h"
#include "../game.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"
#include "../math/mat4.h"
#include "../math/vec3.h"

Input::Input()
{
}
void Input::init()
{
}
void Input::setRoot2D(Node2D *root)
{
    this->_root = root;
}
/**
 * @brief 鼠标按钮事件
 * @param button 鼠标按钮 0: 左键 1: 右键 2: 中键
 * @param action 事件动作
 * @param mods 按键修饰符
 */
void Input::onMouseButton(int button, int action, int mods)
{
    std::cout << "Input::onMouseButton  button: " << button << " action: " << action << " mods: " << mods << std::endl;
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
    //  View *view = Boo::game->view();
    // std::cout << "Input::_propagateEvent: x: " << this->_cursorX << " y: " << this->_cursorY << " is pressed" << std::endl;
    // std::cout << "Input::View: width: " << view->width << " height: " << view->height << " is pressed" << std::endl;
    // std::cout << "Input::_propagateEvent: x: " << this->_cursorX << " y: " << this->_cursorY << " is pressed" << std::endl;
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

    // this->_cursorX = xpos;
    // this->_cursorY = ypos;

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
/**
 * @brief 键盘事件
 * @param key 键盘键值
 * @param scancode 键盘扫描码
 * @param action 键盘操作 0:抬起 1:按下
 * @param mods 键盘修饰键 0:无 1:Shift 2:Ctrl 3:Alt
 */
void Input::onKey(int key, int scancode, int action, int mods)
{
    std::cout << "Input::onKey  key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods << std::endl;
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
        View *view = Boo::game->view();
        bool isIn = node2d->inHitOnNode(this->_cursorX, this->_cursorY);
        if (isIn)
        {
            Vec3 worldPos(this->_cursorX, this->_cursorY, 1.0f);
            NodeInputStruct &nodeInput = this->_nodeInputMap[node2d->getUuid()];
            nodeInput.status = 1;
            nodeInput.touchResult.worldX = worldPos.getX();
            nodeInput.touchResult.worldY = worldPos.getY();
            nodeInput.touchResult.button = button;
            // 计算节点本地坐标
            const Mat4 &worldMat = node2d->getWorldMatrix();
            Mat4 localMat;
            Mat4::inverse(worldMat, localMat);
            Vec3 localPos;
            Mat4::multiplyVec3(localMat, worldPos, localPos);
            nodeInput.touchResult.localX = localPos.getX();
            nodeInput.touchResult.localY = localPos.getY();

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
        Vec3 worldPos(this->_cursorX, this->_cursorY, 1.0f);
        NodeInputStruct &nodeInput = this->_nodeInputMap[node2d->getUuid()];
        nodeInput.status = 0;
        nodeInput.touchResult.worldX = worldPos.getX();
        nodeInput.touchResult.worldY = worldPos.getY();
        nodeInput.touchResult.button = button;
        // 计算节点本地坐标
        const Mat4 &worldMat = node2d->getWorldMatrix();
        Mat4 localMat;
        Mat4::inverse(worldMat, localMat);
        Vec3 localPos;
        Mat4::multiplyVec3(localMat, worldPos, localPos);
        nodeInput.touchResult.localX = localPos.getX();
        nodeInput.touchResult.localY = localPos.getY();
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
        Vec3 worldPos(this->_cursorX, this->_cursorY, 1.0f);
        if (nodeInput.status == 1) // touch move事件触发
        {
            nodeInput.touchResult.worldX = worldPos.getX();
            nodeInput.touchResult.worldY = worldPos.getY();
            // 计算节点本地坐标
            const Mat4 &worldMat = node2d->getWorldMatrix();
            Mat4 localMat;
            Mat4::inverse(worldMat, localMat);
            Vec3 localPos;
            Mat4::multiplyVec3(localMat, worldPos, localPos);
            nodeInput.touchResult.localX = localPos.getX();
            nodeInput.touchResult.localY = localPos.getY();
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
            nodeInput.touchResult.worldX = worldPos.getX();
            nodeInput.touchResult.worldY = worldPos.getY();
             // 计算节点本地坐标
            const Mat4 &worldMat = node2d->getWorldMatrix();
            Mat4 localMat;
            Mat4::inverse(worldMat, localMat);
            Vec3 localPos;
            Mat4::multiplyVec3(localMat, worldPos, localPos);
            nodeInput.touchResult.localX = localPos.getX();
            nodeInput.touchResult.localY = localPos.getY();
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