#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cwctype>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "core/math/vec2.h"

#define FREETYPE_DEBUG_OUT 0

namespace Boo
{
    class TextureAsset;
    class BMFontAsset;
    class UIBMFontText;

    struct BMFontProperty
    {
        int fontSize = 0;
        int lineHeight = 0;
        float allWidth = 0.0f;
        float allHeight = 0.0f;
        float scale = 0.0f;
        float unitX = 0.0f;
        float unitY = 0.0f;
        std::vector<std::wstring> lines;
        std::vector<float> positions;
        std::vector<float> uvs;
        std::vector<uint32_t> indices;
    };

    struct BMFontLayoutResult
    {
        uint32_t width = 0;
        uint32_t height = 0;
        std::vector<float> positions = {};
        std::vector<float> uvs = {};
        std::vector<uint32_t> indices = {};
        TextureAsset *texture = nullptr;
    };

    class BMFontManager
    {
    private:
        BMFontManager(const BMFontManager &) = delete;
        BMFontManager &operator=(const BMFontManager &) = delete;
        BMFontManager() {};
        ~BMFontManager() {};
    private:
        void _parse(const std::wstring &text, const int fontSize, const int lineHeight, BMFontAsset *font, BMFontLayoutResult &layoutResult);
        std::vector<std::wstring> _splitByNewline(const std::wstring &text);
        float _parseLineWidth(BMFontAsset *font, const std::wstring &line);
        void _generateFontMesh(BMFontAsset *font, BMFontProperty &property);
        void _generateLineMesh(BMFontAsset *font, BMFontProperty &property, int lineIndex);

    public:
        static BMFontManager *getInstance();
        void create(BMFontAsset *font, UIBMFontText *uiBMFontText, BMFontLayoutResult &layoutResult);
        void create(const std::wstring &text, int fontSize, int lineHeight, BMFontAsset *font, BMFontLayoutResult &layoutResult);
    };

} // namespace Boo