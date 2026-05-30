#pragma once
#include <vector>
#include "../scene/scene.h"
/**
 * @brief _alpha 场景
 *  引擎启动界面，不可取消，但是可以修改背景颜色和logo以及显示时间
 */
namespace Boo
{
	class Camera;
	class Node2D;
	class UISprite;
	class TextureAsset;

	class Alpha : public Scene
	{
	private:
		void _initCamera();
		Camera *_uiCamera;

		float _width;
		float _height;

		Node2D *_ndAlpha;
		UISprite *_spriteAlpha;
		// logo splash
		Node2D *_ndSplash;
		UISprite *_spriteSplash;
		float _alphaDuration;
		float _splashAlphaNum;
		bool _isAlphaEnd = false;
		void _init();
		void _initAlpha();
		void _initAlphaBg();
		void _initAlphaSplash();

		void _updateLogoAlpha(float deltaTime);
	public:
		Alpha(const std::string name, const std::string uuid = "");
		void update(float deltaTime) override;
		void destroy() override;
		~Alpha();
	};

} // namespace Boo
