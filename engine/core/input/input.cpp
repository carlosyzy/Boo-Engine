#include "input.h"
#include "boo.h"
#include "log.h"
#include "core/view/view.h"
#include "core/game.h"
#include "core/scene/node.h"
#include "core/scene/node-2d.h"
#include "core/scene/scene.h"
#include "core/math/mat4.h"
#include "core/math/vec3.h"

namespace Boo
{

	Input::Input() : _cursorX(0.0), _cursorY(0.0), _inputId(0)
	{
	}
	void Input::init()
	{
	}
	/**
	 * @brief 鼠标按钮事件
	 * @param button 鼠标按钮 0: 左键 1: 右键 2: 中键
	 * @param action 事件动作 0:抬起 1:按下 99:松开
	 * @param mods 按键修饰符
	 */
	void Input::_emitMouseButton(int button, int action, int mods)
	{
		this->_mouseButton = button;
		this->_mouseMods = mods;
		if (this->_cursorX < -view->getWidth() / 2.0 || this->_cursorX > view->getWidth() / 2.0)
		{
			return;
		}
		if (this->_cursorY < -view->getHeight() / 2.0 || this->_cursorY > view->getHeight() / 2.0)
		{
			return;
		}
		if (action == 1)
		{
			this->_emitInputMouseDown();
			if (this->_mouseButton == 0)
			{
				this->_emitNodeTouchStart();
			}
		}
		else if (action == 0)
		{
			// 触摸板释放事件
			this->_emitInputMouseUp();
			if (this->_mouseButton == 0)
			{
				this->_emitNodeTouchEnd();
			}
		}
	}
	/**
	 * @brief 鼠标移动事件
	 * @param xpos 鼠标X坐标
	 * @param ypos 鼠标Y坐标
	 */
	void Input::_emitMouseMove(double xpos, double ypos)
	{
		this->_cursorX = xpos - view->getWidth() / 2.0;
		this->_cursorY = view->getHeight() / 2.0 - ypos;
		if (this->_cursorX < -view->getWidth() / 2.0 || this->_cursorX > view->getWidth() / 2.0)
		{
			return;
		}
		if (this->_cursorY < -view->getHeight() / 2.0 || this->_cursorY > view->getHeight() / 2.0)
		{
			return;
		}
		const Mat4 &fitMatrix = view->getFitMatrix();
		this->_uiCursorX = this->_cursorX / fitMatrix.getM0();
		this->_uiCursorY = this->_cursorY / fitMatrix.getM5();
		// 触摸板移动事件
		this->_emitInputMouseMove();
		this->_emitNodeTouchMove();
	}
	/**
	 * @brief 滚动事件
	 * @param xoffset 滚动X偏移量
	 * @param yoffset 滚动Y偏移量
	 */
	void Input::_emitScroll(double xoffset, double yoffset)
	{
		if (xoffset == 0 && yoffset - std::floor(yoffset) == 0)
		{
			this->_emitMouseScroll(xoffset, yoffset);
		}
		else
		{
			this->_emitTouchpadScroll(xoffset, yoffset);
		}
	}
	/**
	 * @brief 键盘事件
	 * @param key 键盘键值
	 * @param scancode 键盘扫描码
	 * @param action 键盘操作 0:抬起 1:按下
	 * @param mods 键盘修饰键 0:无 1:Shift 2:Ctrl 3:Alt
	 */
	void Input::_emitKey(int key, int scancode, int action, int mods)
	{
		std::cout << "Input::emitKey  key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods << std::endl;
	}
	/**
	 * @brief 触摸事件
	 * @param action 触摸操作 0:按下, 1:抬起, 2:移动, 3:取消
	 * @param x 触摸X坐标
	 * @param y 触摸Y坐标
	 */
	void Input::_emitTouch(int action, float x, float y)
	{
		this->_cursorX = x - view->getWidth() / 2.0;
		this->_cursorY = view->getHeight() / 2.0 - y;
		if (this->_cursorX < -view->getWidth() / 2.0 || this->_cursorX > view->getWidth() / 2.0)
		{
			return;
		}
		if (this->_cursorY < -view->getHeight() / 2.0 || this->_cursorY > view->getHeight() / 2.0)
		{
			return;
		}
		const Mat4 &fitMatrix = view->getFitMatrix();
		this->_uiCursorX = this->_cursorX / fitMatrix.getM0();
		this->_uiCursorY = this->_cursorY / fitMatrix.getM5();
		// 触摸板移动事件

		if (action == 0)
		{
			this->_emitNodeTouchStart();
		}
		else if (action == 1)
		{
			this->_emitNodeTouchEnd();
		}
		else if (action == 2)
		{
			this->_emitNodeTouchMove();
		}
		else if (action == 3)
		{
		}
	}

	void Input::_emitInputMouseDown()
	{
		// 鼠标按下事件
		for (auto &[id, scrollStruct] : this->_mouseButtonMap)
		{
			if (scrollStruct.type == 1)
			{
				scrollStruct.event.button = this->_mouseButton;
				scrollStruct.event.mods = this->_mouseMods;
				scrollStruct.event.x = this->_cursorX;
				scrollStruct.event.y = this->_cursorY;
				scrollStruct.func(scrollStruct.event);
			}
		}
	}
	void Input::_emitInputMouseUp()
	{
		// 鼠标释放事件
		for (auto &[id, scrollStruct] : this->_mouseButtonMap)
		{
			if (scrollStruct.type == 0)
			{
				scrollStruct.event.button = this->_mouseButton;
				scrollStruct.event.mods = this->_mouseMods;
				scrollStruct.event.x = this->_cursorX;
				scrollStruct.event.y = this->_cursorY;
				scrollStruct.func(scrollStruct.event);
			}
		}
	}
	void Input::_emitInputMouseMove()
	{
		// 鼠标移动事件
		for (auto &[id, scrollStruct] : this->_mouseButtonMap)
		{
			if (scrollStruct.type == 2)
			{
				scrollStruct.event.button = this->_mouseButton;
				scrollStruct.event.mods = this->_mouseMods;
				scrollStruct.event.x = this->_cursorX;
				scrollStruct.event.y = this->_cursorY;
				scrollStruct.func(scrollStruct.event);
			}
		}
	}
	void Input::_emitMouseScroll(double xoffset, double yoffset)
	{
		for (auto &[id, scrollStruct] : this->_mouseScrollMap)
		{
			scrollStruct.event.offsetX = xoffset;
			scrollStruct.event.offsetY = yoffset;
			scrollStruct.func(scrollStruct.event);
		}
	}
	void Input::_emitTouchpadScroll(double xoffset, double yoffset)
	{
		for (auto &[id, scrollStruct] : this->_touchpadScrollMap)
		{
			scrollStruct.event.offsetX = xoffset;
			scrollStruct.event.offsetY = yoffset;
			scrollStruct.func(scrollStruct.event);
		}
	}
	void Input::_disposeInputNode(Node2D *node, std::vector<Node2D *> &nodes)
	{
		// 触摸板事件处理
		Node2D *node2d = dynamic_cast<Node2D *>(node);
		if (node2d != nullptr && node2d->getActive())
		{
			if ((node2d->inHitMask(this->_uiCursorX, this->_uiCursorY)))
			{
				nodes.push_back(node2d);
				for (auto &child : node2d->getChildren())
				{
					this->_disposeInputNode(dynamic_cast<Node2D *>(child), nodes);
				}
			}
		}
	}
	void Input::_emitNodeTouchStart()
	{
		// 触摸板按下事件
		Scene *scene = game->getScene();
		Node2D *root = scene->getRoot2D();
		std::vector<Node2D *> nodes;
		this->_disposeInputNode(root, nodes);
		// 反向遍历，处理事件
		for (int i = nodes.size() - 1; i >= 0; i--)
		{
			Node2D *node2d = nodes[i];
			const std::string &nodeUuid = node2d->getUuid();
			if (this->_nodeTouchStartIDMap.find(nodeUuid) == this->_nodeTouchStartIDMap.end())
			{
				// 当前节点未绑定按下事件,跳过
				continue;
			}
			uint64_t id = this->_nodeTouchStartIDMap[nodeUuid];
			FNodeTouchStruct &inputStruct = this->_nodeTouchMap[id];
			if (inputStruct.type != 1)
			{
				continue;
			}
			bool isIn = node2d->inHitOnNode(this->_uiCursorX, this->_uiCursorY);
			if (!isIn)
			{
				continue;
			}
			Vec3 worldPos(this->_uiCursorX, this->_uiCursorY, 1.0f);
			const Mat4 &worldMat = node2d->getWorldMatrix();
			Mat4 localMat;
			Mat4::inverse(worldMat, localMat);
			Vec3 localPos;
			Mat4::multiplyVec3(localMat, worldPos, localPos);
			inputStruct.event.x = localPos.getX();
			inputStruct.event.y = localPos.getY();
			inputStruct.event.worldX = this->_uiCursorX;
			inputStruct.event.worldY = this->_uiCursorY;
			inputStruct.event.viewX = this->_cursorX;
			inputStruct.event.viewY = this->_cursorY;
			inputStruct.func(inputStruct.event);

			// 拦截事件,直接停止处理
			if (inputStruct.isIntercept)
			{
				break;
			}
		}
	}
	void Input::_emitNodeTouchMove()
	{
		// 触摸板移动事件
		Scene *scene = game->getScene();
		Node2D *root = scene->getRoot2D();
		std::vector<Node2D *> nodes;
		this->_disposeInputNode(root, nodes);
		// 反向遍历，处理事件
		for (int i = nodes.size() - 1; i >= 0; i--)
		{
			Node2D *node2d = nodes[i];
			const std::string &nodeUuid = node2d->getUuid();
			if (this->_nodeTouchMoveIDMap.find(nodeUuid) == this->_nodeTouchMoveIDMap.end())
			{
				// 当前节点未绑定移动事件,跳过
				continue;
			}
			uint64_t id = this->_nodeTouchMoveIDMap[nodeUuid];
			FNodeTouchStruct &inputStruct = this->_nodeTouchMap[id];
			if (inputStruct.type != 2)
			{
				continue;
			}
			bool isIn = node2d->inHitOnNode(this->_uiCursorX, this->_uiCursorY);
			if (!isIn)
			{
				continue;
			}
			Vec3 worldPos(this->_uiCursorX, this->_uiCursorY, 1.0f);
			const Mat4 &worldMat = node2d->getWorldMatrix();
			Mat4 localMat;
			Mat4::inverse(worldMat, localMat);
			Vec3 localPos;
			Mat4::multiplyVec3(localMat, worldPos, localPos);
			inputStruct.event.x = localPos.getX();
			inputStruct.event.y = localPos.getY();
			inputStruct.event.worldX = this->_uiCursorX;
			inputStruct.event.worldY = this->_uiCursorY;
			inputStruct.event.viewX = this->_cursorX;
			inputStruct.event.viewY = this->_cursorY;
			inputStruct.func(inputStruct.event);

			// 拦截事件,直接停止处理
			if (inputStruct.isIntercept)
			{
				break;
			}
		}
	}
	void Input::_emitNodeTouchEnd()
	{
		// 触摸板释放事件
		Scene *scene = game->getScene();
		Node2D *root = scene->getRoot2D();
		std::vector<Node2D *> nodes;
		this->_disposeInputNode(root, nodes);
		// 反向遍历，处理事件
		for (int i = nodes.size() - 1; i >= 0; i--)
		{
			Node2D *node2d = nodes[i];
			const std::string &nodeUuid = node2d->getUuid();
			if (this->_nodeTouchEndIDMap.find(nodeUuid) == this->_nodeTouchEndIDMap.end())
			{
				// 当前节点未绑定释放事件,跳过
				continue;
			}
			uint64_t id = this->_nodeTouchEndIDMap[nodeUuid];
			FNodeTouchStruct &inputStruct = this->_nodeTouchMap[id];
			if (inputStruct.type != 0)
			{
				continue;
			}
			bool isIn = node2d->inHitOnNode(this->_uiCursorX, this->_uiCursorY);
			if (!isIn)
			{
				continue;
			}
			Vec3 worldPos(this->_uiCursorX, this->_uiCursorY, 1.0f);
			const Mat4 &worldMat = node2d->getWorldMatrix();
			Mat4 localMat;
			Mat4::inverse(worldMat, localMat);
			Vec3 localPos;
			Mat4::multiplyVec3(localMat, worldPos, localPos);
			inputStruct.event.x = localPos.getX();
			inputStruct.event.y = localPos.getY();
			inputStruct.event.worldX = this->_uiCursorX;
			inputStruct.event.worldY = this->_uiCursorY;
			inputStruct.event.viewX = this->_cursorX;
			inputStruct.event.viewY = this->_cursorY;
			inputStruct.func(inputStruct.event);
			// 拦截事件,直接停止处理
			if (inputStruct.isIntercept)
			{
				break;
			}
		}
	}
	uint64_t Input::onMouseScroll(std::function<void(FInputScrollEvent &)> func)
	{
		uint64_t id = ++this->_inputId;
		FInputScrollEvent event;
		event.id = id;
		event.offsetX = 0;
		event.offsetY = 0;
		FInputScrollStruct scrollStruct;
		scrollStruct.func = func;
		scrollStruct.id = id;
		scrollStruct.event = event;
		this->_mouseScrollMap[id] = scrollStruct;
		return id;
	}
	uint64_t Input::onTouchpadScroll(std::function<void(FInputScrollEvent &)> func)
	{
		uint64_t id = ++this->_inputId;
		FInputScrollEvent event;
		event.id = id;
		event.offsetX = 0;
		event.offsetY = 0;
		FInputScrollStruct scrollStruct;
		scrollStruct.func = func;
		scrollStruct.id = id;
		scrollStruct.event = event;
		this->_touchpadScrollMap[id] = scrollStruct;
		return id;
	}
	void Input::offMouseScroll(uint64_t id)
	{
		this->_mouseScrollMap.erase(id);
	}
	void Input::offTouchpadScroll(uint64_t id)
	{
		this->_touchpadScrollMap.erase(id);
	}

	uint64_t Input::onMouseDown(std::function<void(FInputMouseEvent &)> func)
	{
		uint64_t id = ++this->_inputId;
		FInputMouseEvent event;
		event.id = id;
		event.button = 0;
		event.mods = 0;
		event.x = 0;
		event.y = 0;
		FInputMouseStruct buttonStruct;
		buttonStruct.func = func;
		buttonStruct.id = id;
		buttonStruct.event = event;
		buttonStruct.type = 1;
		this->_mouseButtonMap[id] = buttonStruct;
		return id;
	}
	uint64_t Input::onMouseUp(std::function<void(FInputMouseEvent &)> func)
	{
		uint64_t id = ++this->_inputId;
		FInputMouseEvent event;
		event.id = id;
		event.button = 0;
		event.mods = 0;
		event.x = 0;
		event.y = 0;
		FInputMouseStruct buttonStruct;
		buttonStruct.func = func;
		buttonStruct.id = id;
		buttonStruct.event = event;
		buttonStruct.type = 0;
		this->_mouseButtonMap[id] = buttonStruct;
		return id;
	}
	uint64_t Input::onMouseMove(std::function<void(FInputMouseEvent &)> func)
	{
		uint64_t id = ++this->_inputId;
		FInputMouseEvent event;
		event.id = id;
		event.button = 0;
		event.mods = 0;
		event.x = 0;
		event.y = 0;
		FInputMouseStruct buttonStruct;
		buttonStruct.func = func;
		buttonStruct.id = id;
		buttonStruct.type = 2;
		buttonStruct.event = event;
		this->_mouseButtonMap[id] = buttonStruct;
		return id;
	}
	/**
	 * @brief 注销鼠标按下事件
	 * @param id 输入事件ID
	 */
	void Input::offMouse(uint64_t id)
	{
		this->_mouseButtonMap.erase(id);
	}
	uint64_t Input::onNodeTouchStart(Node2D *node, std::function<void(FNodeTouchEvent &)> func, bool isIntercept)
	{
		if(node==nullptr){
			return 0;
		}
		uint64_t id = ++this->_inputId;
		FNodeTouchEvent event;
		event.id = id;
		event.x = 0;
		event.y = 0;
		event.worldX = 0;
		event.worldY = 0;
		FNodeTouchStruct inputStruct;
		inputStruct.func = func;
		inputStruct.id = id;
		inputStruct.event = event;
		inputStruct.type = 1;
		inputStruct.isIntercept = isIntercept;
		inputStruct.node = node;
		this->_nodeTouchMap[id] = inputStruct;
		this->_nodeTouchStartIDMap[node->getUuid()] = id;
		return id;
	}
	uint64_t Input::onNodeTouchMove(Node2D *node, std::function<void(FNodeTouchEvent &)> func, bool isIntercept)
	{
		if(node==nullptr){
			return 0;
		}
		uint64_t id = ++this->_inputId;
		FNodeTouchEvent event;
		event.id = id;
		event.x = 0;
		event.y = 0;
		event.worldX = 0;
		event.worldY = 0;
		FNodeTouchStruct inputStruct;
		inputStruct.func = func;
		inputStruct.id = id;
		inputStruct.event = event;
		inputStruct.type = 2;
		inputStruct.isIntercept = isIntercept;
		inputStruct.node = node;
		this->_nodeTouchMap[id] = inputStruct;
		this->_nodeTouchMoveIDMap[node->getUuid()] = id;
		return id;
	}
	uint64_t Input::onNodeTouchEnd(Node2D *node, std::function<void(FNodeTouchEvent &)> func, bool isIntercept)
	{
		if(node==nullptr){
			return 0;
		}
		uint64_t id = ++this->_inputId;
		FNodeTouchEvent event;
		event.id = id;
		event.x = 0;
		event.y = 0;
		event.worldX = 0;
		event.worldY = 0;
		FNodeTouchStruct inputStruct;
		inputStruct.func = func;
		inputStruct.id = id;
		inputStruct.event = event;
		inputStruct.type = 0;
		inputStruct.isIntercept = isIntercept;
		inputStruct.node = node;
		this->_nodeTouchMap[id] = inputStruct;
		this->_nodeTouchEndIDMap[node->getUuid()] = id;
		return id;
	}
	void Input::offNodeTouch(Node2D *node, uint64_t id)
	{
		if(node==nullptr){
			return;
		}
		FNodeTouchStruct &inputStruct = this->_nodeTouchMap[id];
		if (inputStruct.type == 1)
		{
			this->_nodeTouchStartIDMap.erase(node->getUuid());
			this->_nodeTouchMap.erase(id);
		}
		else if (inputStruct.type == 0)
		{
			this->_nodeTouchEndIDMap.erase(node->getUuid());
			this->_nodeTouchMap.erase(id);
		}
		else if (inputStruct.type == 2)
		{
			this->_nodeTouchMoveIDMap.erase(node->getUuid());
			this->_nodeTouchMap.erase(id);
		}
		else
		{
			LOGW("[Input]:offNodeTouch:: %s, %s, inputStruct.type is not 1, 0, 2", node->getName().c_str(), id);
			return;
		}
	}
	void Input::offAllNodeTouch(Node2D *node)
	{
		if(node==nullptr){
			return;
		}
		if (this->_nodeTouchStartIDMap.find(node->getUuid()) != this->_nodeTouchStartIDMap.end())
		{
			uint64_t id = this->_nodeTouchStartIDMap[node->getUuid()];
			this->_nodeTouchStartIDMap.erase(node->getUuid());
			this->_nodeTouchMap.erase(id);
		}
		if (this->_nodeTouchMoveIDMap.find(node->getUuid()) != this->_nodeTouchMoveIDMap.end())
		{
			uint64_t id = this->_nodeTouchMoveIDMap[node->getUuid()];
			this->_nodeTouchMoveIDMap.erase(node->getUuid());
			this->_nodeTouchMap.erase(id);
		}
		if (this->_nodeTouchEndIDMap.find(node->getUuid()) != this->_nodeTouchEndIDMap.end())
		{
			uint64_t id = this->_nodeTouchEndIDMap[node->getUuid()];
			this->_nodeTouchEndIDMap.erase(node->getUuid());
			this->_nodeTouchMap.erase(id);
		}
	}

	Input::~Input()
	{
	}
} // namespace Boo