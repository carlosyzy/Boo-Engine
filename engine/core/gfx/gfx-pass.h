#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include "gfx-struct.h"

class GfxContext;

/**
 * pass 支持
 * 1.非多重采样和多重采样模式,
 * 	   附件描述数量的区别,多重采样多了一个 解析附件
 * 2.离屏和非离屏模式     
 * 	   参数区别
 */

// /**
//  * @brief UI Pass 颜色附件
//  */
// struct PassAttachmentColor
// {
// 	/**
// 	 * 定义多采样抗锯齿（MSAA）的采样数。
// 	 * VK_SAMPLE_COUNT_1_BIT：禁用 MSAA。
// 	 * VK_SAMPLE_COUNT_4_BIT：4x MSAA。
// 	 * VK_SAMPLE_COUNT_8_BIT：8x MSAA。
// 	 * VK_SAMPLE_COUNT_16_BIT：16x MSAA。
// 	 * VK_SAMPLE_COUNT_32_BIT：32x MSAA。
// 	 * 关联性：
// 	 * 如果启用 MSAA，需要额外的解析附件（Resolve Attachment）来存储最终结果。
// 	 * 必须与渲染流程中的其他附件（如深度附件）的采样数一致。
// 	 */
// 	VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
// 	/**
// 	 * 指定渲染流程开始时如何处理附件的现有内容。
// 	 * VK_ATTACHMENT_LOAD_OP_LOAD：保留附件原有内容（适合连续渲染或多帧叠加）。
// 	 * VK_ATTACHMENT_LOAD_OP_CLEAR：清除为指定值（适合每帧重新绘制）。
// 	 * VK_ATTACHMENT_LOAD_OP_DONT_CARE：不加载附件的内容，可能会导致性能问题。不关心原有内容（性能优化，忽略内容）。
// 	 * 如果设为 LOAD，需要确保附件的 initialLayout 与之前渲染流程的 finalLayout 匹配。
// 	 */
// 	VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
// 	/**
// 	 * 指定渲染流程结束时如何处理附件的内容。
// 	 * VK_ATTACHMENT_STORE_OP_STORE：将附件内容存储到内存中（后续渲染或呈现需要）。
// 	 * VK_ATTACHMENT_STORE_OP_DONT_CARE：VK_ATTACHMENT_STORE_OP_DONT_CARE：不保存（适合临时附件，如深度缓冲）。
// 	 * 关联性：
// 	 * VK_ATTACHMENT_STORE_OP_DONT_CARE：不保存（适合临时附件，如深度缓冲）。
// 	 */
// 	VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
// 	/**
// 	 * 作用：控制模板缓冲的加载和存储行为（仅对含模板的格式有效，如 VK_FORMAT_D24_UNORM_S8_UINT）。
// 	 * 当前配置：
// 	 * 由于这是一个纯颜色附件（无模板分量），设为 DONT_CARE 是合理的。
// 	 * 关联性：
// 	 * 如果格式包含模板（如深度/模板附件），需根据需求选择 LOAD 或 CLEAR。
// 	 */
// 	VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
// 	VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// 	/**
// 	 * 作用：指定附件在渲染流程开始前的初始布局。
// 	 * COLOR_ATTACHMENT_OPTIMAL：附件作为颜色输出使用。
// 	 * UNDEFINED：不关心初始内容（通常与 LOAD_OP_CLEAR 搭配）。
// 	 * 关联性：
// 	 * 如果 loadOp 是 LOAD，布局必须与之前流程的 finalLayout 一致。
// 	 * 如果 loadOp 是 CLEAR 或 DONT_CARE，可以用 UNDEFINED 优化性能
// 	 */
// 	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
// 	/**
// 	 * 作用：指定附件在渲染流程结束后的最终布局。
// 	 * PRESENT_SRC_KHR：附件将用于呈现到屏幕（交换链图像必须转换到此布局）。
// 	 * COLOR_ATTACHMENT_OPTIMAL：附件继续用于后续渲染。
// 	 * 关联性：
// 	 * 必须与交换链图像的最终用途匹配，否则呈现会失败。
// 	 */
// 	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
// };
/* // 关键关联性总结
// 1.loadOp 与 initialLayout
// - LOAD + COLOR_ATTACHMENT_OPTIMAL：保留上一帧结果继续渲染（如渐进式渲染）。
// - CLEAR + UNDEFINED：每帧清空附件（常规渲染）。
// 2.storeOp 与 finalLayout
// - 交换链附件必须使用 STORE 和 PRESENT_SRC_KHR，否则内容无法显示。
// 3.多采样与格式兼容性
// - 如果启用 MSAA（samples > 1），需额外配置解析附件（pResolveAttachments）。
 */
/**
 * 附件
 * 颜色附件  单通道 ui
 * 		format:VK_FORMAT_R8_UNORM
 * 		samples:VK_SAMPLE_COUNT_1_BIT或者msaa开启状态下读取指定的samples数
 * 		loadOp:VK_ATTACHMENT_LOAD_OP_LOAD或者VK_ATTACHMENT_LOAD_OP_CLEAR
 * 		storeOp:VK_ATTACHMENT_STORE_OP_STORE
 * 		stencilLoadOp:VK_ATTACHMENT_LOAD_OP_DONT_CARE
 * 		stencilStoreOp:VK_ATTACHMENT_STORE_OP_DONT_CARE
 * 		initialLayout:VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
 * 		finalLayout:VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
 * 解析附件  msaa 解析附件（Resolve Attachment）用于存储 MSAA 渲染结果的最终颜色值。
 * 		format:VK_FORMAT_R8_UNORM  解析目标必须是单采样
 * 		samples:VK_SAMPLE_COUNT_1_BIT
 * 		loadOp:VK_ATTACHMENT_LOAD_OP_DONT_CARE
 * 		storeOp:VK_ATTACHMENT_STORE_OP_STORE  必须存储，因为要显示
 * 		stencilLoadOp:VK_ATTACHMENT_LOAD_OP_DONT_CARE
 * 		stencilStoreOp:VK_ATTACHMENT_STORE_OP_DONT_CARE
 * 		initialLayout:VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
 * 		finalLayout:VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
 * 深度附件  多重采样深度附件,深度Stencil附件,
 *		多重采样
 *			format:VK_FORMAT_D32_SFLOAT_S8_UINT
 *			samples:VK_SAMPLE_COUNT_1_BIT或者msaa开启状态下读取指定的samples数
 *  		loadOp:VK_ATTACHMENT_LOAD_OP_CLEAR
 * 		    storeOp: VK_ATTACHMENT_STORE_OP_DONT_CARE; 渲染完成后通常不需要深度数据
 * 		    stencilLoadOp:VK_ATTACHMENT_LOAD_OP_DONT_CARE
 * 		    stencilStoreOp:VK_ATTACHMENT_STORE_OP_DONT_CARE
 * 		    initialLayout:VK_IMAGE_LAYOUT_UNDEFINED
 * 		    finalLayout:VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
 *      Stencil附件  深度Stencil附件,
 * 			format:VK_FORMAT_D24_UNORM_S8_UINT
 * 			samples:VK_SAMPLE_COUNT_1_BIT
 *  		loadOp:VK_ATTACHMENT_LOAD_OP_CLEAR
 * 		    storeOp: VK_ATTACHMENT_STORE_OP_DONT_CARE; 渲染完成后通常不需要Stencil数据
 * 		    stencilLoadOp:VK_ATTACHMENT_LOAD_OP_CLEAR;   清除Stencil为0
 * 		    stencilStoreOp:VK_ATTACHMENT_STORE_OP_STORE; 必须存储Stencil！
 * 		    initialLayout:VK_IMAGE_LAYOUT_UNDEFINED
 * 		    finalLayout:VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
 * 
 * 
 */



class GfxPass
{
private:
    std::string _name;
    GfxContext *_context;
    VkRenderPass _vkRenderPass = VK_NULL_HANDLE;

    void _createVkRenderPass();
    void _Log(std::string msg);

public:
    GfxPass(std::string name, GfxContext *context);
    void create();
    void clear();
    void reset();

    std::string name()
    {
        return this->_name;
    }
    VkRenderPass getVkRenderPass()
    {
        return this->_vkRenderPass;
    }

    ~GfxPass();
};



