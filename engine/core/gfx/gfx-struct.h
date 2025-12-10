#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

struct GfxMaterial
{
    std::string name;
    std::string uuid;
    std::string pass;
    std::string pipeline;
    std::vector<std::string> textures;
};
struct GfxMesh
{
    std::string name;
    std::string uuid;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    // 后续可以加入网格哈希
};

// GfxMesh defaultMesh = {
//     .name = "defaultMesh",
//     .uuid = "defaultMesh",
//     .vertices = {
//         -0.5f, 0.5f, 0.0f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0f, 0.0f,  /** @brief 左上 */
//         -0.5f, -0.5f, 0.0f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, /** @brief 左下 */
//         0.5f, -0.5f, 0.0f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0f, 1.0f,  /** @brief 右下 */
//         // 0.5f, 0.5f, 0.0f    /** @brief 右上 */  ,1.0f, 0.0f
//     },
//     .indices = {
//         0, 1, 2, /** @brief 左上-坐下-右下 */
//         // 0, 2, 3   /** @brief 左上-右下-右上 */
//     },
// };
// GfxMaterial defaultMaterial = {
//     .name = "",
//     .uuid = "",
//     .pass = "",
//     .pipeline = "",
//     .textures = {},
// };
