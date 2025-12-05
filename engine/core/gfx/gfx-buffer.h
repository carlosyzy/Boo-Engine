#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>

class GfxBuffer
{
private:
    // 现有的成员变量...
    std::vector<VkBuffer> _uniformBuffers;
    std::vector<VkDeviceMemory> _uniformBuffersMemory;
    std::vector<void *> _uniformBuffersMapped;
    // 现有的方法...
    void _createUniformBuffers();
    uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    /**
     * @brief 缓冲区是否被占用
     */
    bool _isOccupied = false;
public:
    GfxBuffer();
    void create(VkDeviceSize bufferSize);
    ~GfxBuffer();
};