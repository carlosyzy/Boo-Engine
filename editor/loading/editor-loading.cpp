#include "editor-loading.h"
#include "../boo-editor.h"
#include "../layout/editor-layout.h"
// #include "../cache/editor-assets-cache.h"
// #include "../cache/editor-config-cache.h"
// #include "../cache/editor-project-cache.h"
// #include "../cache/editor-scene-cache.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/scene/node-2d.h"
#include "../../engine/core/scene/node.h"
#include "../../engine/core/scene/scene.h"

#include "../../engine/core/utils/file-util.h"
#include "../../engine/core/utils/json-util.h"

EditorLoading::EditorLoading(std::string name, Node *node, std::string uuid)
	: Component(name, node, uuid)
{
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功后，更新模块尺寸
 */
void EditorLoading::_deserialized() { Component::_deserialized(); }

void EditorLoading::Awake()
{
	std::cout << "EditorLoading::Awake" << std::endl;
	this->_logoRatio = 0.35f;
	this->_width = Boo::game->view()->width;
	this->_height = Boo::game->view()->height;
	this->_textureDefault = new TextureAsset("boo-default-texture");
	std::filesystem::path defaultPath = (std::filesystem::path(BooEditor::editorPath) / "res/alpha/default.png").generic_string();
	this->_textureDefault->create(defaultPath.string());
	this->_textureLogo = new TextureAsset("boo-logo-texture");
	std::filesystem::path logoPath = (std::filesystem::path(BooEditor::editorPath) / "res/alpha/logo.png").generic_string();
	this->_textureLogo->create(logoPath.string());
	this->_logoTxWidth = this->_textureLogo->width();
	this->_logoTxHeight = this->_textureLogo->height();

	this->_initBg();
	this->_initLogo();
	this->_initLoadUI();
	this->_initAssetsDB();
}
void EditorLoading::Enable() { Component::Enable(); }
void EditorLoading::setOnLoadComplete(std::function<void()> onLoadComplete)
{
	this->_onLoadComplete = onLoadComplete;
}

void EditorLoading::_initBg()
{
	this->_ndAlpha = new Node2D("Editor-EditorLoading");
	this->_node->addChild(this->_ndAlpha);
	// 背景
	Component *compAlpha = this->_ndAlpha->addComponent("UISprite");
	if (compAlpha != nullptr)
	{
		this->_spriteAlpha = dynamic_cast<UISprite *>(compAlpha);
		this->_spriteAlpha->setTextureAsset(this->_textureDefault);
		this->_spriteAlpha->setMaterialAsset(nullptr);
		this->_spriteAlpha->setColor("#000000ff");
	}
	this->_ndAlpha->setSize(this->_width, this->_height);
}
void EditorLoading::_initLogo()
{
	// 添加logo
	this->_ndLogo = new Node2D("Editor-EditorLoading-Logo");
	this->_node->addChild(this->_ndLogo);
	this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
	Component *compLogo = this->_ndLogo->addComponent("UISprite");
	if (compLogo != nullptr)
	{
		this->_spriteLogo = dynamic_cast<UISprite *>(compLogo);
		this->_spriteLogo->setEnabled(true);
		this->_spriteLogo->setTextureAsset(this->_textureLogo);
		this->_spriteLogo->setMaterialAsset(nullptr);
		this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// 初始化logo大小
	this->_updateLogoSize(this->_width, this->_height);
}
void EditorLoading::_initLoadUI()
{
	this->_ndLoad = new Node2D("Editor-Load");
	this->_node->addChild(this->_ndLoad);
	this->_spriteLoad =
		dynamic_cast<UISprite *>(this->_ndLoad->addComponent("UISprite"));
	if (this->_spriteLoad != nullptr)
	{
		this->_spriteLoad->setTextureAsset(this->_textureDefault);
		this->_spriteLoad->setMaterialAsset(nullptr);
		this->_spriteLoad->setColor("#0A2F36");
	}
	this->_ndLoadBar = new Node2D("Editor-LoadBar");
	this->_ndLoad->addChild(this->_ndLoadBar);
	this->_spriteLoadBar = dynamic_cast<UISprite *>(this->_ndLoadBar->addComponent("UISprite"));
	if (this->_spriteLoadBar != nullptr)
	{
		this->_spriteLoadBar->setTextureAsset(this->_textureDefault);
		this->_spriteLoadBar->setMaterialAsset(nullptr);
		this->_spriteLoadBar->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		this->_spriteLoadBar->setColor("#AFF2FF");
	}
	this->_setLoadProgress(0.0f);
	this->_updateLoadBarSize(this->_width, this->_height);
}
void EditorLoading::_setLoadProgress(float progress)
{
	this->_loadProgress = progress;
	if (this->_ndLoad != nullptr && this->_ndLoadBar != nullptr)
	{
		float loadWidth = this->_ndLoad->getSize().getWidth();
		float loadBarWidth = loadWidth * this->_loadProgress;
		this->_ndLoadBar->setSize(loadBarWidth, 5.0f);
		this->_ndLoadBar->setPosition(-loadWidth / 2.0 + loadBarWidth / 2.0, 0.0f,
									  0.0f);
	}
}
void EditorLoading::_initAssetsDB()
{
	BooEditor::cache->initAssetsDBMaps(&EditorLoading::_initAssetsDBCallback, this);
}
void EditorLoading::_initAssetsDBCallback(int completedCount, int allCount,
										  float progress)
{
	std::cout << "EditorLoading::_initAssetsDBCallback: " << completedCount << " " << allCount << " " << progress << std::endl;
	this->_setLoadProgress(progress * 0.9f);
	if (completedCount == allCount)
	{
		// 加载完成
		this->_saveEditorCache();
	}
}
void EditorLoading::_saveEditorCache()
{
	std::cout << "EditorLoading::_saveEditorCache" << std::endl;
	// 保存资产数据库
	BooEditor::cache->saveAssetsDB();
	// 加载 res/private 下边的所有资产
	std::filesystem::path privatePath = (std::filesystem::path(BooEditor::editorPath) / "res/private").generic_string();
	for (const auto &entry : std::filesystem::recursive_directory_iterator(privatePath))
	{
		if (std::filesystem::is_regular_file(entry))
		{
			std::string relativePath = std::filesystem::relative(entry.path(), privatePath).generic_string();
			TextureAsset *texture = new TextureAsset(relativePath);
			texture->create(entry.path().string());
			BooEditor::cache->addEditorTexture(relativePath, texture);
		}
	}
	this->_setLoadProgress(1.0f);
	this->_launchEditorTaskId = Boo::game->scheduleOnce(&EditorLoading::_launchEditor, this, 0.1f);
}
void EditorLoading::_launchEditor()
{
	Boo::game->unschedule(this->_launchEditorTaskId);
	this->_launchEditorTaskId = -1;
	if (this->_onLoadComplete != nullptr)
	{
		this->_onLoadComplete();
	}
}

void EditorLoading::Update(float deltaTime)
{
	Component::Update(deltaTime);
	float width = Boo::game->view()->width;
	float height = Boo::game->view()->height;
	if (this->_width != width || this->_height != height)
	{
		this->_width = width;
		this->_height = height;
		std::cout << "EditorLoading::update() width: " << width
				  << " height: " << height << std::endl;
		// Update logo size
		this->_updateBgSize(width, height);
		this->_updateLogoSize(width, height);
		this->_updateLoadBarSize(width, height);
	}
}
void EditorLoading::LateUpdate(float deltaTime)
{
	Component::LateUpdate(deltaTime);
}
void EditorLoading::Render() { Component::Render(); }

void EditorLoading::_updateBgSize(float width, float height)
{
	if (this->_ndAlpha)
	{
		this->_ndAlpha->setSize(width, height);
	}
}
void EditorLoading::_updateLogoSize(float width, float height)
{
	if (this->_logoTxWidth == 0.0f || this->_logoTxHeight == 0.0f)
	{
		return;
	}
	float logoWidth = width * this->_logoRatio;
	float logoHeight = logoWidth * this->_logoTxHeight / this->_logoTxWidth;
	if (this->_ndLogo)
	{
		this->_ndLogo->setSize(logoWidth, logoHeight);
		std::cout << "EditorLoading::_updateLogoSize() logoWidth: " << logoWidth
				  << " logoHeight: " << logoHeight << std::endl;
	}
}
void EditorLoading::_updateLoadBarSize(float width, float height)
{
	float loadWidth = this->_width * 0.55;
	this->_ndLoad->setSize(loadWidth, 5.0f);
	this->_ndLoad->setPosition(0.0, -this->_height / 2 + this->_height / 2 * 0.4,
							   0.0f);
	if (this->_ndLoadBar != nullptr)
	{
		float loadBarWidth = loadWidth * this->_loadProgress;
		this->_ndLoadBar->setSize(loadBarWidth, 5.0f);
		this->_ndLoadBar->setPosition(-loadWidth / 2.0 + loadBarWidth / 2.0, 0.0f,
									  0.0f);
	}
}
void EditorLoading::Disable() { Component::Disable(); }

void EditorLoading::destroy()
{
	Component::destroy();
	Boo::game->unschedule(this->_launchEditorTaskId);
	this->_launchEditorTaskId = -1;
}
EditorLoading::~EditorLoading() {}
