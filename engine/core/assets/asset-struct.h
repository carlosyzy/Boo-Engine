#pragma once
#include <string>
#include <filesystem>

enum class AssetType
{
    None,
    Texture,
    Audio,
    Font,
    Shader,
    Material,
    Scene,
};

struct AssetDB
{
    std::string name;
    std::string path;
    std::string uuid;
    std::string extension;
    AssetType type;
};