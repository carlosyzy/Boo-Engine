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
    Scene,
};
class Asset
{
private:
protected:
    /**
     * @brief 资源类型
     */
    AssetType _type;
    /**
     * @brief 资源键值
     */
    std::string _uuid;
    /**
     * @brief 资源路径
     * 基于环境的绝对路径
     */
    std::string _path;
    virtual void _load() = 0;
public:
    Asset(std::string uuid);
    Asset(std::string uuid,std::string path);
    const std::string& getUuid() {
        return this->_uuid;
    }
    virtual void destroy();
    ~Asset();
};
