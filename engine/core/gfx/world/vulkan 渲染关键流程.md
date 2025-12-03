在Vulkan渲染过程中，drawcall通常指的是vkCmdDrawIndexed或vkCmdDraw调用。这是真正触发几何图元绘制的命令。

各环节性能分析：
🔥 高消耗环节（对性能要求高）
1. vkCmdDrawIndexed（drawcall本身）

原因：这是GPU实际开始执行顶点处理、光栅化、片段着色等渲染管线的起点

每个drawcall都有固定的驱动程序开销（CPU端）

过多的drawcall会导致CPU成为瓶颈

2. vkCmdBindPipeline

原因：绑定新的管线状态（特别是不同的着色器、混合状态等）代价高昂

管线切换是Vulkan中最昂贵的操作之一

优化策略：按管线状态排序drawcall，减少切换

3. vkCmdBindDescriptorSets

原因：绑定新的描述符集可能需要更新着色器资源绑定

描述符集切换也可能导致管线停顿

优化策略：合并资源到大型描述符集，使用bindless技术

🟡 中低消耗环节
1. vkCmdBindVertexBuffers / vkCmdBindIndexBuffer

中等开销，切换顶点/索引缓冲区相对较轻量

但如果频繁切换不同的缓冲区，仍会产生累积开销

2. vkCmdPushConstants

开销很小，因为是直接推入命令缓冲区的数据

适合每帧、每个对象变化的少量数据（如模型矩阵）

Stencil相关命令（vkCmdSetStencilReference等）

开销很小，只是设置状态参数

但如果频繁改变，仍会影响性能

3. vkCmdBeginRenderPass

开启渲染通道，涉及隐式清除操作和帧缓冲区切换

开销中等，但通常每帧只有几次