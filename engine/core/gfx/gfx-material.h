#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include "gfx-struct.h"
#include "gfx-pipeline-struct.h"
#include "gfx-push-constants.h"

class GfxMaterial
{
private:
    /**
     * @brief 渲染通道
     * 最重要的
     */
    std::string _pass;
    /**
     * @brief 渲染管线结构体
     */
    GfxPipelineStruct _pipelineStruct;

    /**
     * @brief 材质使用的纹理
     */
    std::vector<std::string> _textures;
    /**
     * @brief 推送常量
     */
    GfxPushConstants _pushConstants;

public:
    GfxMaterial();
    void setRenderPass(const std::string pass);
    void setPipelineStruct(const GfxPipelineStruct pipelineStruct);
    void setTextures(const std::vector<std::string> textures);
    void setPushConstants(const GfxPushConstants pushConstants);

    /**
     * @brief 比较材质是否相等
     *
     * @param material
     * @return true
     * @return false
     */
    bool equals(const GfxMaterial *material) const;

    const GfxPipelineStruct &getPipelineStruct() const;
    const GfxPushConstants &getPushConstants() const;
    const std::vector<std::string> &getTextures() const;

    ~GfxMaterial();
};