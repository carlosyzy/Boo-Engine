#pragma once
#include <string>
class Asset;
class Shader;
class AssetLoad
{
public:
    static Asset *load(const std::string path, const std::string fullPath);
};