#pragma once
#include <string>
#include <filesystem>
#include "asset-type.h"
namespace Boo
{
    class Asset
    {
    private:
    protected:
        /**
         * @brief 资源类型
         */
        EAssetType _type;

        /**
         * @brief 资源键值
         */
        std::string _uuid;

        /**
         *
         */
        std::string _name;
        /**
         * @brief 基于资产的路径
         * assets/下的路径
         */
        std::string _path;
        /**
         * @brief 是否永久加载
         */
        bool _isPerpetual = false;
    public:
        Asset();
        Asset(std::string uuid);
        /**
         * @brief 构造函数
         * @param uuid 资产唯一标识
         */
        Asset(std::string uuid,std::string path,std::string name);
        /**
         * @brief 获取资源名
         */
        const std::string &getName();
        const std::string &getUuid();
        void setIsPerpetual(bool isPerpetual);
        const bool &getIsPerpetual();
        virtual void destroy();
        ~Asset();
    };

} // namespace Boo
