#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>

#include "input-struct.h"

class Node;
class Node2D;

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
	void setRoot2D(Node2D *root);
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
