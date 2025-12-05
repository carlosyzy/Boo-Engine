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
    //后续可以加入网格哈希
};