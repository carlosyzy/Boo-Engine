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
	TOUCH_START,
	TOUCH_MOVE,
	TOUCH_END,
	TOUCH_CANCEL,
	CURSOR_HOVER,
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
};