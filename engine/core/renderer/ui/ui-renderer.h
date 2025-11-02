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

enum class UIFlag : uint32_t
{
    UI_NONE = 0,
    UI_TRANSFORM = 1 << 0, /** @brief 变换矩阵 */
    UI_MATERIAL = 1 << 1,  /** @brief 材质 */
    UI_COLOR = 1 << 2,     /** @brief 颜色 */
    UI_TEXTURE = 1 << 3,   /** @brief 纹理 */
    UI_ALL = 0xFF,
};

class UIRenderer : public Component
{
private:
    std::vector<float> _positions = {
        -0.5f, 0.5f, 0.0f,  /** @brief 左上 */
        -0.5f, -0.5f, 0.0f, /** @brief 坐下 */
        0.5f, -0.5f, 0.0f,  /** @brief 右下 */
        0.5f, 0.5f, 0.0f    /** @brief 右上 */
    };
    std::vector<float> _colors = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f};
    std::vector<float> _normals = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f};
    std::vector<float> _uvs = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f};
    std::vector<uint32_t> _indices = {
        0, 1, 2,
        0, 2, 3};

protected:
    std::string _shaderVert = "resources/effects/ui/ui.vert";
    std::string _shaderFrag = "resources/effects/ui/ui.frag";
    std::string _texture = "resources/textures/ic-default.png";
    std::string _pipelineState;
    Color _color;
    uint32_t _flag;

    Mat4 _modelMatrix;
    Mat4 _projMatrix;
    void _init();
    void _initDefaultGfxPipeline();
    void _initDefaultGfxTexture();
    void _initDefaultGfxRenderObject();
    void _createGfxTexture();

public:
    UIRenderer(Node *node, std::string uuid = "");

    /**
     * @brief 设置渲染器的颜色
     *
     * @param color
     */
    void setColor(float r, float g, float b, float a);
    void setColor(std::string color);
    void setTexture(std::string texture);
    void setShader(std::string vert, std::string frag);
    void setModelMat4();
    void update(float deltaTime) override;

    /*void updateViewMatrix();
    void updateProjMatrix();*/

    const Color &color() { return this->_color; };

    virtual ~UIRenderer() override;
};
