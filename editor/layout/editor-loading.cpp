#include "editor-loading.h"
#include "../boo-editor.h"
#include "../layout/editor-layout.h"
#include "../cache/editor-assets-cache.h"
#include "../cache/editor-config-cache.h"
#include "../cache/editor-project-cache.h"
#include "../../engine/boo.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/scene/node.h"
#include "../../engine/core/scene/node-2d.h"

EditorLoading::EditorLoading(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
}
void EditorLoading::Awake()
{
	std::cout << "EditorLoading::Awake" << std::endl;
	this->_logoRatio = 0.35f;
	this->_width = Boo::game->view()->width;
	this->_height = Boo::game->view()->height;
	this->_initBg();
	this->_initLogo();
	this->_initLoadUI();
	this->_initLoadingResources();
	for (auto &node : this->_node->getChildren())
	{
		std::cout << "EditorLoading::Awake: " << node->getName() << std::endl;
	}
}
void EditorLoading::Enable()
{
	Component::Enable();
}
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
		this->_spriteAlpha->setTexture("resources/texture/alpha.png");
		this->_spriteAlpha->setMaterial(nullptr);
		this->_spriteAlpha->setColor(0.2f, 0.0f, 1.0f, 1.0f);
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
		this->_spriteLogo->setTexture("resources/texture/logo.png");
		this->_spriteLogo->setMaterial(nullptr);
		this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	Texture *textureLogo = static_cast<Texture *>(Boo::game->assetsManager()->get("resources/texture/logo.png"));
	if (textureLogo != nullptr)
	{
		this->_logoTxWidth = textureLogo->width();
		this->_logoTxHeight = textureLogo->height();
	}
	// 初始化logo大小
	this->_updateLogoSize(this->_width, this->_height);
}
void EditorLoading::_initLoadUI()
{
	this->_ndLoad = new Node2D("Editor-Load");
	this->_node->addChild(this->_ndLoad);
	this->_spriteLoad = dynamic_cast<UISprite *>(this->_ndLoad->addComponent("UISprite"));
	if (this->_spriteLoad != nullptr)
	{
		this->_spriteLoad->setTexture("resources/texture/ic-default.png");
		this->_spriteLoad->setMaterial(nullptr);
		this->_spriteLoad->setColor("#0A2F36");
	}
	this->_ndLoadBar = new Node2D("Editor-LoadBar");
	this->_ndLoad->addChild(this->_ndLoadBar);
	this->_spriteLoadBar = dynamic_cast<UISprite *>(this->_ndLoadBar->addComponent("UISprite"));
	if (this->_spriteLoadBar != nullptr)
	{
		this->_spriteLoadBar->setTexture("resources/texture/ic-default.png");
		this->_spriteLoadBar->setMaterial(nullptr);
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
		this->_ndLoadBar->setPosition(-loadWidth / 2.0 + loadBarWidth / 2.0, 0.0f, 0.0f);
	}
}
void EditorLoading::_initLoadingResources()
{
	// 先加载公用resources文件,
	const std::string &root = Boo::game->assetsManager()->root();
	std::filesystem::path resources = std::filesystem::path(root) / "resources";
	std::vector<std::string> paths;
	for (const auto &entry : std::filesystem::recursive_directory_iterator(resources))
	{
		if (std::filesystem::is_regular_file(entry.path()))
		{
			std::filesystem::path path = std::filesystem::relative(entry.path(), std::filesystem::path(root));
			paths.push_back(path.generic_string());
			std::cout << "add resource " << path.generic_string() << std::endl;
		}
	}
	// 记载工程assets文件
	std::filesystem::path projectAssets = std::filesystem::path(BooEditor::projectPath) / "assets";
	for (const auto &entry : std::filesystem::recursive_directory_iterator(projectAssets))
	{
		if (std::filesystem::is_regular_file(entry.path()))
		{
			std::filesystem::path path = std::filesystem::relative(entry.path(), std::filesystem::path(root));
			paths.push_back(path.generic_string());
			std::cout << "add resource " << path.generic_string() << std::endl;
		}
	}
	this->_loadingResourcesTaskId = Boo::game->assetsManager()->loadListAsync(paths, &EditorLoading::_onLoadCallBack, this);
}
void EditorLoading::_onLoadCallBack(int completedCount, int allCount, float progress)
{
	this->_setLoadProgress(progress * 0.9f);
	if (completedCount == allCount)
	{
		// 加载完成
		Boo::game->assetsManager()->clearLoadCall(this->_loadingResourcesTaskId);
		this->_loadingResourcesTaskId = -1;
		// 初始化编辑器缓存
		this->_initEditorCache();
	}
}
void EditorLoading::_initEditorCache()
{
	// 项目setting配置
	//  初始化editor 配置缓存
	BooEditor::config->init();
	// 初始化assets 资源缓存
	BooEditor::assets->init();
	// 初始化project 项目配置
	BooEditor::project->init();
	this->_setLoadProgress(1.0f);
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
		std::cout << "EditorLoading::update() width: " << width << " height: " << height << std::endl;
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
void EditorLoading::Render()
{
	Component::Render();
}



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
		std::cout << "EditorLoading::_updateLogoSize() logoWidth: " << logoWidth << " logoHeight: " << logoHeight << std::endl;
	}
}
void EditorLoading::_updateLoadBarSize(float width, float height)
{
	float loadWidth = this->_width * 0.55;
	this->_ndLoad->setSize(loadWidth, 5.0f);
	this->_ndLoad->setPosition(0.0, -this->_height / 2 + this->_height / 2 * 0.4, 0.0f);
	if (this->_ndLoadBar != nullptr)
	{
		float loadBarWidth = loadWidth * this->_loadProgress;
		this->_ndLoadBar->setSize(loadBarWidth, 5.0f);
		this->_ndLoadBar->setPosition(-loadWidth / 2.0 + loadBarWidth / 2.0, 0.0f, 0.0f);
	}
}
void EditorLoading::Disable()
{
	Component::Disable();
}

void EditorLoading::destroy()
{
	Component::destroy();
	Boo::game->assetsManager()->clearLoadCall(this->_loadingResourcesTaskId);
	this->_loadingResourcesTaskId = -1;
	std::cout << "EditorLoading::destroy() loadingResourcesTaskId: " << this->_loadingResourcesTaskId << std::endl;
}
EditorLoading::~EditorLoading()
{
}
