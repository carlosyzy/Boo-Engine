#pragma once
#include <vector>
#include "../../../engine/core/scene/scene.h"

class Node2D;
class UISprite;
class Texture;

class EditorLoading :public Scene
{
private:
	float _width;
	float _height;
    Node2D* _ndAlpha;
	UISprite* _spriteAlpha;
	void _initBg();
	// logo
	Node2D* _ndLogo;
	UISprite* _spriteLogo;
	float _logoTxWidth;
	float _logoTxHeight;
	float _logoRatio;

	void _initLogo();
	void _updateBgSize(float width, float height);
	void _updateLogoSize(float width, float height);
public:
    EditorLoading(const std::string name, const std::string uuid="");
	void update(float deltaTime) override;
	void destroy() override;
    ~EditorLoading();
};

