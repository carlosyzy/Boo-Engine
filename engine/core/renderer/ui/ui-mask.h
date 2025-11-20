#pragma once
#include <string>
#include "../../component/component.h"
#include "../../scene/node.h"
class Texture;
class UIMask : public Component
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

    std::string _addUuid;
    std::string _subUuid;

public:
    UIMask(Node *node, std::string uuid = "");
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
    void lateRender() override;
    void destroy() override;
    ~UIMask() override;
};