#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>

class GfxBufferUBO
{
private:
    // 现有的成员变量...
    VkBuffer _buffer;
    VkDeviceMemory _bufferMemory;
    void * _bufferMapped;
    uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    /**
     * @brief 缓冲区是否被占用
     */
    bool _isOccupied = false;

public:
    GfxBufferUBO();
    void create(VkDeviceSize bufferSize);
    void setIsOccupied(bool isOccupied);
    bool getIsOccupied() const;
    ~GfxBufferUBO();
};