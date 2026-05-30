#include "asset.h"
#include "boo.h"
#include "core/util/uuid-util.h"
namespace Boo
{

    Asset::Asset() : _type(EAssetType::None),
                     _uuid(UuidUtil::generateUUID()),
                     _name(""),
                     _path("")
    {
    }
    Asset::Asset(std::string uuid) : _type(EAssetType::None),
                                     _uuid(uuid),
                                     _name(""),
                                     _path("")
    {
    }
    Asset::Asset(std::string uuid, std::string path, std::string name) : _type(EAssetType::None),
                                                                         _uuid(uuid),
                                                                         _name(name),
                                                                         _path(path)
    {
    }
    const std::string &Asset::getName()
    {
        return this->_name;
    }
    const std::string &Asset::getUuid()
    {
        return this->_uuid;
    }
    void Asset::setIsPerpetual(bool isPerpetual)
    {
        this->_isPerpetual = isPerpetual;
    }
    const bool &Asset::getIsPerpetual()
    {
        return this->_isPerpetual;
    }

    void Asset::destroy()
    {
    }
    Asset::~Asset()
    {
    }

} // namespace Boo
