#pragma once
#include "asset.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include "core/asset/texture-asset.h"

namespace Boo
{
    class TextureAsset;
    struct BMGlyphData
    {
        int id;
        int x;
        int y;
        int width;    // 位图宽度（像素）
        int height;   // 位图高度（像素）
        int xoffset;  // 在所在字体纹理中的水平偏移（从左到右）
        int yoffset;  // 在所在字体纹理中的垂直偏移（从上到下）
        int xadvance; // 绘制完此字符后，光标需要前进的距离
        int page;     // 字符所属的纹理页编号（从0开始）
        int chnl;     // 通道掩码（0-15）
    };
    struct BMFontPage
    {
        int id;
        std::string file;
        TextureAsset *texture = nullptr; // 纹理资源
    };
    class BMFontAsset : public Asset
    {
    private:
        int _fontSize;
        int _lineHeight;
        int _textureWidth;
        int _textureHeight;
        int _base;
        std::map<int, BMFontPage> _pageFiles;
        std::map<int, BMGlyphData> _glyphs; // 该页的所有字形
    private:
        // BMFontAtlas _fontAtlas;
        void _parseContent(std::string &bmFontContent);
        void _parseBMFontXML(std::string &bmFontContent);
        void _parseBMFont(std::string &bmFontContent);
        void _parseBMPageTexture();

    protected:
    public:
        BMFontAsset();
        BMFontAsset(std::string uuid);
        BMFontAsset(std::string uuid, std::string path, std::string name);
        /**
         * @brief 创建BMFont资产
         *
         * @param bmFontContent BMFont资产内容
         */
        void create(std::string bmFontContent);
        const int getFontSize();
        BMGlyphData getGlyphData(wchar_t ch);
        const float getCharWidth(wchar_t ch);
        const float getLineHeight();
        TextureAsset * getTexture();
        const int getTextureWidth();
        const int getTextureHeight();
        /**
         * @brief 销毁BMFont资产
         */
        void destroy() override;
        ~BMFontAsset();
    };

} // namespace Boo
