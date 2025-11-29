#pragma once
#include <vector>
#include <functional>
#include <filesystem>
#include "../../engine/core/component/component.h"
#include "../../engine/core/component/component-register.h"

class Node2D;
class UISprite;
class TextureAsset;

class EditorLoading : public Component
{
private:
	float _width;
	float _height;
	TextureAsset* _textureDefault;

	Node2D *_ndAlpha;
	UISprite *_spriteAlpha;
	void _initBg();
	void _updateBgSize(float width, float height);
	// logo
	Node2D *_ndLogo;
	UISprite *_spriteLogo;
	TextureAsset* _textureLogo;
	float _logoTxWidth;
	float _logoTxHeight;
	float _logoRatio;
	void _initLogo();
	void _updateLogoSize(float width, float height);

	Node2D *_ndLoad;
	UISprite *_spriteLoad;
	Node2D *_ndLoadBar;
	UISprite *_spriteLoadBar;
	float _loadProgress = 0.0f;
	void _initLoadUI();
	void _setLoadProgress(float progress);
	void _updateLoadBarSize(float width, float height);

	void _initAssetsDB();
	void _initAssetsDBCallback(int completedCount, int allCount, float progress);

	void _saveEditorCache();
	int _launchEditorTaskId = -1;
	void _launchEditor();
	std::function<void()> _onLoadComplete;
protected:
	void _deserialized() override;
public:
	EditorLoading(std::string name, Node *node, std::string uuid = "");
	/**
	 * @brief 设置加载完成回调
	 * 
	 * @param onLoadComplete 
	 */
	void setOnLoadComplete(std::function<void()> onLoadComplete);
	
	void Awake() override;
	void Enable() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Render() override;
	void Disable() override;
	void destroy() override;
	~EditorLoading();
};
REGISTER_COMPONENT(EditorLoading, "自定义编辑器加载界面")
