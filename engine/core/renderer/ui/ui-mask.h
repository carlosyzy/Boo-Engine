#pragma once
#include <string>
#include <iostream>
#include "ui-renderer.h"
#include "../../component/component-property-register.h"
#include "../../component/component-register.h"
#include "../../scene/node.h"
class TextureAsset;
class Camera;

/**
 * mask 组件继承与ui-renderer组件
 * 后续优化
 */

class UIMask : public UIRenderer
{
private:
    std::string _addUuid;
    std::string _subUuid;

protected:
    /**
     * @brief 反序列化组件属性-配置
     * 反序列化成功
     */
    void _deserialized() override;
    // /**
    //  * 更新渲染状态
    //  */
    // void _updateRendererState() override;
    // /**
    //  * 更新模型矩阵
    //  */
    // void _updateModelMatrix() override;

public:
    UIMask(std::string name, Node *node, std::string uuid = "");
    void Awake() override;
    void Enable() override;

    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render(Camera *camera) override;
    void lateRender();
    void Disable() override;
    void destroy() override;
    ~UIMask() override;
};
REGISTER_COMPONENT(UIMask, "UI Mask Component")
