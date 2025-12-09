#include "gfx-pipeline-ui.h"

GfxPipelineUI::GfxPipelineUI(const std::string &name) : GfxPipeline(name) {

}
void GfxPipelineUI::_initVertexInputState()
{
    // 初始化顶点位置输入属性描述
    this->_vInputAttribDescriptionPos.location = 0;
    this->_vInputAttribDescriptionPos.binding = 0;
    this->_vInputAttribDescriptionPos.format = VK_FORMAT_R32G32B32_SFLOAT;
    this->_vInputAttribDescriptionPos.offset = 0;
    // 初始化顶点纹理坐标输入属性描述
    this->_vInputAttribDescriptionTexCoord.location = 1;
    this->_vInputAttribDescriptionTexCoord.binding = 0;
    this->_vInputAttribDescriptionTexCoord.format = VK_FORMAT_R32G32_SFLOAT;
    this->_vInputAttribDescriptionTexCoord.offset = sizeof(float) * (3);

    this->_vertexInputAttributes.push_back(this->_vInputAttribDescriptionPos);
    this->_vertexInputAttributes.push_back(this->_vInputAttribDescriptionTexCoord);

    this->_vInputBindDescription.binding = 0;
    this->_vInputBindDescription.stride = sizeof(float) * (3 + 2);
    this->_vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    this->_vertexInputBindings.push_back(this->_vInputBindDescription);

    this->_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->_vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)this->_vertexInputBindings.size();
    this->_vertexInputInfo.pVertexBindingDescriptions = this->_vertexInputBindings.data();
    this->_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)this->_vertexInputAttributes.size();
    this->_vertexInputInfo.pVertexAttributeDescriptions = this->_vertexInputAttributes.data();
}