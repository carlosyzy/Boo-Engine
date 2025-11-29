#pragma once
#include <vector>
#include "../scene/scene.h"
/**
 * @brief _alpha 场景
 *  引擎启动界面，不可取消，但是可以修改背景颜色和logo以及显示时间
 */
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
	TextureAsset *_textureBg;
	// logo
	Node2D *_ndLogo;
	UISprite *_spriteLogo;
	TextureAsset *_textureLogo;
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
