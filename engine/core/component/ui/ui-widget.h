#pragma once
#include "../component.h"
class Node;
class Node2D;

enum class WidgetHorizontal
{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    CENTER = 3,
    ALL = 9,

};
enum class WidgetVertical
{
    NONE = 0,
    TOP = 1,
    BOTTOM = 2,
    CENTER = 3,
    ALL = 9,
};
struct WidgetHorizontalParam
{
    float left;
    float right;
    float center;
};
struct WidgetVerticalParam
{
    float top;
    float bottom;
    float center;
};

class Node;

class UIWidget : public Component
{
private:
    int _flag = 0;

    WidgetHorizontal _horizontal = WidgetHorizontal::NONE;
    WidgetHorizontalParam _horizontalParam{};

    WidgetVertical _vertical = WidgetVertical::NONE;
    WidgetVerticalParam _verticalParam{};

    int _nodeTransform_ID = 0;
    void _onNodeTransformChange(int nodeTransform);
public:
    UIWidget(Node *node, std::string uuid = "");
    void Awake() override;
    void setHorizontal(WidgetHorizontal horizontal, WidgetHorizontalParam param);
    void setVertical(WidgetVertical vertical, WidgetVerticalParam param);
    virtual void Enable() override;
    void updateWidget();
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void LateRender() override;
    void Disable() override;
    void destroy() override;
    void clearGfxObject() override;
    ~UIWidget();
};
