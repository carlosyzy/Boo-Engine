#pragma once
#include "../scene/scene.h"

class Node2D;
class UISprite;

class Alpha :public Scene
{
private:
    Node2D* _ndAlpha;
	UISprite* _spriteAlpha;
	Node2D* _ndLogo;
	UISprite* _spriteLogo;
	void _initAlpha();
public:
    Alpha(const std::string name, const std::string uuid="");
    ~Alpha();
};

