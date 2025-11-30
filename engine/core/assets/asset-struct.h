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
    /**
     * @brief 纹理名称
     */
    std::string name = "";
    /**
     * @brief 纹理原始路径
     * 相对于assets 目录的路径
     */
    std::string path = "";
    /**
     * @brief 纹理UUID
     */
    std::string uuid = "";
    /**
     * @brief 纹理扩展名
     */
    std::string extension = "";
    /**
     * @brief 资产类型
     */
    AssetType type = AssetType::None;

    // 判断是否为空的成员函数
    bool isEmpty() const
    {
        return name.empty() &&
               path.empty() &&
               uuid.empty() &&
               extension.empty();
    }
};
