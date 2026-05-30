#include "gfx-builtin-batch-3d.h"
#include "../../gfx.h"
#include "../../gfx-context.h"
#include "../../gfx-renderer.h"
#include "../../base/gfx-mesh.h"
#include "../../base/gfx-material.h"
#include "../../base/gfx-texture.h"
#include "../../base/gfx-render-texture.h"
#include "../../base/gfx-buffer.h"
#include "../../base/gfx-buffer-ubo.h"
#include "../../base/gfx-buffer-instance.h"
#include "../gfx-builtin-renderer.h"
#include "../gfx-builtin-pipeline/gfx-builtin-pipeline.h"

GfxBuiltinBatch3D::GfxBuiltinBatch3D()
    : GfxBuiltinBatch()
{
    this->_disTransparent = 0.0f;
}
void GfxBuiltinBatch3D::addObject(const std::vector<char> &instanceData)
{
    if (instanceData.size() != 128)
    {
        LOGE("[Gfx : BatchBuiltin3D] :: addObject: instanceData size must be 32! Current size: %d", (int)instanceData.size());
        return;
    }
    GfxBuiltinBatch::addObject(instanceData);
    const GfxRendererState &pipelineState = this->_material->getRendererState();
    if (pipelineState.type == GfxRendererType::_Transparent)
    {
        // 从材质中获取世界矩阵，提取世界坐标，计算与相机的距离
        const std::array<float, 16> &worldMatrix = this->_material->getInstanceWorldMatrixData();
        float x = worldMatrix[12] - this->_cameraPos[0];
        float y = worldMatrix[13] - this->_cameraPos[1];
        float z = worldMatrix[14] - this->_cameraPos[2];
        this->_disTransparent = sqrtf(x * x + y * y + z * z);
    }
}
void GfxBuiltinBatch3D::render(VkCommandBuffer &queueCommandBuffer)
{
    const GfxRendererState &pipelineState = this->_material->getRendererState();
    GfxBuiltinPipeline *pipeline = this->_renderer->getPipeline(pipelineState);
    if (pipeline == nullptr)
    {
        LOGE("[Gfx : BatchBuiltin] :: render: pipeline not found!");
        return;
    }
    this->_bindUniformBuffer();
    this->_bindPipeline(queueCommandBuffer, pipeline);
    this->_setViewportScissor(queueCommandBuffer);
    this->_bindDescriptorSets(queueCommandBuffer, pipeline, this->_ubo);
    this->_bindVertexIndicesBuffers(queueCommandBuffer);
    this->_drawIndexed(queueCommandBuffer);
}
void GfxBuiltinBatch3D::_bindUniformBuffer()
{
    this->_ubo = Gfx::_bufferUBO->getBuffer((16 + 16 + 1 + 4 + 4 + 4) * sizeof(float)); // 视图矩阵+投影矩阵+时间+相机位置+主光方向+主光颜色
    this->_ubo->setIsOccupied(true);
    // 提交视图矩阵和投影矩阵
    memcpy(this->_ubo->getMappedData(), this->_viewMatrix.data(), sizeof(float) * 16);
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 16, this->_projMatrix.data(), sizeof(float) * 16);
    // 提交相机位置
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 32, this->_cameraPos.data(), sizeof(float) * 4);
    // 提交主光方向
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 36, Gfx::_mainLitDir.data(), sizeof(float) * 4);
    // 提交主光颜色
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 40, Gfx::_mainLitColor.data(), sizeof(float) * 4);
    // 提交时间
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 44, &Gfx::_frameTime, sizeof(float));
}

void GfxBuiltinBatch3D::_setViewportScissor(VkCommandBuffer &queueCommandBuffer)
{
    GfxBuiltinBatch::_setViewportScissor(queueCommandBuffer);
}

void GfxBuiltinBatch3D::_bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo)
{
    VkDescriptorSet descriptor = this->_renderer->get3DDescriptorSet();
    std::array<VkWriteDescriptorSet, 11> descriptorWrites{};
    // 绑定ubo
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = ubo->getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(float) * (16 + 16 + 4 + 4 + 4 + 1); // 视图矩阵+投影矩阵+相机位置+主光方向+主光颜色+全局时间
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptor;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    // 绑定采样器
    std::array<VkDescriptorImageInfo, 10> imageInfos;
    GfxTexture *defaultTexture = Gfx::_textures.at("builtin::default.png");

    for (size_t i = 0; i < 10; i++)
    {
        imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        if (i >= this->_material->getTextures().size())
        {
            imageInfos[i].imageView = defaultTexture->getImageView();
            imageInfos[i].sampler = defaultTexture->getSampler();
        }
        else
        {
            if (Gfx::_textures.find(this->_material->getTextures()[i]) != Gfx::_textures.end())
            {
                GfxTexture *texture = Gfx::_textures.at(this->_material->getTextures()[i]);
                imageInfos[i].imageView = texture->getImageView();
                imageInfos[i].sampler = texture->getSampler();
            }
            else
            {
                imageInfos[i].imageView = defaultTexture->getImageView();
                imageInfos[i].sampler = defaultTexture->getSampler();
            }
        }
        descriptorWrites[i + 1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[i + 1].dstSet = descriptor;
        descriptorWrites[i + 1].dstBinding = static_cast<uint32_t>(i + 1);
        descriptorWrites[i + 1].dstArrayElement = 0;
        descriptorWrites[i + 1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[i + 1].descriptorCount = 1;
        descriptorWrites[i + 1].pImageInfo = &imageInfos[i];
    }
    vkUpdateDescriptorSets(Gfx::_context->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    vkCmdBindDescriptorSets(queueCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipelineLayout(), 0, 1, &descriptor, 0, nullptr);
}
void GfxBuiltinBatch3D::_drawIndexed(VkCommandBuffer &queueCommandBuffer)
{
    GfxBuiltinBatch::_drawIndexed(queueCommandBuffer);
}

void GfxBuiltinBatch3D::_bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer)
{
    GfxBuiltinBatch::_bindVertexIndicesBuffers(queueCommandBuffer);
}
float GfxBuiltinBatch3D::getDisTransparent() const
{
    return this->_disTransparent;
}



GfxBuiltinBatch3D::~GfxBuiltinBatch3D()
{
}