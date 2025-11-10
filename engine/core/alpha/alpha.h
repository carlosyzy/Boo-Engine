#pragma once
#include <vector>
#include "../scene/scene.h"

class Node2D;
class UISprite;

class Alpha :public Scene
{
private:
    Node2D* _ndAlpha;
	Node2D* _ndLogo;
	UISprite* _spriteLogo;
	int _frame;
	float _logoAlphaNum;
	void _init();
	void _initRes();
	void _initAlpha();
	void _updateLogoAlpha();
public:
    Alpha(const std::string name, const std::string uuid="");
	void update(float deltaTime) override;
	void destroy() override;
    ~Alpha();
};

