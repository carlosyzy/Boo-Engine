#include "alpha.h"
#include "../../boo.h"
#include "../assets/assets-manager.h"
#include "../renderer/ui/ui-sprite.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/gfx-pipeline-struct.h"

Alpha::Alpha(const std::string name, const std::string uuid) : Scene(name, uuid), _logoAlphaNum(0.0f)
{
	this->_alphaDuration = 2.0f;
	this->_logoAlphaNum = 0.0f;
	this->_logoRatio = 0.35f;

	// 创建Pass是在摄像机初始化的时候
	// 创建Pipeline应该是渲染物体第一次调用的时候
	GfxPipelineStruct uiPipelineStruct = {};
	uiPipelineStruct.name = "ui";
	uiPipelineStruct.vert = std::filesystem::path("resources/shader/ui/ui.vert.spv").generic_string();
	uiPipelineStruct.frag = std::filesystem::path("resources/shader/ui/ui.frag.spv").generic_string();
	uiPipelineStruct.pass = "ui";
	uiPipelineStruct.depthTest = 0;
	uiPipelineStruct.depthWrite = 0;
	uiPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
	// 模版测试 关闭
	uiPipelineStruct.stencilTest = 0;
	// 颜色混合 开启
	uiPipelineStruct.colorBlend = 1;
	uiPipelineStruct.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
	uiPipelineStruct.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
	uiPipelineStruct.colorBlendOp = GfxPipelineColorBlendOp::Add;
	uiPipelineStruct.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
	uiPipelineStruct.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
	uiPipelineStruct.alphaBlendOp = GfxPipelineColorBlendOp::Add;
	uiPipelineStruct.colorWriteMask = 4;
	// 多边形模式 填充
	uiPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
	// 剔除模式 背面
	uiPipelineStruct.cullMode = GfxPipelineCullMode::Back;
	GfxMgr::getInstance()->createPipeline("ui.mtl", uiPipelineStruct);

	// 模式ui 遮罩 模式为Fill 时 启用cullMode 为Back
	GfxPipelineStruct uiMaskPipelineStruct = {};
	uiMaskPipelineStruct.name = "ui-mask";
	uiMaskPipelineStruct.vert = std::filesystem::path("resources/shader/ui/ui-mask.vert.spv").generic_string();
	uiMaskPipelineStruct.frag = std::filesystem::path("resources/shader/ui/ui-mask.frag.spv").generic_string();
	uiMaskPipelineStruct.pass = "ui";
	uiMaskPipelineStruct.depthTest = 0;
	uiMaskPipelineStruct.depthWrite = 0;
	uiMaskPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
	// 模版测试 关闭
	uiMaskPipelineStruct.stencilTest = 1;
	//ui 遮罩,正面和背面保持一致
	uiMaskPipelineStruct.stencilFrontCompareOp = GfxPipelineCompareOp::Always;
	uiMaskPipelineStruct.stencilFrontFailOp = GfxPipelineStencilOp::Keep;
	uiMaskPipelineStruct.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
	uiMaskPipelineStruct.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add;
	uiMaskPipelineStruct.stencilBackCompareOp = GfxPipelineCompareOp::Always;
	uiMaskPipelineStruct.stencilBackFailOp = GfxPipelineStencilOp::Keep;
	uiMaskPipelineStruct.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
	uiMaskPipelineStruct.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;// Decrement_Subtract;

	// 颜色混合 开启
	uiMaskPipelineStruct.colorBlend = 0;
	// 多边形模式 填充
	uiMaskPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
	// 剔除模式 背面
	uiMaskPipelineStruct.cullMode = GfxPipelineCullMode::Back;
	GfxMgr::getInstance()->createPipeline("ui-mask.mtl", uiMaskPipelineStruct);

	this->_init();
}

void Alpha::_init()
{
	this->_initRes();
	this->_initAlpha();
	this->_initDelayScheduleID = Boo::game->scheduleOnce(&Alpha::_onAlphaAnimOK, this, this->_alphaDuration / 2.0);
}
void Alpha::_initRes()
{
	Texture *textureLogo = static_cast<Texture *>(Boo::game->assetsManager()->get("resources/texture/logo.png"));
	if (textureLogo != nullptr)
	{
		this->_logoTxWidth = textureLogo->width();
		this->_logoTxHeight = textureLogo->height();
	}
}
void Alpha::_initAlpha()
{
	std::cout << "Alpha::_initAlpha()" << std::endl;
	this->_ndAlpha = new Node2D("Editor-Alpha");
	this->_root2D->addChild(this->_ndAlpha);
	// 添加logo
	this->_ndLogo = new Node2D("Editor-Alpha-Logo");
	this->_ndAlpha->addChild(this->_ndLogo);
	this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
	Component *compLogo = this->_ndLogo->addComponent("UISprite");
	if (compLogo != nullptr)
	{
		this->_spriteLogo = dynamic_cast<UISprite *>(compLogo);
		// Asset *tex = Boo::game->assetsManager()->get("resources/texture/logo.png");
		this->_spriteLogo->setTexture("resources/texture/logo.png");
		this->_spriteLogo->setMaterial(nullptr);
		this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	this->_updateLogoSize();
}

void Alpha::_onAlphaAnimOK()
{
}

void Alpha::update(float deltaTime)
{
	Scene::update(deltaTime);
	// Update logo alpha
	this->_updateLogoAlpha(deltaTime);
	// Update logo size
	this->_updateLogoSize();
}

void Alpha::_updateLogoAlpha(float deltaTime)
{
	if (this->_spriteLogo == nullptr)
	{
		return;
	}
	if (this->_logoAlphaNum > this->_alphaDuration)
	{
		return;
	}
	return;
	this->_logoAlphaNum += deltaTime;
	this->_spriteLogo->setAlpha(this->_logoAlphaNum / this->_alphaDuration);
}
void Alpha::_updateLogoSize()
{
	if (this->_spriteLogo == nullptr)
	{
		return;
	}
	if (this->_logoTxWidth == 0.0f || this->_logoTxHeight == 0.0f)
	{
		return;
	}
	float _width = Boo::game->view()->width;
	float _height = Boo::game->view()->height;
	if (this->_width == _width && this->_height == _height)
	{
		return;
	}
	this->_width = _width;
	this->_height = _height;
	float width = this->_width * this->_logoRatio;
	float height = width * this->_logoTxHeight / this->_logoTxWidth;
	this->_ndLogo->setSize(width, height);
}

void Alpha::destroy()
{
	Scene::destroy();
	Boo::game->unschedule(this->_initDelayScheduleID);
}
Alpha::~Alpha()
{
}
