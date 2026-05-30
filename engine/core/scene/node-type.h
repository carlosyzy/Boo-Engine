#pragma once
#include <cstdint>
namespace Boo
{
    /**
     * @brief 节点层级
     * 标注当前节点属于那种: 2D节点、3D节点、场景节点
     */
    enum class ENodeLayer
	{
		Default = 0,
		Node2D,
		Node3D,
		Scene,
	};
    /**
     * @brief 节点组
     * 目前默认Default,Node2D,Node3D,用于摄像机的Groups确定其可见性
     */
    enum class ENodeGroup
	{
		Default = 1 << 0,
		Node3D = 1 << 1,
		Node2D = 1 << 2,
	};
    /**
     * @brief 节点变换标志
     * 标注当前节点的变换标志: 位置、旋转、缩放、大小、锚点
     * 用于判断节点是否需要更新其变换矩阵
     * 目前属于内部使用，不建议外部直接操作
     */
    enum ENodeTransformFlag : uint32_t
	{
		NONE_FLAG = 0,
		POSITION_FLAG = 1 << 0,
		ROTATION_FLAG = 1 << 1,
		SCALE_FLAG = 1 << 2,
		SIZE_FLAG = 1 << 3,
		ANCHOR_FLAG = 1 << 4,
		ALL_FLAG = POSITION_FLAG | ROTATION_FLAG | SCALE_FLAG | SIZE_FLAG | ANCHOR_FLAG,
	};
	/**
	 * @brief 2d 节点的大小锁
	 * 标注当前节点的大小锁: None, SpriteRaw
	 * 用于判断节点是否需要更新其变换矩阵
	 * 目前属于内部使用，不建议外部直接操作
	*/
	// 添加组：groups | = group（位或运算）
    // 移除组：groups &= ~group（位与+非运算）
    // 检查组：(groups & group) == group（位与运算）
    // 切换组：groups ^= group（位异或运算）
    enum class ENode2DSizeLock
    {
        None = 0,
        SpriteRaw = 1 << 0,
    };
	
} // namespace Boo
