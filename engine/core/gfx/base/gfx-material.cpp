#include "gfx-material.h"

GfxMaterial::GfxMaterial()
{
}
void GfxMaterial::setPipelineStruct(const GfxPipelineStruct pipelineStruct)
{
    this->_pipelineStruct = pipelineStruct;
    
}
void GfxMaterial::setTextures(const std::vector<std::string> textures)
{
    this->_textures = textures;
}
void GfxMaterial::setPushConstants(const GfxPushConstants pushConstants)
{
    this->_pushConstants = pushConstants;
}
/**
 * @brief 比较材质是否相等
 *
 * @param material
 * @return true
 * @return false
 */
bool GfxMaterial::equals(const GfxMaterial *material) const
{
    if (this->_pipelineStruct.generateKey() != material->_pipelineStruct.generateKey())
    {
        return false;
    }
    if (this->_pushConstants.getKey() != material->_pushConstants.getKey())
    {
        return false;
    }
    if (this->_textures.size() != material->_textures.size())
    {
        return false;
    }
    for (int i = 0; i < this->_textures.size(); i++)
    {
        if (this->_textures[i] != material->_textures[i])
        {
            return false;
        }
    }
    return true;
}
const GfxPipelineStruct &GfxMaterial::getPipelineStruct() const
{
    return this->_pipelineStruct;
}
const GfxPushConstants &GfxMaterial::getPushConstants() const
{
    return this->_pushConstants;
}
const std::vector<std::string> &GfxMaterial::getTextures() const
{
    return this->_textures;
}

GfxMaterial::~GfxMaterial()
{
}