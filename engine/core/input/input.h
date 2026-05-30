#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>

#include "input-type.h"

namespace Boo
{
	class Node;
	class Node2D;

	class Input
	{
	private:
		float _cursorX = 0;
		float _cursorY = 0;
		float _uiCursorX = 0;
		float _uiCursorY = 0;
		int _mouseButton = 0;
		int _mouseMods = 0;
		uint64_t _inputId = 0;
		/**
		 * @brief 鼠标滚动事件
		 */
		std::map<uint64_t, FInputScrollStruct> _mouseScrollMap;
		/**
		 * @brief 触摸板滚动事件
		 */
		std::map<uint64_t, FInputScrollStruct> _touchpadScrollMap;
		/**
		 * @brief 鼠标事件
		 */
		std::map<uint64_t, FInputMouseStruct> _mouseButtonMap;

		/**
		 * @brief 节点事件 键为节点UUID
		 * @param 为节点输入事件结构体
		 */
		std::map<uint64_t, FNodeTouchStruct> _nodeTouchMap;
		std::map<std::string, uint64_t> _nodeTouchStartIDMap;
		std::map<std::string, uint64_t> _nodeTouchMoveIDMap;
		std::map<std::string, uint64_t> _nodeTouchEndIDMap;

	public:
		/**
		 * @brief 鼠标按钮事件
		 * @param button 鼠标按钮 0: 左键 1: 右键 2: 中键
		 * @param action 事件动作 GLFW_PRESS(0): 按下 GLFW_RELEASE(1): 释放
		 * @param mods 按键修饰符 0:无 1:Shift 2:Ctrl 3:Alt
		 */
		void _emitMouseButton(int button, int action, int mods);
		/**
		 * @brief 鼠标位置事件
		 * @param xpos 鼠标X坐标
		 * @param ypos 鼠标Y坐标
		 */
		void _emitMouseMove(double xpos, double ypos);
		/**
		 * @brief 更新滚动状态
		 * @param xoffset 滚动X偏移量
		 * @param yoffset 滚动Y偏移量
		 */
		void _emitScroll(double xoffset, double yoffset);
		/**
		 * @brief 鼠标滚动事件
		 * @param xoffset 滚动X偏移量
		 * @param yoffset 滚动Y偏移量
		 */
		void _emitMouseScroll(double xoffset, double yoffset);
		/**
		 * @brief 触摸板滚动事件
		 * @param xoffset 滚动X偏移量
		 * @param yoffset 滚动Y偏移量
		 */
		void _emitTouchpadScroll(double xoffset, double yoffset);

		void _emitKey(int key, int scancode, int action, int mods);
		/**
		 * @brief 触摸事件
		 * @param action 触摸操作 0:按下, 1:抬起, 2:移动, 3:取消
		 * @param x 触摸X坐标
		 * @param y 触摸Y坐标
		 */
		void _emitTouch(int action, float x, float y);

	private:
		void _emitInputMouseDown();
		void _emitInputMouseUp();
		void _emitInputMouseMove();
		/**
		 * 遍历节点
		 */
		void _disposeInputNode(Node2D *node, std::vector<Node2D *> &nodes);
		void _emitNodeTouchStart();
		void _emitNodeTouchMove();
		void _emitNodeTouchEnd();

	public:
		Input();
		void init();
		/**
		 * @brief 鼠标滚动事件
		 * @param func 回调函数
		 * @return int 输入事件ID
		 */
		uint64_t onMouseScroll(std::function<void(FInputScrollEvent &)> func);
		/**
		 * @brief 触摸板滚动事件
		 * @param func 回调函数
		 * @return int 输入事件ID
		 */
		uint64_t onTouchpadScroll(std::function<void(FInputScrollEvent &)> func);
		void offMouseScroll(uint64_t id);
		void offTouchpadScroll(uint64_t id);
		/**
		 * @brief 鼠标按下事件
		 * @param func 回调函数
		 * @return int 输入事件ID
		 */
		uint64_t onMouseDown(std::function<void(FInputMouseEvent &)> func);
		/**
		 * @brief 鼠标释放事件
		 * @param func 回调函数
		 * @return int 输入事件ID
		 */
		uint64_t onMouseUp(std::function<void(FInputMouseEvent &)> func);
		/**
		 * @brief 鼠标移动事件
		 * @param func 回调函数
		 * @return int 输入事件ID
		 */
		uint64_t onMouseMove(std::function<void(FInputMouseEvent &)> func);
		/**
		 * @brief 注销鼠标按下事件
		 * @param id 输入事件ID
		 */
		void offMouse(uint64_t id);

		/**
		 * @brief 节点触摸开始事件
		 * @param node 节点
		 * @param func 回调函数
		 * @param isIntercept 是否拦截事件
		 * @return int 输入事件ID
		 */
		uint64_t onNodeTouchStart(Node2D *node, std::function<void(FNodeTouchEvent &)> func, bool isIntercept = false);
		/**
		 * @brief 节点触摸移动事件
		 * @param node 节点
		 * @param func 回调函数
		 * @param isIntercept 是否拦截事件
		 * @return int 输入事件ID
		 */
		uint64_t onNodeTouchMove(Node2D *node, std::function<void(FNodeTouchEvent &)> func, bool isIntercept = false);
		/**
		 * @brief 节点触摸结束事件
		 * @param node 节点
		 * @param func 回调函数
		 * @param isIntercept 是否拦截事件
		 * @return int 输入事件ID
		 */
		uint64_t onNodeTouchEnd(Node2D *node, std::function<void(FNodeTouchEvent &)> func, bool isIntercept = false);
		/**
		 * @brief 注销节点触摸事件
		 * @param node 节点
		 * @param id 输入事件ID
		 */
		void offNodeTouch(Node2D *node, uint64_t id);
		/**
		 * @brief 注销节点所有触摸事件
		 * @param node 节点
		 */
		void offAllNodeTouch(Node2D *node);

		~Input();
	};

} // namespace Boo

#include "input-impl.h"
