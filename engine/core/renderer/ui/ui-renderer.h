#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <array>
#include <cstdint>

#include "../../math/mat4.h"
#include "../../math/vec3.h"
#include "../../math/vec2.h"
#include "../../math/quat.h"
#include "../../math/color.h"
#include "../../math/size.h"
#include "../../component/component.h"

class Node;
class MaterialAsset;
class TextureAsset;
class Camera;

class UIRenderer : public Component
{
private:
protected:
    MaterialAsset *_materialAsset;
    std::vector<float> _instanceData;
    Color _color;
    /**
     * @brief 反序列化组件属性-配置
     * 反序列化成功
     */
    void _deserialized() override;
    /**
     * @brief 设置渲染器的纹理
     *
     */
    void _setTexture(TextureAsset *texture);
    /**
     * @brief 设置渲染器的材质
     *
     */
    void _setMaterial(MaterialAsset *mtl);
    /**
     * @brief 设置渲染器的颜色
     *
     */
    void _setColor(float r, float g, float b, float a);
    // /**
    //  * 更新渲染状态
    //  */
    // virtual void _updateRendererState();
    // /**
    //  * 更新模型矩阵
    //  */
    // virtual void _updateModelMatrix();

public:
    UIRenderer(std::string name, Node *node, std::string uuid = "");

    void Awake() override;
    const int getVisibility();
    const Color &getColor() { return this->_color; };
    void Enable() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    /**
     * @brief 组件渲染函数
     * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
     */
    virtual void Render(Camera *camera);
    void Disable() override;
    void destroy() override;

    virtual ~UIRenderer() override;
};
