#pragma once
#include "../component.h"
class Node;
class Node2D;

struct UILayoutHorizontalConfig
{
    float width; // 比例,宽度占比
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
    void Awake() override;
    void setSize(int size);
    void Enable() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void LateRender() override;
    void Disable() override;
    void destroy() override;
    ~UILayoutHorizontal();
};
