// #include "freetype-mgr.h"
// #include "log.h"
// #include "platforms/platform.h"
// #include "core/gfx/gfx-manager.h"
// #include "core/util/uuid-util.h"
// #include "core/util/fmt-util.h"
// #include "core/asset/asset-manager.h"
// #include "core/asset/texture-asset.h"
// #include "core/gfx/base/gfx-texture.h"

// namespace Boo
// {

//     FreetypeMgr::FreetypeMgr() : _fontPath(""),
//                                  _fontSize(0),
//                                  _fontBaseLine(0),
//                                  _fontLineHeight(0),
//                                  _face(nullptr),
//                                  _ft(nullptr),
//                                  _atlasPages()
//     {
// // #if defined(BOO_PLATFORM_WINDOWS)
// //         this->_fontPath = "C:\\Windows\\Fonts\\simhei.ttf";
// // #elif defined(BOO_PLATFORM_MACOS)
// //         this->_fontPath = "/System/Library/Fonts/STHeiti Light.ttc";
// // #else
// //         LOGI("[FreetypeMgr]: 其他系统");
// //         return;
// // #endif
//     }
//     FreetypeMgr *FreetypeMgr::getInstance()
//     {
//         static FreetypeMgr _instance;
//         return &_instance;
//     }
//     void FreetypeMgr::init()
//     {
//         // this->_fontSpace = 4;
//         // this->_fontSize = 50;
//         // this->_fontBaseLine = this->_fontSize * 0.95;
//         // this->_fontLineHeight = this->_fontSize * 1.1;
//         // this->_createFontAtlas();
//         // // 初始化 FreeType
//         // if (FT_Init_FreeType(&this->_ft))
//         // {
//         //     throw std::runtime_error("could not init FreeType Library");
//         // }
//         // // 加载字体

//         // if (FT_New_Face(this->_ft, this->_fontPath.c_str(), 0, &this->_face))
//         // {
//         //     throw std::runtime_error("failed to load font");
//         // }
//         // this->_parseText("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890999999,.-=+:;ssdjadfasfhlksdfjkasdjfaskld");
//     }
//     /**
//      * @brief 检查字符是否已缓存
//      *
//      * @param character 字符
//      * @return true 已缓存
//      * @return false 未缓存
//      */
//     bool FreetypeMgr::_isGlyphCached(wchar_t character)
//     {
//         return this->_glyphCache.find(character) != this->_glyphCache.end();
//     }
//     /**
//      * @brief 创建字体图集页
//      *
//      */
//     void FreetypeMgr::_createFontAtlas()
//     {
//         int width = 1024;
//         int height = 1024;
//         int channels = 4;
//         // 创建新的纹理
//         std::vector<uint8_t> transparentPixels(width * height * channels, 0);
//         TextureAsset *textureAsset = new TextureAsset(UuidUtil::generateUUID());
//         textureAsset->createDynamic(width, height, channels, transparentPixels, GfxTextureFormat::R8G8B8A8_SRGB);
//         FontAtlas atlasAtlas;
//         atlasAtlas.uuid = UuidUtil::generateUUID();
//         atlasAtlas.width = width;
//         atlasAtlas.height = height;
//         atlasAtlas.cursorX = 0;
//         atlasAtlas.cursorY = 0;
//         atlasAtlas.texture = textureAsset;
//         atlasAtlas.isFull = false;
//         this->_atlasPages.push_back(atlasAtlas);
//     }
//     /**
//      * @brief 加载字符位图
//      *
//      * @param character 字符
//      */
//     void FreetypeMgr::_loadGlyph(GlyphData &glyphData)
//     {
//         FT_Set_Pixel_Sizes(this->_face, 0, this->_fontSize);
//         if (FT_Load_Char(this->_face, glyphData.character, FT_LOAD_RENDER))
//         {
//             return;
//         }
//         if (glyphData.character == L' ' || glyphData.character == L'\t' || glyphData.character == L'\n')
//         {
//             // 空格,制表符,换行符,不存储
//             return;
//         }
//         FT_GlyphSlot glyph = this->_face->glyph;
//         uint32_t width = glyph->bitmap.width;                         // 位图宽度（像素） 宽度是更具高度动态变化的
//         uint32_t height = glyph->bitmap.rows;                         // 位图高度（像素） 高度大于会在设置的size,但是说是有可能也会大于size,
//         uint32_t bearingX = glyph->bitmap_left;                       // 水平距离，即位图相对于原点的水平位置（像素）
//         uint32_t bearingY = glyph->bitmap_top;                        // 垂直距离，即位图相对于原点的垂直位置（像素）
//         uint32_t advance = glyph->advance.x >> 6;                     // 水平预留值，即原点到下一个字形原点的水平距离（单位：1/64像素）
//         int offsetY = _fontBaseLine - bearingY;                       // 基准线距离顶部的距离
//         std::vector<uint8_t> datas(width * this->_fontLineHeight, 0); // 70 * width的像素数据
//         // 复制位图数据到datas
//         for (uint32_t y = 0; y < height; y++)
//         {
//             for (uint32_t x = 0; x < width; x++)
//             {
//                 uint32_t glyphIndex = y * width + x;
//                 uint8_t gray = glyph->bitmap.buffer[glyphIndex];
//                 datas[(y + offsetY) * width + x] = gray;
//             }
//         }
//         glyphData.width = width;
//         glyphData.height = uint32_t(this->_fontLineHeight);
//         glyphData.bitmapData = datas;
//     }
//     void FreetypeMgr::_writeToAtlas(GlyphData &glyphData)
//     {
//         uint32_t offset_x = 4;
//         // 获取最后一张图集页
//         FontAtlas &atlas = this->_atlasPages.back();
//         // 检查是否需要换行
//         if (atlas.cursorX + glyphData.width + offset_x > atlas.width)
//         {
//             atlas.cursorX = 0;
//             atlas.cursorY += this->_fontLineHeight;
//         }
//         // 换行后如果放不下需要创建新的图集页
//         if (atlas.cursorY + glyphData.height > atlas.height)
//         {
//             atlas.isFull = true;
//             this->_createFontAtlas();
//             atlas = this->_atlasPages.back();
//         }
//         GfxTexture *texture = atlas.texture->getGfxTexture();
//         // 开启预留2个像素
//         uint32_t startX = atlas.cursorX + offset_x;
//         uint32_t startY = atlas.cursorY;
//         glyphData.offsetX = startX;
//         glyphData.offsetY = startY;
//         for (uint32_t y = 0; y < glyphData.height; y++)
//         {
//             for (uint32_t x = 0; x < glyphData.width; x++)
//             {
//                 uint32_t _x = startX + x;
//                 uint32_t _y = startY + y;

//                 if (texture != nullptr)
//                 {
//                     uint8_t pixel = glyphData.bitmapData[(y)*glyphData.width + x];
//                     texture->setPixel(_x, _y, {pixel, pixel, pixel, pixel});
//                 }
//             }
//         }
//         atlas.cursorX = startX + glyphData.width;
//         atlas.glyphs[glyphData.character] = glyphData;
//         this->_glyphCache[glyphData.character] = glyphData;
//     }

//     void FreetypeMgr::_parseText(const std::string &text)
//     {
//         FT_Set_Pixel_Sizes(this->_face, 0, this->_fontSize);
//         std::wstring wstr = FmtUtil::utf8_to_wstring(text);
//         for (wchar_t c : wstr)
//         {
//             if (std::iswspace(c))
//             {
//                 continue;
//             }
//             if (this->_isGlyphCached(c))
//             {
//                 continue;
//             }
//             GlyphData glyphData{};
//             glyphData.character = c;
//             this->_loadGlyph(glyphData);
//             if (glyphData.bitmapData.empty())
//             {
//                 continue;
//             }
//             this->_writeToAtlas(glyphData);

//             // 写入后,清空位图数据
//             glyphData.bitmapData.clear();
//         }
// #if FREETYPE_DEBUG_OUT
//         for (FontAtlas &atlas : this->_atlasPages)
//         {
//             GfxTexture *texture = atlas.texture->getGfxTexture();
//             texture->updateData();
//             texture->saveToFile("font-" + texture->getUuid() + ".png"); // 调试用查看图集页数据
//         }
// #endif
//     }

//     std::vector<std::string> FreetypeMgr::_splitTextLines(const std::string &text)
//     {
//         std::vector<std::string> lines;
//         std::stringstream ss(text);
//         std::string line;
//         while (std::getline(ss, line, '\n'))
//         {
//             lines.push_back(line);
//         }
//         return lines;
//     }
//     void FreetypeMgr::_getTextContentSize(std::vector<std::string> &lines, uint32_t &width, uint32_t &height)
//     {
//         height = lines.size() * this->_fontLineHeight;
//         for (std::string line : lines)
//         {
//             std::wstring wstr = FmtUtil::utf8_to_wstring(line);
//             int lineWidth = 0;
//             for (wchar_t c : wstr)
//             {
//                 if (std::iswspace(c))
//                 {
//                     // 空格,制表符,换行符,不存储
//                     lineWidth += this->_fontSize * 0.3;
//                     lineWidth += this->_fontSpace;
//                     continue;
//                 }
//                 if (!this->_isGlyphCached(c))
//                 {
//                     continue;
//                 }
//                 GlyphData &glyphData = this->_glyphCache[c];
//                 lineWidth += glyphData.width;
//                 lineWidth += this->_fontSpace;
//             }
//             // 最后一个字不需要预留2个像素
//             if (lineWidth > 0)
//                 lineWidth -= this->_fontSpace;
//             width = std::max(width, uint32_t(lineWidth));
//         }
//     }
//     FontAtlas *FreetypeMgr::_findGlyphAtlasPage(wchar_t c)
//     {
//         for (FontAtlas &atlas : this->_atlasPages)
//         {
//             std::map<wchar_t, GlyphData> &glyphs = atlas.glyphs;
//             if (glyphs.find(c) != glyphs.end())
//             {
//                 return &atlas;
//             }
//         }
//         return nullptr;
//     }
//     TextMeshBatch *FreetypeMgr::_findTextMeshBatch(FontAtlas *atlas, std::vector<TextMeshBatch> &batches)
//     {
//         // std::cout << "findTextMeshBatch: atlas " << atlas->uuid << std::endl;
//         // if (batches.size() > 0)
//         // {
//         for (TextMeshBatch &batch : batches)
//         {
//             if (batch.fontAtlasUuid == atlas->uuid)
//             {
//                 return &batch;
//             }
//         }
//         // }
//         // std::cout << "findTextMeshBatch: create new batch for atlas " << atlas->uuid << std::endl;
//         // 没有找见,创建新的批次
//         TextMeshBatch batch;
//         batch.fontAtlasUuid = atlas->uuid;
//         batch.texture = atlas->texture;
//         batch.positions.clear();
//         batch.uvs.clear();
//         batch.indices.clear();
//         batches.push_back(batch);
//         return &batches.back();
//     }

//     void FreetypeMgr::_layoutLineText(const std::string &text, float offsetY, float lineHeight, TextLayoutResult &result)
//     {
//         std::wstring wstr = FmtUtil::utf8_to_wstring(text);
//         // 计算当前行宽度
//         float lineWidth = 0;
//         for (wchar_t c : wstr)
//         {
//             if (std::iswspace(c))
//             {
//                 // 空格,制表符,换行符,不存储
//                 lineWidth += this->_fontSize * 0.3;
//                 lineWidth += this->_fontSpace;
//                 continue;
//             }
//             if (!this->_isGlyphCached(c))
//             {
//                 continue;
//             }
//             GlyphData &glyphData = this->_glyphCache[c];
//             lineWidth += glyphData.width;
//             lineWidth += this->_fontSpace;
//         }
//         // 最后一个字不需要预留像素
//         if (lineWidth > 0)
//             lineWidth -= this->_fontSpace;
//         float unit_x = 1.0f / result.width;
//         float unit_y = 1.0f / result.height;
//         float left = -0.5 + (result.width - lineWidth) / 2.0f * unit_x;
//         float right = left;
//         float top = offsetY * unit_y;
//         float bottom = (offsetY - this->_fontLineHeight) * unit_y;
//         for (wchar_t c : wstr)
//         {
//             if (std::iswspace(c))
//             {
//                 // 空格,制表符,换行符,不存储
//                 left += this->_fontSize * 0.3 * unit_x;
//                 left += this->_fontSpace * unit_x;
//                 continue;
//             }
//             if (!this->_isGlyphCached(c))
//             {
//                 continue;
//             }
//             FontAtlas *atlas = this->_findGlyphAtlasPage(c);
//             if (atlas == nullptr)
//             {
//                 continue;
//             }
//             TextMeshBatch *batch = this->_findTextMeshBatch(atlas, result.batches);
//             if (batch == nullptr)
//             {
//                 std::cout << "layoutLineText: findTextMeshBatch failed for atlas " << atlas->uuid << std::endl;
//                 continue;
//             }
//             // std::cout << "layoutLineText: c " << std::to_string(c) << std::endl;
//             GlyphData &glyphData = atlas->glyphs[c];
//             right = left + glyphData.width * unit_x;
//             float uv_x = 1.0 * glyphData.offsetX / atlas->width;
//             float uv_y = 1.0 * glyphData.offsetY / atlas->height;
//             float uv_w = 1.0 * glyphData.width / atlas->width;
//             float uv_h = 1.0 * glyphData.height / atlas->height;
//             // 索引
//             int indices_offset = batch->positions.size() / 3;

//             // 第一个三角形
//             // 左上角
//             batch->positions.push_back(left);
//             batch->positions.push_back(top);
//             batch->positions.push_back(0.0f);
//             batch->uvs.push_back(uv_x);
//             batch->uvs.push_back(uv_y);
//             // 左下角
//             batch->positions.push_back(left);
//             batch->positions.push_back(bottom);
//             batch->positions.push_back(0.0f);
//             batch->uvs.push_back(uv_x);
//             batch->uvs.push_back(uv_y + uv_h);
//             // 右下角
//             batch->positions.push_back(right);
//             batch->positions.push_back(bottom);
//             batch->positions.push_back(0.0f);
//             batch->uvs.push_back(uv_x + uv_w);
//             batch->uvs.push_back(uv_y + uv_h);
//             // 第二个三角形
//             // 左上角
//             batch->positions.push_back(left);
//             batch->positions.push_back(top);
//             batch->positions.push_back(0.0f);
//             batch->uvs.push_back(uv_x);
//             batch->uvs.push_back(uv_y);
//             // 右下角
//             batch->positions.push_back(right);
//             batch->positions.push_back(bottom);
//             batch->positions.push_back(0.0f);
//             batch->uvs.push_back(uv_x + uv_w);
//             batch->uvs.push_back(uv_y + uv_h);
//             // 右上角
//             batch->positions.push_back(right);
//             batch->positions.push_back(top);
//             batch->positions.push_back(0.0f);
//             batch->uvs.push_back(uv_x + uv_w);
//             batch->uvs.push_back(uv_y);
//             // 索引
//             batch->indices.push_back(indices_offset);
//             batch->indices.push_back(indices_offset + 1);
//             batch->indices.push_back(indices_offset + 2);
//             batch->indices.push_back(indices_offset + 3);
//             batch->indices.push_back(indices_offset + 4);
//             batch->indices.push_back(indices_offset + 5);
//             left = right;
//             left += this->_fontSpace * unit_x;
//         }
//     }
//     void FreetypeMgr::_layoutText(std::vector<std::string> &lines, TextLayoutResult &result)
//     {
//         float offsetY = result.height / 2.0f;
//         for (std::string line : lines)
//         {
//             this->_layoutLineText(line, offsetY, this->_fontLineHeight, result);
//             offsetY -= this->_fontLineHeight;
//         }
//     }

//     TextLayoutResult FreetypeMgr::create(const std::string &text, int fontSize)
//     {
//         // 解析文本,进行存储
//         this->_parseText(text);
//         TextLayoutResult result;
//         std::vector<std::string> lines = this->_splitTextLines(text);
//         this->_getTextContentSize(lines, result.width, result.height);
//         this->_layoutText(lines, result);
//         float rate = 1.0f * fontSize / this->_fontSize;
//         result.fontSize = fontSize;
//         result.width *= rate;
//         result.height *= rate;
//         return result;
//     }

//     void FreetypeMgr::render()
//     {
//     }
//     FreetypeMgr::~FreetypeMgr()
//     {
//         FT_Done_Face(this->_face);
//         FT_Done_FreeType(this->_ft);
//     }

// } // namespace Boo
