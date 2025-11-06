#include "asset-load.h"
#include "asset.h"
#include "texture.h"
#include <filesystem>
#include <iostream>
#include "shader.h"

Asset *AssetLoad::load(std::string path, std::string fullPath)
{
    if (!std::filesystem::exists(fullPath))
    {
        std::cerr << "AssetLoad:资产不存在:" << fullPath << std::endl;
        return nullptr;
    }
    if (!std::filesystem::is_regular_file(fullPath))
    {
        std::cerr << "AssetLoad:资产不是文件:" << fullPath << std::endl;
        return nullptr;
    }
    std::string extension = std::filesystem::path(fullPath).extension().string();
    if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG")
    {
        Texture *texture = new Texture(path, fullPath);
        return texture;
    }else if (extension == ".vert" || extension == ".frag")
    {
        Shader *shader = new Shader(path, fullPath);
        return shader;
    }
    //     else if (extension == ".vert")
    //     {
    //         // uiTreeData.icon = "resources/textures/ic-shader-v.png";
    //     }
    //     else if (extension == ".frag")
    //     {
    //         // uiTreeData.icon = "resources/textures/ic-shader-f.png";
    //     }
    //     else if (extension == ".mtl")
    //     {
    //         // uiTreeData.icon = "resources/textures/ic-material.png";
    //     }
    //     else if (extension == ".md")
    //     {
    //         // uiTreeData.icon = "resources/textures/ic-readme.png";
    //     }
    //     else
    //     {
    //         // uiTreeData.icon = "resources/textures/ic-2d.png";
    //     }
    return nullptr;
}