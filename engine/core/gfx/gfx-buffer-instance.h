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

struct GfxBufferInstancePool
{
    std::vector<GfxBuffer *> buffers;
};


class GfxBufferInstance
{
private:
    std::map<int, GfxBufferInstancePool *> pools;

public:
    GfxBufferInstance();
     GfxBuffer *getBuffer(int size);
     void clear();
    ~GfxBufferInstance();
};