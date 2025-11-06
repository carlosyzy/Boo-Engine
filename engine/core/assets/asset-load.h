#pragma once
#include <string>
class Asset;
class Shader;
class AssetLoad
{
public:
    static Asset *load(std::string path, std::string fullPath);
};