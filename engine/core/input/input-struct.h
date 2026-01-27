#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>

class Node;
class Node2D;


struct NodeInputResult
{
	Node2D *node = nullptr;
	float localX = 0;
	float localY = 0;
	float worldX = 0;
	float worldY = 0;
	/**
	 * 鼠标按钮 0: 左键 1: 右键 2: 中键
	 */
	int button = 0;
};
enum class NodeInput
{
	/**
	 * 触摸开始
	 */
	TOUCH_START,
	/**
	 * 触摸移动
	 */
	TOUCH_MOVE,
	/**
	 * 触摸结束
	 */
	TOUCH_END,
	/**
	 * 触摸取消
	 */
	TOUCH_CANCEL,
	/**
	 * 光标悬停
	 */
	CURSOR_HOVER,
	/**
	 * 在节点外悬停
	 */
	CURSOR_HOVER_OUT,
};

/**
 * 节点的输入事件
 * @brief 节点输入结构体
 */
struct NodeInputStruct
{

	Node2D *node = nullptr;

	bool isIntercept = false;

	int status = 0;

	NodeInputResult touchResult{};

	std::map<int, std::function<void(NodeInputResult &)>> touchStarts;
	std::map<int, std::function<void(NodeInputResult &)>> touchMoves;
	std::map<int, std::function<void(NodeInputResult &)>> touchEnds;
	std::map<int, std::function<void(NodeInputResult &)>> touchCancels;
	std::map<int, std::function<void(NodeInputResult &)>> cursorHovers;
	std::map<int, std::function<void(NodeInputResult &)>> cursorHoverOuts;
};