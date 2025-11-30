#pragma once
#include <string>
#include <filesystem>
#include "asset-struct.h"

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
     * 
     */
    std::string _name;
    /**
     * @brief 资源路径
     * 基于环境的绝对路径
     */
    std::string _path;
   
public:
    /**
     * @brief 构造函数
     * @param uuid 资产唯一标识  自动生成
     */
    Asset();
    /**
     * @brief 构造函数
     * @param uuid 资产唯一标识
     */
    Asset(std::string uuid);
    /**
     * @brief 创建资产
     * @param path 资产路径
     */
    virtual void create(std::string path);

    /**
     * @brief 获取资源名
     * 
     * @return const std::string& 
     */
    const std::string& getName() {
        return this->_name;
    }
    const std::string& getUuid() {
        return this->_uuid;
    }
    virtual void destroy();
    ~Asset();
};
