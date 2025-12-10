// #pragma once
// #include <vulkan/vulkan_core.h>
// #include <iostream>
// #include <algorithm>
// #include <fstream>
// #include <vector>
// #include <set>
// #include <map>
// #include <cstdint>

// class GfxBufferSSBO
// {
// private:
//     std::vector<VkBuffer> _buffers;
//     std::vector<VkDeviceMemory> _buffersMemory;
//     std::vector<void *> _buffersMapped;

//     // // 现有的成员变量...
//     // std::vector<VkBuffer> _storageBuffers;
//     // std::vector<VkDeviceMemory> _storageBuffersMemory;
//     // std::vector<void *> _storageBuffersMapped;
//     // void _createStorageBuffers();
//     uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//     /**
//      * @brief 缓冲区是否被占用
//      */
//     bool _isOccupied = false;

// public:
//     GfxBufferSSBO();
//     void create(VkDeviceSize bufferSize);
//     ~GfxBufferSSBO();
// };