#include "editor.h"
#include "../engine/engine.h"
#include "../engine/boo.h"
#include "../engine/core/game.h"
#include "../engine/core/assets/assets-manager.h"
#include "editor-layout.h"
#include "../engine/core/utils/time-util.h"
#include "hierarchy/editor-hierarchy.h"
#include "assets/editor-assets.h"
#include "property/editor-property.h"

Editor::Editor() : _editorLayout(nullptr)
{
}
Editor::~Editor()
{
}
Editor *Editor::getInstance()
{
	static Editor instance;
	return &instance;
}

void Editor::init()
{
	this->_initLayout();
	this->_initRes();
}
void Editor::_initLayout()
{
	this->_editorLayout = new EditorLayout();
	// // Editor:: : 表示 Editor 类的作用域
	// // _onAlphaAnimOK : 成员函数名
	// // & : 取地址运算符，获取成员函数的地址
	// // 成员函数指针的正确格式是 &ClassName::MemberFunction
	// // &this->member 语法用于获取成员变量的地址，不适用于成员函数
	this->_scheduleID_AlphaAnim = Boo::game->scheduleOnce(&Editor::_onAlphaAnimOK, this, 2.0f);
}
void Editor::_onAlphaAnimOK()
{
	this->_alphaAnimOK = true;
	if (this->_loadComplete)
	{
		this->_launchEditor();
	}
}

void Editor::_initRes()
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
	// 将在工程
	Boo::game->assetsManager()->loadListAsync(paths, &Editor::_onLoadCallBack, this);
}
void Editor::_onLoadCallBack(const int complete, const int all, const float progress)
{
	this->_editorLayout->setLoadProgress(progress);
	if (complete == all)
	{
		this->_loadComplete = true;
		if (this->_alphaAnimOK)
		{
			this->_launchEditor();
		}
	}
}
// 启动编辑器
void Editor::_launchEditor()
{
	std::cout << "launch editor" << std::endl;
	Boo::game->assetsManager()->load("res/ic-title-hierarchy.png");
	Boo::game->assetsManager()->load("res/ic-title-assets.png");
	Boo::game->assetsManager()->load("res/ic-title-property.png");
	Boo::game->assetsManager()->load("res/ic-title-scene.png");


	this->_editorLayout->launch();
	// this->_initHierarchy();
	// this->_initAssets();
	// this->_initProperty();

	/*  EditorIpc::getInstance()->on(IpcEvent::UPDATE_HIERARCHY_ROOT, &Editor::_onHierarchyRootUpdate, this);
	  EditorIpc::getInstance()->send(IpcEvent::UPDATE_HIERARCHY_ROOT,0);*/
}
void Editor::_initHierarchy()
{
	Node2D *layout = this->_editorLayout->getHierarchy();
	this->_hierarchy = new EditorHierarchy(layout);
	this->_hierarchy->setScene(this->_editorLayout->getScene());
}
// void Editor::_initAssets()
// {
//     Node2D *layout = this->_editorLayout->getAssets();
//     this->_assets = new EditorAssets(layout);
// }
// void Editor::_initProperty()
// {
//     Node2D *layout = this->_editorLayout->getProperty();
//     this->_property = new EditorProperty(layout);
// }
// void Editor::_onHierarchyRootUpdate(const EventValue &value)
//{
//     if (this->_hierarchy != nullptr)
//     {
//         std::cout << "EditorHierarchy Update Tree" << std::endl;
//         this->_hierarchy->updateRoot();
//     }
// }
// void Editor::setHierarchyRoot(Scene *scene)
//{
//     if (this->_hierarchy != nullptr)
//     {
//         this->_hierarchy->setScene(scene);
//     }
// }
// void Editor::setHierarchyNode(Node *node)
//{
//     if (this->_hierarchy != nullptr)
//     {
//         this->_hierarchy->setNode(node);
//     }
// }
// void Editor::setAssetsRoot(std::string root)
//{
//     if (this->_assets != nullptr)
//     {
//         this->_assets->setRoot(root);
//     }
// }

void Editor::update(float dt)
{
	if (this->_editorLayout != nullptr)
	{
		this->_editorLayout->update(dt);
	}
	this->_hierarchy->update(dt);
	// this->_assets->update(dt);
	// this->_property->update(dt);*/
}

void Editor::destroy()
{
}