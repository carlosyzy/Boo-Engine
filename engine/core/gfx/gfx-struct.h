#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>
#include <cstring>

struct GfxMaterial
{
    std::string name;
    std::string uuid;
    std::string pass;
    std::string pipeline;
    uint32_t ssboSize;
};
struct GfxMesh
{
    std::string name;
    std::string uuid;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    // 后续可以加入网格哈希
};

const GfxMesh defaultMesh = {
    .name = "defaultMesh",
    .uuid = "defaultMesh",
    .vertices = {
        -0.5f, 0.5f, 0.0f,  /** @brief 左上 */
        -0.5f, -0.5f, 0.0f, /** @brief 坐下 */
        0.5f, -0.5f, 0.0f,  /** @brief 右下 */
        0.5f, 0.5f, 0.0f    /** @brief 右上 */
    },
    .indices = {
        0, 1, 2,  /** @brief 左上-坐下-右下 */
        0, 2, 3   /** @brief 左上-右下-右上 */
    },
};
const GfxMaterial defaultMaterial = {
    .name = "defaultMaterial",
    .uuid = "defaultMaterial",
    .pass = "pass-built",
    .pipeline = "pipeline-built",
    .ssboSize = 0,
};
