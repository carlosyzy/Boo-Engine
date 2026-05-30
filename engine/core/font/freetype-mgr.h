// #pragma once
// #include <string>
// #include <vector>
// #include <map>
// #include <cstdint>
// #include <codecvt>
// #include <locale>
// #include <algorithm>
// #include <iostream>
// #include <sstream>
// #include <cwctype>
// #include <ft2build.h>
// #include FT_FREETYPE_H
// #include "core/math/vec2.h"

// // 调试用查看字体图集页数据
// #define FREETYPE_DEBUG_OUT 0

// namespace Boo
// {
//     class TextureAsset;
// }

// namespace Boo
// {
//     /**
//      * @brief 字符数据结构
//      * 包含字符的位图数据、偏移量等信息。
//      */
//     struct GlyphData
//     {
//         wchar_t character; // 字符
//         uint32_t width;    // 位图宽度（像素）
//         uint32_t height;   // 位图高度（像素）
//         int32_t offsetX;   // 在所在字体纹理中的水平偏移（从左到右）
//         int32_t offsetY;   // 在所在字体纹理中的垂直偏移（从上到下）
//         std::vector<uint8_t> bitmapData; // 位图数据（灰度）
//     };
//     /**
//      * @brief 字体图集页结构
//      * 包含字体图集页的宽度、高度、当前写入位置等信息。
//      */
//     struct FontAtlas
//     {
//         std::string uuid;                    // 唯一标识
//         TextureAsset *texture = nullptr;     // 纹理资源
//         size_t width = 0;                    // 图集宽度
//         size_t height = 0;                   // 图集高度
//         size_t cursorX = 0;                  // 当前写入位置 X
//         size_t cursorY = 0;                  // 当前写入位置 Y
//         bool isFull = false;                 // 是否已满
//         std::map<wchar_t, GlyphData> glyphs; // 该页的所有字形
//     };
//     /**
//      * @brief 文本网格批次结构
//      * 包含文本网格的顶点、UV 坐标、索引数据等信息。
//      * 相同字体图集页的字符可以共享同一个批次。
//      */
//     struct TextMeshBatch
//     {
//         std::string fontAtlasUuid;                    // 唯一标识
//         TextureAsset *texture = nullptr; // 使用的纹理
//         std::vector<float> positions={};     // 顶点数据 (x, y, z)
//         std::vector<float> uvs={};          // UV 坐标 (u, v)
//         std::vector<uint32_t> indices={};   // 索引数据
//     };
//     struct TextLayoutResult
//     {
//         int fontSize = 50;
//         uint32_t width = 0;                 // 文本内容宽度
//         uint32_t height = 0;                // 文本内容高度
//         std::vector<TextMeshBatch> batches={}; // 所有纹理批次数组
//     };

//     class FreetypeMgr
//     {
//     private:
//         FreetypeMgr(const FreetypeMgr &) = delete;            // 禁用拷贝构造
//         FreetypeMgr &operator=(const FreetypeMgr &) = delete; // 禁用赋值操作符
//         FreetypeMgr();                                        // 构造函数设为私有
//         ~FreetypeMgr();                                       // 析构函数设为私有
//     private:
//         FT_Face _face;
//         FT_Library _ft;

//     private:
//         std::string _fontPath;
//         // 字体默认大小
//         int _fontSize;
//         // 字体默认基准线距离顶部的距离
//         int _fontBaseLine;
//         // 字体默认行高
//         int _fontLineHeight;
//         // 字间距
//         int _fontSpace;
//         /**
//          * @brief 字体图集页数组
//          */
//         std::vector<FontAtlas> _atlasPages;
//         std::map<wchar_t, GlyphData> _glyphCache;
       
//     private:
//         /**
//          * @brief 创建字体图集页
//          *
//          */
//         void _createFontAtlas();
//         /**
//          * @brief 检查字符是否已缓存
//          *
//          * @param character 字符
//          * @return true 已缓存
//          * @return false 未缓存
//          */
//         bool _isGlyphCached(wchar_t character);
//         /**
//          * @brief 加载字符位图
//          *
//          * @param character 字符
//          */
//         void _loadGlyph(GlyphData &glyphData);
//         /**
//          * @brief 写入字体图集页
//          *
//          * @param glyphData 字符数据
//          */
//         void _writeToAtlas(GlyphData &glyphData);
//         /**
//          * @brief 按行分割文本
//          *
//          * @param text 文本内容
//          * @return std::vector<std::string> 每行文本的向量
//          */
//         std::vector<std::string> _splitTextLines(const std::string &text);
//         /**
//          * @brief 计算文本内容的最大宽度和高度
//          *
//          * @param lines 文本行向量
//          * @param width 文本内容宽度
//          * @param height 文本内容高度
//          */
//         void _getTextContentSize(std::vector<std::string> &lines, uint32_t &width, uint32_t &height);

//         /**
//          * @brief 查找字符所在字体图集页
//          *
//          * @param c 字符
//          * @return FontAtlas 字 字符所在字体图集页
//          */
//         FontAtlas *_findGlyphAtlasPage(wchar_t c);
//         /**
//          * @brief 查找文本网格批次
//          *
//          * @param fontAtlasUuid 字体图集页唯一标识
//          * @return TextMeshBatch 文本网格批次
//          */
//         TextMeshBatch *_findTextMeshBatch(FontAtlas *atlas, std::vector<TextMeshBatch> &batches);
//         /**
//          * @brief 解析文本
//          *
//          * @param text
//          */
//         void _parseText(const std::string &text);
//         void _layoutLineText(const std::string &text, float offsetY,float lineHeight, TextLayoutResult &result);
//         void _layoutText(std::vector<std::string> &lines, TextLayoutResult &result);

//     public:
//         static FreetypeMgr *getInstance();
//         void init();
//         TextLayoutResult create(const std::string &text,int fontSize);
//         void render();
//     };

// } // namespace Boo
