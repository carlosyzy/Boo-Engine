#pragma once
#include "../component.h"
class Node;
class Node2D;

struct UILayoutHorizontalConfig
{
    float width; //比例,宽度占比
    Node2D *node;
};

class UILayoutHorizontal : public Component
{
private:
    int _flag = 0;
    /**
     * 水平子节点数量
     */
    int _size;
    // // FILL 目前只使用单一模式:填充
   /* std::vector<UILayoutHorizontalConfig> _configs;*/

    int _nodeTransformEventId = 0;
    void _onNodeTransformEvent(uint32_t eventId);

    void _clear();

public:
    UILayoutHorizontal(Node *node, std::string uuid = "");
    void setSize(int size);
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
     void lateRender() override;
    void destroy() override;
    ~UILayoutHorizontal();
};
