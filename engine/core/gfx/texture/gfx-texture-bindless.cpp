#include "gfx-texture-bindless.h"
#include "gfx-texture.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-renderer.h"
#include "../gfx-struct.h"
#include "../gfx-descriptor.h"
GfxTextureBindless::GfxTextureBindless(GfxContext *context)
{
    this->_context = context;
}
void GfxTextureBindless::_addTexture(GfxTexture *texture)
{
    if (this->_nextIndex >= this->_maxTextures)
    {
        std::cout << "texture bindless pool is full!" << std::endl;
        return;
    }
    uint32_t index = _nextIndex++;
    // 更新描述符集
    VkDescriptorImageInfo imageInfo = {
        .sampler = texture->getSampler(),
        .imageView = texture->getImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    texture->setBindlessIndex(index);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkWriteDescriptorSet write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = Gfx::renderer->descriptor()->descriptorSets()[i],
            .dstBinding = 2,
            .dstArrayElement = index, // 在数组中的位置
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &imageInfo};

        vkUpdateDescriptorSets(Gfx::context->vkDevice(), 1, &write, 0, nullptr);
    }
}
GfxTextureBindless::~GfxTextureBindless()
{
}
