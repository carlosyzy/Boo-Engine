#include "editor-camera.h"
#include "boo.h"
#include "log.h"
#include "core/scene/node-3d.h"
#include "core/renderer/3d/mesh-renderer.h"
#include "core/renderer/camera.h"
#include "core/input/input.h"
#include "core/input/input-type.h"
namespace Boo
{
	EditorCamera::EditorCamera(std::string name, Node *node, std::string uuid)
		: Component(name, node, uuid)
	{
		this->_layer = EComponentLayer::Layer3D;
		this->_node3D = dynamic_cast<Node3D *>(node);
		this->_camera = nullptr;
		this->_isMiddleMouseDown = false;
	}
	void EditorCamera::OnAwake()
	{
		this->_camera = dynamic_cast<Camera *>(this->_node3D->getComponent("Camera"));
		this->_worldFocusPos = {0, 0, 0};
		this->_node3D->setPosition(0, 30, 30);
		this->_node3D->setEulerAngles(-45, 0, 0);
		this->_resetFocus();
	}
	void EditorCamera::setProperty(json &data)
	{
		Component::setProperty(data);
	}

	void EditorCamera::OnEnable()
	{
		// 鼠标滚动事件
		this->_mouseScrollId = input->onMouseScroll([this](FInputScrollEvent &event)
													{ this->_onMouseScroll(event); });
		// 触摸板滚动事件
		this->_touchpadScrollId = input->onTouchpadScroll([this](FInputScrollEvent &event)
														  { this->_onTouchpadScroll(event); });
		// 鼠标按下事件
		this->_mouseDownId = input->onMouseDown([this](FInputMouseEvent &event)
												{ this->_onMouseDown(event); });
		// 鼠标释放事件
		this->_mouseUpId = input->onMouseUp([this](FInputMouseEvent &event)
											{ this->_onMouseUp(event); });
		// 鼠标移动事件
		this->_mouseMoveId = input->onMouseMove([this](FInputMouseEvent &event)
												{ this->_onMouseMove(event); });
	}
	void EditorCamera::_onMouseScroll(FInputScrollEvent &event)
	{
		std::cout << "mouse scroll: offsetX: " << event.offsetX << " offsetY: " << event.offsetY << std::endl;
		// 处理鼠标滚轮事件 摄像机的拉近和拉远
		this->_focusRadius = Vec3::distance(this->_node3D->getWorldPosition(), this->_worldFocusPos);
		this->_focusRadius -= event.offsetY * 0.5f;
		this->_updateCameraNodePos();
	}
	void EditorCamera::_onTouchpadScroll(FInputScrollEvent &event)
	{
		std::cout << "touchpad scroll: offsetX: " << event.offsetX << " offsetY: " << event.offsetY << std::endl;
		// 处理触摸板滚轮事件 摄像机的旋转
		if (event.offsetX != 0)
		{
			this->_rotateOffsetH -= event.offsetX * 0.02f;
		}
		const float MAX_VERTICAL_ANGLE = 1.48353f;	// 85度
		const float MIN_VERTICAL_ANGLE = -1.48353f; // -85度
		if (event.offsetY != 0)
		{
			this->_rotateOffsetV += event.offsetY * 0.02f;
			// 限制垂直角度范围，防止翻转
			if (this->_rotateOffsetV > MAX_VERTICAL_ANGLE)
			{
				this->_rotateOffsetV = MAX_VERTICAL_ANGLE;
			}
			if (this->_rotateOffsetV < MIN_VERTICAL_ANGLE)
			{
				this->_rotateOffsetV = MIN_VERTICAL_ANGLE;
			}
		}
		this->_updateCameraNodePos();
	}
	// 鼠标按下事件
	void EditorCamera::_onMouseDown(FInputMouseEvent &event)
	{
		std::cout << "mouse down: x: " << event.x << " y: " << event.y << std::endl;
		if (this->_isMiddleMouseDown)
			return;
		// 处理鼠标按下事件 摄像机的旋转
		if (event.button == 2)
		{
			this->_isMiddleMouseDown = true;
			this->_mouseX = event.x;
			this->_mouseY = event.y;
		}
	}
	// 鼠标释放事件
	void EditorCamera::_onMouseUp(FInputMouseEvent &event)
	{
		std::cout << "mouse up: x: " << event.x << " y: " << event.y << std::endl;
		// 处理鼠标释放事件 摄像机的旋转
		if (event.button == 2)
		{
			this->_isMiddleMouseDown = false;
		}
	}
	// 鼠标移动事件
	void EditorCamera::_onMouseMove(FInputMouseEvent &event)
	{
		// 处理鼠标释放事件 摄像机的旋转
		if (this->_isMiddleMouseDown)
		{
			float offsetX = event.x - this->_mouseX;
			float offsetY = event.y - this->_mouseY;
			this->_mouseX = event.x;
			this->_mouseY = event.y;
			std::cout << "mouse up: offsetX: " << offsetX << " offsetY: " << offsetY << std::endl;
			this->_rotateOffsetH -= offsetX * 0.01f;
			this->_rotateOffsetV -= offsetY * 0.01f;
			const float MAX_VERTICAL_ANGLE = 1.48353f;	// 85度
			const float MIN_VERTICAL_ANGLE = -1.48353f; // -85度
			if (this->_rotateOffsetV > MAX_VERTICAL_ANGLE)
			{
				this->_rotateOffsetV = MAX_VERTICAL_ANGLE;
			}
			if (this->_rotateOffsetV < MIN_VERTICAL_ANGLE)
			{
				this->_rotateOffsetV = MIN_VERTICAL_ANGLE;
			}
			this->_updateCameraNodePos();
		}
	}

	void EditorCamera::_resetFocus()
	{
		this->_focusRadius = Vec3::distance(this->_node3D->getWorldPosition(), this->_worldFocusPos);
		this->_rotateOffsetH = 0.0f;
		this->_rotateOffsetV = 0.0f;

		// 计算方向向量
		Vec3 sub;
		Vec3::subtract(this->_node3D->getWorldPosition(), this->_worldFocusPos, sub);
		sub.normalize();
		// 计算旋转角度
		this->_rotateOffsetH = atan2(sub.getX(), sub.getZ()); // 计算水平旋转角度
		float radiusXZ = sqrt(sub.getX() * sub.getX() + sub.getZ() * sub.getZ());
		this->_rotateOffsetV = atan2(sub.getY(), radiusXZ); // 计算垂直旋转角度
		std::cout << "rotateOffset: " << this->_rotateOffsetH << " " << this->_rotateOffsetV << std::endl;
	}
	void EditorCamera::_updateCameraNodePos()
	{
		// 第一种方案
		// 计算水平面投影的半径
		float radiusXZ = this->_focusRadius * cos(this->_rotateOffsetV);
		// 计算x轴和z轴的投影
		float x = radiusXZ * sin(this->_rotateOffsetH);
		float z = radiusXZ * cos(this->_rotateOffsetH);
		// 计算y轴
		float y = this->_focusRadius * sin(this->_rotateOffsetV);
		x += this->_worldFocusPos.getX();
		z += this->_worldFocusPos.getZ();
		y += this->_worldFocusPos.getY();
		this->_node3D->setWorldPosition(x, y, z);

		this->_updateCameraNodeAngle();
	}
	void EditorCamera::_updateCameraNodeAngle()
	{
		// 更新摄像机角度,让去始终看相焦点位置
		Vec3 forward;
		// 计算方向向量 因为是-Z 所以是焦点-摄像机
		Vec3::subtract(this->_node3D->getWorldPosition(), this->_worldFocusPos, forward);
		forward.normalize();
		Quat quat;
		Vec3 up = {0, 1, 0};
		Quat::fromViewUp(forward, up, quat);
		this->_node3D->setWorldRotation(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
	}

	void EditorCamera::Update(float deltaTime)
	{
	}
	void EditorCamera::LateUpdate(float deltaTime)
	{
	}

	void EditorCamera::OnDisable()
	{
		input->offMouseScroll(this->_mouseScrollId);
		input->offTouchpadScroll(this->_touchpadScrollId);
		input->offMouse(this->_mouseDownId);
		input->offMouse(this->_mouseUpId);
		input->offMouse(this->_mouseMoveId);
		this->_mouseScrollId = 0;
		this->_touchpadScrollId = 0;
		this->_mouseDownId = 0;
		this->_mouseUpId = 0;
		this->_mouseMoveId = 0;
	}
	void EditorCamera::destroy()
	{
		this->_focusRadius = 0.0f;
		this->_rotateOffsetH = 0.0f;
		this->_rotateOffsetV = 0.0f;
	}
	EditorCamera::~EditorCamera()
	{
		this->_node3D = nullptr;
		this->_camera = nullptr;
	}
}