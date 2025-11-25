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

class Input
{
private:
	float _cursorX = 0;
	float _cursorY = 0;
	bool _isTouching = false;
	Node2D *_root = nullptr;
	int _nodeInputCallId = 0;
	std::unordered_map<std::string, NodeInputStruct> _nodeInputMap;
	bool _propagateEvent(Node *node, int button, int action);

public:
	Input();
	void init();
	void setRoot(Node2D *root);
	void onMouseButton(int button, int action, int mods);
	void onCursorPos(double xpos, double ypos);
	void onKey(int key, int scancode, int action, int mods);

	template <typename T, typename Func>
	int onNodeInputEvent(Node2D *node, NodeInput input, Func func, T *instance, bool isIntercept=true);
	void offNodeInputEvent(Node2D *node, int inputID);
	void offAllNodeInputEvent(Node2D *node);

	~Input();
};

#include "input-impl.h"
