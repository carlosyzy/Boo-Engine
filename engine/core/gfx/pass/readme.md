
#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "gfx-struct.h"

enum class GfxPassAttachmentLoadOp
{
	/**
	 * 保留附件原有内容（适合连续渲染或多帧叠加）。
	 */
	Load = 0,
	/**
	 * 清除为指定值（适合每帧重新绘制）。
	 */
	Clear = 1,
	/**
	 * 不关心原有内容（性能优化，忽略内容）。
	 */
	DontCare = 2,
};
/**
 * 指定渲染流程结束时如何处理附件的内容
 */
enum class GfxPassAttachmentStoreOp
{
	/**
	 * 将附件内容存储到内存中（后续渲染或呈现需要）。
	 */
	Store = 0,
	/**
	 * 不保存（适合临时附件，如深度缓冲）。
	 */
	DontCare = 1,
};

// 交换链图像（直接渲染到屏幕）
// colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  // 开始前：内容不重要
// colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // 结束后：准备显示
// 离屏渲染目标（后续作为纹理使用）
// colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  // 开始前：内容不重要
// colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // 结束后：作为纹理使用
// 深度模板附件
// depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  // 开始前：内容不重要
// depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // 结束后：保存深度模板信息
// 颜色附件和解析附件都属于颜色附件,解析附件是颜色附件的一种特殊情况,

enum class GfxPassAttachmentLayout
{
	/**
	 * 图像内容不重要，可以被丢弃,RenderPass 开始时需要清除的附件,最佳性能，驱动不需要保存原有内容
	 */
	Undefined = 0,
	/**
	 * 图像用作颜色附件，适合片段着色器写入,颜色缓冲区
	 */
	Color = 1,
	/**
	 * 图像用作深度附件，适合深度测试写入,深度缓冲区
	 */
	Depth = 2,
	/**
	 * 图像适合在着色器中采样,模版缓冲区
	 */
	Shader = 3,
	/**
	 * 图像准备显示到屏幕
	 */
	Present = 4,
};
/**
 * @brief 渲染 pass 附件描述
 */
struct GfxPassAttachment
{
	/**
	 * 是否启用附件
	 */
	bool enable = false;
	/**
	 * 0: 附件格式 表示和 swapchain 一致
	 * 1: VK_FORMAT_D32_SFLOAT   32位深度 无模版
	 * 2: VK_FORMAT_D24_UNORM_S8_UINT	 24位深度,8位模版
	 * 3: VK_FORMAT_D32_SFLOAT_S8_UINT  32位深度,8位模版
	 */
	VkFormat format = VK_FORMAT_UNDEFINED;
	VkSampleCountFlagBits samples;
	GfxPassAttachmentLoadOp loadOp = GfxPassAttachmentLoadOp::Load;
	GfxPassAttachmentStoreOp storeOp = GfxPassAttachmentStoreOp::Store;
	GfxPassAttachmentLoadOp stencilLoadOp = GfxPassAttachmentLoadOp::DontCare;
	GfxPassAttachmentStoreOp stencilStoreOp = GfxPassAttachmentStoreOp::DontCare;
	GfxPassAttachmentLayout initialLayout = GfxPassAttachmentLayout::Undefined;
	GfxPassAttachmentLayout finalLayout = GfxPassAttachmentLayout::Undefined;
	GfxPassAttachmentLayout refLayout = GfxPassAttachmentLayout::Undefined;
	uint32_t attachment = 0;
	// VkClearColorValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
	// VkClearDepthStencilValue clearDepthStencil = {1.0f, 0};
};
struct GfxPassStruct
{
	/**
	 * 附件数量
	 */
	uint32_t attachmentCount = 0;
	/**
	 * 颜色附件
	 */
	GfxPassAttachment colorAttachment;
	/**
	 * 解析附件
	 */
	GfxPassAttachment resolveAttachment;
	/**
	 * 深度附件
	 */
	GfxPassAttachment depthAttachment;
};