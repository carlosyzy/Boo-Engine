#pragma once
#include <vector>
#include "../scene/scene.h"

class Node2D;
class UISprite;
class TextureAsset;

class Alpha : public Scene
{
private:
	float _width;
	float _height;

	Node2D *_ndAlpha;
	UISprite *_spriteAlpha;

	// logo
	Node2D *_ndLogo;
	UISprite *_spriteLogo;
	float _logoTxWidth;
	float _logoTxHeight;
	float _alphaDuration;
	float _logoAlphaNum;
	float _logoRatio;


	void _init();
	void _initRes();
	void _initAlpha();
	void _updateLogoAlpha(float deltaTime);
	void _updateBgSize(float width, float height);
	void _updateLogoSize(float width, float height);

public:
	Alpha(const std::string name, const std::string uuid = "");

	void update(float deltaTime) override;
	void destroy() override;
	~Alpha();
};
