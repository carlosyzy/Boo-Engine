#pragma once
#include <iostream>
#include <cmath>
#include "../../engine/core/component/component.h"
#include "../../engine/core/component/component-register.h"

class Scene;
class Node2D;
class UISprite;
class UIWidget;
class Camera;
class EditorLayout;

class EditorHierarchy;

class EditorMain : public Component
{
private:
	Camera* _uiCamera;
	void _initCamera();
	
	EditorLayout *_layout;
	void _initLayout();

	EditorHierarchy *_hierarchy;
	void _initHierarchy();
	
	Scene *_scene;
	void _initScene();

protected:
	void _deserialized() override;
public:
	EditorMain(std::string name, Node *node, std::string uuid = "");
	void Awake() override;
	void Enable() override;

	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Disable() override;
	void destroy() override;
	~EditorMain();
};
REGISTER_COMPONENT(EditorMain,"Custom Editor Main Component")
