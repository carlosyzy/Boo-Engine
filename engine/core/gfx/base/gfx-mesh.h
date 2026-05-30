#pragma once
// #include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include <cmath>
#include "../gfx-struct.h"
#include "../../../log.h"

class GfxMesh
{
private:
    std::string _uuid;
    /**
     * 网格类型
     * 0:ui网格
     * 1:3d网格
     */
    int _meshMode;
    /**
     * @brief 网格类型
     * 0:静态网格
     * 1:动态网格
     */
    int _meshType;
    std::vector<float> _vertices;
    std::vector<uint32_t> _indices;
    /**
     * @brief 顶点哈希值
     */
    size_t _vertexHash = 0;
    /**
     * @brief 索引哈希值
     */
    size_t _indexHash = 0;

    bool _isVisible = true;
    uint32_t _indexSize = 0;

    VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;
    void _createStaticBuffers();
    void _createDynamicBuffers();
    void _clearBuffers();
    void _calculateHash();

public:
    // 后续可以加入网格哈希
    GfxMesh(std::string uuid);
    /**
     * @brief 创建ui网格
     * @param meshMode 网格模式 0:静态网格 1:动态网格
     * @param vertices 顶点数据
     * @param indices 索引数据
     */
    void createUIMesh(int meshMode, const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
    /**
     * @brief 创建3d网格
     * @param meshMode 网格模式 0:静态网格 1:动态网格
     * @param vertices 顶点数据
     * @param indices 索引数据
     */
    void createMesh(int meshMode, const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
    /**
     * @brief 更新网格数据
     * @param vertices 顶点数据
     * @param indices 索引数据
     */
    void update(const std::vector<float> &vertices, const std::vector<uint32_t> &indices);

    // void createUIMesh(const std::vector<float> &_positions, const std::vector<float> &_uvs, const std::vector<int> &_indices);
    // void createMesh(const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices);
    
    // void update(const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices);
    VkBuffer getVertexBuffer();
    VkBuffer getIndexBuffer();
    const std::string &getUuid();
    const uint32_t getIndexCount();
    bool isVisible();
    bool equals(const GfxMesh *mesh) const;
    void destroy();
};