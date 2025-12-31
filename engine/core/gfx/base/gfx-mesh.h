#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include "gfx-struct.h"

class GfxMesh
{
private:
    std::string _uuid;
    std::vector<float> _vertices;
    std::vector<uint32_t> _indices;

    VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;
    int _indexSize;
    void _createVertexBuffers();
    void _clearVertexBuffers();
public:
    // 后续可以加入网格哈希
    GfxMesh(std::string uuid);
    void setInputVertices(std::vector<float> vertices, std::vector<uint32_t> indices);
    VkBuffer getVertexBuffer();
    uint32_t getVertexCount();
    VkBuffer getIndexBuffer();
    uint32_t getIndexCount();
    int getIndexSize();
    bool equals(const GfxMesh *mesh) const;
};