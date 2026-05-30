#include "bmfont-manager.h"
#include "freetype-mgr.h"
#include "log.h"
#include "platforms/platform.h"
#include "core/gfx/gfx-manager.h"
#include "core/util/uuid-util.h"
#include "core/util/fmt-util.h"
#include "core/asset/asset-manager.h"
#include "core/asset/texture-asset.h"
#include "core/asset/bmfont-asset.h"
#include "core/renderer/ui/ui-bmfont-text.h"

namespace Boo
{
    BMFontManager *BMFontManager::getInstance()
    {
        static BMFontManager _instance;
        return &_instance;
    }
    void BMFontManager::create(BMFontAsset *font, UIBMFontText *uiBMFontText, BMFontLayoutResult &layoutResult)
    {
        layoutResult.width = 0;
        layoutResult.height = 0;
        layoutResult.positions.clear();
        layoutResult.uvs.clear();
        layoutResult.indices.clear();
        if (font == nullptr || uiBMFontText == nullptr)
        {
            return;
        }
        const std::wstring &text = uiBMFontText->getText();
        const int &lineHeight = uiBMFontText->getLineHeight();
        const int &fontSize = uiBMFontText->getSize();
        this->_parse(text, fontSize, lineHeight, font, layoutResult);
    }
    void BMFontManager::create(const std::wstring &text, int fontSize, int lineHeight, BMFontAsset *font, BMFontLayoutResult &layoutResult)
    {
        layoutResult.width = 0;
        layoutResult.height = 0;
        layoutResult.positions.clear();
        layoutResult.uvs.clear();
        layoutResult.indices.clear();
        if (font == nullptr)
        {
            return;
        }

        this->_parse(text, fontSize, lineHeight, font, layoutResult);
    }
    void BMFontManager::_parse(const std::wstring &text, const int fontSize, const int lineHeight, BMFontAsset *font, BMFontLayoutResult &layoutResult)
    {
        if (text.empty())
        {
            return;
        }
        if (fontSize <= 0)
        {
            return;
        }
        size_t textLen = text.length();
        BMFontProperty property = {};
        property.fontSize = fontSize;
        property.lineHeight = lineHeight;
        property.scale = 1.0f;
        property.allWidth = 1.0f;
        property.allHeight = 1.0f;
        property.positions.reserve(textLen * 3 * 6);
        property.uvs.reserve(textLen * 2 * 6);
        property.indices.reserve(textLen * 6);

        property.lines = this->_splitByNewline(text);
        for (auto &line : property.lines)
        {
            property.allWidth = std::max(property.allWidth, this->_parseLineWidth(font, line));
        }
        property.scale = 1.0f * fontSize / font->getFontSize();
        property.allWidth *= property.scale;
        property.allHeight = std::max(lineHeight, fontSize) + ((property.lines.size() - 1) * lineHeight);

        this->_generateFontMesh(font, property);
        layoutResult.width = property.allWidth;
        layoutResult.height = property.allHeight;
        layoutResult.positions = property.positions;
        layoutResult.uvs = property.uvs;
        layoutResult.indices = property.indices;
        layoutResult.texture = font->getTexture();
    }
    std::vector<std::wstring> BMFontManager::_splitByNewline(const std::wstring &text)
    {
        std::vector<std::wstring> lines;
        std::wstringstream ss(text);
        std::wstring line;

        while (std::getline(ss, line))
        {
            lines.push_back(line);
        }

        if (!text.empty() && text.back() == L'\n')
        {
            lines.push_back(L"");
        }
        return lines;
    }
    float BMFontManager::_parseLineWidth(BMFontAsset *font, const std::wstring &line)
    {
        if (font == nullptr)
        {
            return 0.0f;
        }
        float width = 0.0f;
        for (auto &ch : line)
        {
            if (ch == L' ')
            {
                width += font->getFontSize() / 3.0f;
            }
            else
            {
                width += font->getCharWidth(ch);
            }
        }
        return width;
    }
    void BMFontManager::_generateFontMesh(BMFontAsset *font, BMFontProperty &property)
    {
        if (font == nullptr)
        {
            return;
        }
        property.unitX = 1.0f / property.allWidth;
        property.unitY = 1.0f / property.allHeight;
        for (size_t i = 0; i < property.lines.size(); i++)
        {
            this->_generateLineMesh(font, property, i);
        }
    }
    void BMFontManager::_generateLineMesh(BMFontAsset *font, BMFontProperty &property, int lineIndex)
    {
        const int &textureWidth = font->getTextureWidth();
        const int &textureHeight = font->getTextureHeight();

        std::wstring &line = property.lines[lineIndex];
        float startX = 0.0f;
        float startY = 0.0f;
        float lineWidth = this->_parseLineWidth(font, line) * property.scale;
        startX = -1 * lineWidth / 2.0f;
        startY = 1 * property.allHeight / 2.0f - lineIndex * property.lineHeight;
        if (property.lineHeight > property.fontSize)
        {
            startY = startY - (property.lineHeight - property.fontSize) / 2.0f;
        }

        for (auto &ch : line)
        {
            BMGlyphData glyph = font->getGlyphData(ch);
            if (glyph.id == 0)
            {
                continue;
            }
            if (ch == L' ')
            {
                startX += font->getFontSize() * property.scale / 3.0f;
                continue;
            }
            float x = startX + glyph.xoffset * property.scale;
            float y = startY - glyph.yoffset * property.scale;
            float width = 1.0f * glyph.width * property.scale;
            float height = 1.0f * glyph.height * property.scale;
            float uv_x = 1.0f * glyph.x / textureWidth;
            float uv_y = 1.0f * glyph.y / textureHeight;
            float uv_w = 1.0f * glyph.width / textureWidth;
            float uv_h = 1.0f * glyph.height / textureHeight;

            int indices_offset = property.positions.size() / 3;

            property.positions.push_back(x * property.unitX);
            property.positions.push_back(y * property.unitY);
            property.positions.push_back(0.0f);
            property.uvs.push_back(uv_x);
            property.uvs.push_back(uv_y);

            property.positions.push_back(x * property.unitX);
            property.positions.push_back((y - height) * property.unitY);
            property.positions.push_back(0.0f);
            property.uvs.push_back(uv_x);
            property.uvs.push_back(uv_y + uv_h);

            property.positions.push_back((x + width) * property.unitX);
            property.positions.push_back((y - height) * property.unitY);
            property.positions.push_back(0.0f);
            property.uvs.push_back(uv_x + uv_w);
            property.uvs.push_back(uv_y + uv_h);

            property.positions.push_back(x * property.unitX);
            property.positions.push_back(y * property.unitY);
            property.positions.push_back(0.0f);
            property.uvs.push_back(uv_x);
            property.uvs.push_back(uv_y);

            property.positions.push_back((x + width) * property.unitX);
            property.positions.push_back((y - height) * property.unitY);
            property.positions.push_back(0.0f);
            property.uvs.push_back(uv_x + uv_w);
            property.uvs.push_back(uv_y + uv_h);

            property.positions.push_back((x + width) * property.unitX);
            property.positions.push_back(y * property.unitY);
            property.positions.push_back(0.0f);
            property.uvs.push_back(uv_x + uv_w);
            property.uvs.push_back(uv_y);

            property.indices.push_back(indices_offset);
            property.indices.push_back(indices_offset + 1);
            property.indices.push_back(indices_offset + 2);
            property.indices.push_back(indices_offset + 3);
            property.indices.push_back(indices_offset + 4);
            property.indices.push_back(indices_offset + 5);

            startX += 1.0f * glyph.xadvance * property.scale;
        }
    }

} // namespace Boo