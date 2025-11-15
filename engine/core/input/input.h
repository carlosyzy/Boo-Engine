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
	TOUCH_CANCEL
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

	template <typename T, typename Func>
	int onNodeInputEvent(Node2D *node, NodeInput input, Func func, T *instance, bool isIntercept);
	/*
	int onNodeInputEvent(Node2D* node, NodeInput input, Func func, T* instance, bool isIntercept)
	{
		int id = this->_nodeInputCallId++;
		if (this->_nodeInputMap.find(node->getUuid()) == this->_nodeInputMap.end())
		{
			NodeInputStruct nodeInputStruct;
			nodeInputStruct.node = node;
			nodeInputStruct.isIntercept = isIntercept;
			nodeInputStruct.touchResult.node = node;
			nodeInputStruct.status = 0;
			this->_nodeInputMap.emplace(node->getUuid(), nodeInputStruct);
		}
		auto callback = [func, instance](NodeInputResult& result)
			{
				if (instance != nullptr && func != nullptr)
				{
					(instance->*func)(result);
				}
			};
		if (input == NodeInput::TOUCH_START)
		{
			this->_nodeInputMap[node->getUuid()].touchStarts.emplace(id, callback);
		}
		else if (input == NodeInput::TOUCH_END)
		{
			this->_nodeInputMap[node->getUuid()].touchEnds.emplace(id, callback);
		}
		else if (input == NodeInput::TOUCH_MOVE)
		{
			this->_nodeInputMap[node->getUuid()].touchMoves.emplace(id, callback);
		}
		else if (input == NodeInput::TOUCH_CANCEL)
		{
			this->_nodeInputMap[node->getUuid()].touchCancels.emplace(id, callback);
		}
		return id;
	}*/

	void offNodeInputEvent(Node2D *node, int inputID);

	void offAllNodeInputEvent(Node2D *node);

	~Input();
};
