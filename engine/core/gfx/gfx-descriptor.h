// #pragma once
// #include <vulkan/vulkan_core.h>
// #include <vector>
// class GfxDescriptor
// {
// private:
//     VkDescriptorPool _descriptorPool;
//     /**
//      * @brief 描述符集布局
//      * 绑定ubo和采样器
//      */
//     VkDescriptorSetLayout _descriptorSetLayout;
// 	/**
// 	 * @brief 描述符集
// 	 * 描述符集
// 	 */
// 	std::vector<VkDescriptorSet> _descriptorSets;


//     void _createDescriptorPool();
//     void _createDescriptorSetLayout();
//     void _createDescriptorSets();
// public:
//     GfxDescriptor();

//     VkDescriptorPool descriptorPool() const
//     {
//         return this->_descriptorPool;
//     }
//     VkDescriptorSetLayout descriptorSetLayout() const
//     {
//         return this->_descriptorSetLayout;
//     }
//     std::vector<VkDescriptorSet> descriptorSets() const
//     {
//         return this->_descriptorSets;
//     }

//     ~GfxDescriptor();
// };
