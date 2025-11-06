#pragma once
#include <string>
enum class AssetType
{
    None,
    Texture,
    Audio,
    Font,
    Shader,
    Material,
};
class Asset
{
private:
protected:
    AssetType _type;
    std::string _key;
    std::string _path;
    virtual void _load() = 0;
public:
    Asset(const std::string key, const std::string path);
    virtual void destroy();
    ~Asset();
};
