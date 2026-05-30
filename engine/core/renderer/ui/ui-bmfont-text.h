#pragma once
#include <string>
#include <iostream>
#include "ui-renderer.h"
#include "core/font/freetype-mgr.h"
#include "core/component/component-register.h"
#include "core/math/color.h"

namespace Boo
{

    class MaterialAsset;
    class TextureAsset;
    class MeshAsset;
    class Camera;
    class Size;
    class MeshAsset;
    class BMFontAsset;

    class UIBMFontText : public UIRenderer
    {

    private:
        std::wstring _text; // 记录当前设置的文本
        int _fontSize;
        int _lineHeight;
        Color _color;
        bool _isVertDirty=false;
        bool _isTextDirty=false;
        bool _isMBFontDirty=false;

    private:
        BMFontAsset *_bmFont;
        void _createBMFont();
        void _updateBMFont();

    private:
        MaterialAsset *_materialAsset;
        void _createMaterial();
        size_t _textLen;
        MeshAsset *_meshAsset;
        void _createMesh();

    public:
        UIBMFontText(std::string name, Node *node, std::string uuid = "");

        void OnAwake() override;
        void OnEnable() override;
        void setProperty(json &data) override;
        void setBMFont(std::string path);
        void setBMFont(BMFontAsset *bmFont);
        void setText(const std::string &text);
        void setText(const std::wstring &text);
        void setText(const wchar_t *text);
        const std::wstring &getText();
        void setSize(int fontSize);
        const int &getSize();
        void setLineHeight(int lineHeight);
        const int &getLineHeight();

        void setColor(Color &color);
        void setColor(std::string color);
        void setColor(float r, float g, float b, float a);
        void setAlpha(float alpha);
        void setMaterial(std::string material);
        void setMaterial(MaterialAsset *material);

        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void Render(Camera *camera) override;
        void OnDisable() override;
        void OnDestroy() override {};
        void destroy() override;
        ~UIBMFontText() override;
    };
    // 注册宏调用
    REGISTER_COMPONENT(UIBMFontText, "UI BM Font Text Component")
} // namespace Boo
