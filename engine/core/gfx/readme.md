# GFX 图形管理器
# vulkan 版本 1.3+

## history 历史
- 2025 12.7
 1. 交换链重置影响必须重置对象:renderpass,pipeline,   

- 2025 12.5 重新设计结构
 1. gfx-context 图形上下文保持不变
 2. renderpass 全局统一
    - Default: 清除颜色附件和深度附件
    - UI: renderpass1: 离屏渲染,双通道,颜色附件,深度附件  renderpass2: 屏幕渲染,单通道,颜色附件
    - 3D: renderpass1: 离屏渲染,双通道,颜色附件,深度附件  renderpass2: 屏幕渲染,单通道,颜色附件
 3. pipeline:
    - Default: 顶点输入只包含顶点和uv
    - UI: 顶点输入只包顶点和uv
    - 3D: 顶点输入包含顶点，uv，法线，切线，位法向量, 颜色,可能还有多套uv,
    

 2. 取消gfx-object的这个类，不需要存储渲染物体，在camera的过程中直接整理渲染数据，提交渲染
 3. camera 渲染直接提交到渲染队列中，重构渲染队列，队列中添加渲染批次，不同渲染状态在不同批次中进行渲染
 4. 每个摄像机都需要对场景中中的所有的物体进行渲染遍历

- 2025 12.3
 1. 保持pass和pipeline结构不变，添加检测pass相同性和pipeline相同性的功能
 2. 取消gfx-object的这个类，不需要存储渲染物体，在camera的过程中直接整理渲染数据，提交渲染
 3. camera 渲染直接提交到渲染队列中，重构渲染队列，队列中添加渲染批次，不同渲染状态在不同批次中进行渲染
 4. 每个摄像机都需要对场景中中的所有的物体进行渲染遍历
 5. pass和pipeline 采取集成式的创建，手动进行pass和pipeline队列的管理
 6. pass,pipeline,shader 采取关键影响状态Hash ID的存储方式



| 资源类型           | 依赖交换链？ | 重建时机     | 原因             |
  |----------------|--------|----------|----------------|
  | UBO 缓冲区        | ❌ 否    | 只创建一次    | 只是内存块，不涉及格式/尺寸 |
  | UBO 内容         | ⚠️ 部分  | 每帧更新     | 投影矩阵等可能依赖窗口尺寸  |
  | Render Pass    | ✅ 是    | 交换链重建时   | 依赖图像格式         |
  | Framebuffer    | ✅ 是    | 交换链重建时   | 依赖图像视图和尺寸      |
  | Image View     | ✅ 是    | 交换链重建时   | 依赖交换链图像        |
  | Descriptor Set | ❌ 否    | 只创建一次    | 绑定关系不变         |
  | Pipeline       | ⚠️ 看情况 | 看是否用动态状态 | 动态视口/裁剪可以不重建   |


## gfx-mgr 图形管理器
- 图形渲染管理：分别负责gfx-context（专门负责 Vulkan 环境的初始化、管理和生命周期）以及gfx-renderer（负责渲染相关的操作，如绘制、交换链管理等）
### gfx-context 图形上下文
- 负责 Vulkan 环境的初始化、管理和生命周期
- 提供 Vulkan 相关的资源管理，如设备、队列、交换链等
- 负责 Vulkan 实例的创建和销毁
### gfx-renderer 渲染器
- 管理所有物体渲染
- 管理gfx-shader，gfx-texture，gfx-pass，gfx-pipeline全局复用缓存

#### gfx-shader 着色器
- 通过着色器代码，动态进行读取，编译，缓存到全局复用缓存中
- default.vert 顶点着色器
- default.geom 几何着色器（暂时不支持）
- default.frag 像素着色器

#### gfx-pass 渲染通道
- 通过 pass 的决定性属性创建指定通道，缓存到全局复用缓存中
- 一个pass可以对应多个pipeline

#### gfx-pipeline 渲染管线
- 通过shader，pass以及管线的决定性属性创建指定管线，缓存到全局复用缓存中
- 一个pipeline可以对应多个pass

#### gfx-texture 纹理
- vulkan 渲染纹理，直接传入数据引用，不进行数据拷贝

投影举证设置为全局属性
<!-- 

#### gfx-material 材质
- 材质为自定义的结构体，包含 shader，pass，pipeline，texture 等信息
- 相同材质并且顶点结构相同的，理论上可以进行渲染的合批吧
**结构设计：整体暂定 json 格式**
```
{
    "name": "material-ui",
    "pipelines": [
        {
            "name": "default",
            "vertexShader": "default.vert",
            "fragmentShader": "default.frag",
            "pass": "default"
        }
    ],
    "textures": ""
}
```


- 1.顶点着色器关联
- 2.几何着色器关联（暂时不支持）
- 3.像素着色器关联
- 4.渲染通道关联
- 5.渲染管线关联
- 6.渲染贴图关联 -->


