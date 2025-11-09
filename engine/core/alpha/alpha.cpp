#include "alpha.h"
#include "../game.h"
#include "../scene/scene.h"
#include "../scene/node-2d.h"
#include "../renderer/ui/ui-sprite.h"
#include "../assets/assets-manager.h"
#include "../assets/asset.h"
#include "../assets/Texture.h"

Alpha::Alpha(const std::string name, const std::string uuid) : Scene(name, uuid), _frame(0), _logoAlphaNum(0.0f)
{
}
void Alpha::update(float deltaTime)
{
    if (this->_frame == 0)
    {
        this->_frame++;
    }
    else if (this->_frame == 1)
    {
        this->_frame++;
        this->_init();
    }
    else if (this->_frame == 2)
    {
        this->_frame++;
    }
    else
    {
        this->_updateLogoAlpha();
    }
}

void Alpha::_init()
{
    this->_initRes();
    this->_initAlpha();
}
void Alpha::_initRes()
{
    Game::getInstance()->assetsManager()->load("resources/texture/logo.png");
    Game::getInstance()->assetsManager()->load("resources/texture/ic-default.png");
    Game::getInstance()->assetsManager()->load("resources/shader/ui/ui.vert");
    Game::getInstance()->assetsManager()->load("resources/shader/ui/ui.frag");
}

void Alpha::_initAlpha()
{
    this->_ndAlpha = new Node2D("Editor-Alpha");
    this->_root2D->addChild(this->_ndAlpha);
    // 添加logo
    this->_ndLogo = new Node2D("Editor-Alpha-Logo");
    this->_ndAlpha->addChild(this->_ndLogo);
    this->_ndLogo->setSize(550 * 0.7, 400 * 0.7);
    this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
    Component *compLogo = this->_ndLogo->addComponent("UISprite");
    if (compLogo != nullptr)
    {
        this->_spriteLogo = static_cast<UISprite *>(compLogo);
        Asset *tex = Game::getInstance()->assetsManager()->get("resources/texture/logo.png");
        this->_spriteLogo->setTexture(static_cast<Texture *>(tex));
        this->_spriteLogo->setMaterial(nullptr);
        this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 0.0f);
    }
}

void Alpha::_updateLogoAlpha()
{
    if (this->_spriteLogo == nullptr)
    {
        return;
    }
    if (this->_logoAlphaNum > 1.0)
    {
        return;
    }
    this->_logoAlphaNum += 0.02;
    std::cout << "this->_logoAlphaNum: " << this->_logoAlphaNum << std::endl;
    this->_spriteLogo->setAlpha(this->_logoAlphaNum);
}
// void Alpha::_loadResources()
// {
//     const std::string &root = Game::getInstance()->assetsManager()->root();
//     std::filesystem::path fullPath = std::filesystem::path(root) / "resources";
//     std::vector<std::string> paths;
//     for (const auto &entry : std::filesystem::recursive_directory_iterator(fullPath))
//     {
//         std::filesystem::path path = std::filesystem::relative(entry.path(), std::filesystem::path(root));
//         paths.push_back(path.generic_string());
//     }
// }

void Alpha::destroy()
{
}
Alpha::~Alpha()
{
}
