#pragma once
#include <vector>

#include "../../engine/core/component/component.h"

class Node2D;
class UISprite;
class Texture;

class EditorLoading : public Component
{
private:
	float _width;
	float _height;
	Node2D *_ndAlpha;
	UISprite *_spriteAlpha;
	void _initBg();
	void _updateBgSize(float width, float height);
	// logo
	Node2D *_ndLogo;
	UISprite *_spriteLogo;
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

	/**
	 * @brief 初始化加载资源
	 * 
	 */
	void _initLoadingResources();
	int _loadingResourcesTaskId = -1;
	void _onLoadCallBack(int completedCount, int allCount, float progress);
	/**
	 * @brief 初始化编辑器缓存
	 */
	void _initEditorCache();

	std::function<void()> _onLoadComplete;

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
