#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>

class GfxBuffer;

struct GfxBufferUBOPool
{
    std::vector<GfxBuffer *> buffers;
};

class GfxBufferUBO
{
private:
    std::map<int, GfxBufferUBOPool *> pools;
public:
    GfxBufferUBO();
    GfxBuffer *getBuffer(int size);
    void clear();
    ~GfxBufferUBO();
};