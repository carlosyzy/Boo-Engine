// #include "freetype-mgr.h"
// #include "../../libs/stb/stb_image_write.h"
// #include "../gfx/gfx-mgr.h"

// FreetypeMgr *FreetypeMgr::getInstance()
// {
//     static FreetypeMgr instance;
//     return &instance;
// }
// void FreetypeMgr::init()
// {

// #if defined(_WIN32) || defined(_WIN64)
//     this->_fontPath = "C:\\Windows\\Fonts\\simhei.ttf";
// #elif defined(__APPLE__)
//     this->_fontPath = "/System/Library/Fonts/STHeiti Light.ttc";
// #else
//     std::cout << "其他系统" << std::endl;
// #endif
//     // 初始化 FreeType
//     if (FT_Init_FreeType(&this->_ft))
//     {
//         throw std::runtime_error("could not init FreeType Library");
//     }
//     // 加载字体

//     if (FT_New_Face(this->_ft, this->_fontPath.c_str(), 0, &this->_face))
//     {
//         throw std::runtime_error("failed to load font");
//     }
//     TextTexture texture = this->crateFont("NodeNameqj", 80, 80);
// }
// TextTexture FreetypeMgr::crateFont(const std::string &str, uint32_t fontSize, uint32_t lineHeight)
// {
//     TextTexture texture;
//     std::string textureUuid = "font-";
//     FT_Set_Pixel_Sizes(this->_face, 0, fontSize);
//     std::vector<wchar_t> chars;
//     std::wstring wstr = this->_utf8_to_wstring(str);
//     for (wchar_t c : wstr)
//     {
//         // 避免重复添加
//         chars.push_back(c);
//     }
//     uint32_t x = 3;
//     uint32_t y = 3;
//     uint32_t atlasWidth = 6;
//     uint32_t atlasHeight = 6;
//     uint32_t datumLine = 0;              // 基准线
//     uint32_t datumLine_below_height = 0; // 基准线下边的高度

//     // 提前与计算宽高
//     for (wchar_t c : chars)
//     {
//         if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))
//         {
//             continue;
//         }
//         textureUuid += std::to_string(c);
//         FT_GlyphSlot glyph = this->_face->glyph;
//         // 最大宽度
//         uint32_t width = glyph->bitmap.width;
//         atlasWidth += width + 3; // 加1像素间隔
//         // 最大高度
//         uint32_t height = glyph->bitmap.rows; // 文字的高度
//         uint32_t top = glyph->bitmap_top;     // 文字的基准线距离顶部的距离
//         datumLine = std::max(datumLine, top);
//         if (height > top)
//         {
//             datumLine_below_height = std::max(datumLine_below_height, height - top);
//         }
//     }
//     atlasHeight += datumLine + datumLine_below_height;
//     std::vector<uint8_t> atlasData(atlasWidth * atlasHeight * 1, 0);
//     for (wchar_t c : chars)
//     {
//         if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))

//         {
//             continue;
//         }

//         FT_GlyphSlot glyph = this->_face->glyph;
//         uint32_t width = glyph->bitmap.width;
//         uint32_t height = glyph->bitmap.rows;
//         int offsetY = (datumLine - glyph->bitmap_top);
//         // 复制字形数据到图集
//         for (uint32_t i = 0; i < height; i++)
//         {
//             for (uint32_t j = 0; j < width; j++)
//             {
//                 // 计算当前文字的x和y坐标的颜色值  0或1
//                 uint32_t glyphIndex = i * width + j;

//                 uint8_t gray = glyph->bitmap.buffer[glyphIndex];
//                 // 填充到atlasData
//                 uint32_t _x = x + j;
//                 uint32_t _y = y + i + offsetY; // 高度需要进行偏移
//                 uint32_t atlasIndex = (_y)*atlasWidth + (_x);
//                 if (atlasIndex < atlasData.size())
//                 {
//                     atlasData[atlasIndex] = gray;
//                 }
//             }
//         }
//         x += width + 3; // 加1像素间隔
//     }
//     // FileUtil::saveAtlasAsPGM(textureUuid + ".ppm", atlasData, atlasWidth, atlasHeight);
//     std::vector<uint8_t> defDatas(atlasWidth * atlasHeight * 1, 0);
//     defDatas = SDFUtil::generateSDF(atlasData, atlasWidth, atlasHeight);
//     textureUuid += ".png";
//     GfxMgr::getInstance()->createTexture(textureUuid, atlasWidth, atlasHeight, 1, &defDatas);

//     texture.texture = textureUuid;
//     texture.width = atlasWidth;
//     texture.height = atlasHeight;

//     return texture;
// }

// void FreetypeMgr::destroy()
// {
//     FT_Done_Face(this->_face);
//     FT_Done_FreeType(this->_ft);
// }

// // // this->saveAtlasAsPGM("font_atlas.ppm", atlasData, atlasWidth, atlasHeight);
// // // 保存单通道数据为PGM文件（更高效，文件更小）
// // void FreetypeMgr::saveAtlasAsPGM(const std::string &filename,
// //                                  const std::vector<uint8_t> &atlasData,
// //                                  int width, int height)
// // {
// //     if (atlasData.size() != width * height)
// //     {
// //         throw std::runtime_error("Atlas data size doesn't match dimensions");
// //     }

// //     std::ofstream file(filename, std::ios::binary);
// //     if (!file)
// //     {
// //         throw std::runtime_error("Failed to open file: " + filename);
// //     }

// //     // 写入PGM文件头
// //     file << "P5\n"
// //          << width << " " << height << "\n255\n";

// //     // 直接写入单通道数据
// //     file.write(reinterpret_cast<const char *>(atlasData.data()), atlasData.size());

// //     file.close();
// // }
// // void FreetypeMgr::saveAtlasAsPNG(const std::string &filename,
// //                                  const std::vector<uint8_t> &atlasData,
// //                                  int width, int height)
// // {
// //     // 由于 atlasData 是 int8_t（有符号字符），我们需要转换为 unsigned char
// //     std::vector<unsigned char> unsignedData(atlasData.size());
// //     for (size_t i = 0; i < atlasData.size(); ++i)
// //     {
// //         // 将有符号转换为无符号，保持相同的位模式
// //         unsignedData[i] = dynamic_cast<unsigned char>(atlasData[i]);
// //     }

// //     // 保存为 PNG
// //     int result = stbi_write_png(
// //         filename.c_str(),    // 文件名
// //         width,               // 宽度
// //         height,              // 高度
// //         4,                   // 通道数 (RGBA)
// //         unsignedData.data(), // 数据指针
// //         width * 4            // 每行的字节数 (宽度 * 4通道)
// //     );

// //     // if (result)
// //     // {
// //     //     std::cout << "Successfully saved atlas to: " << filename << std::endl;
// //     // }
// //     // else
// //     // {
// //     //     std::cerr << "Failed to save atlas to: " << filename << std::endl;
// //     // }
// // }

// // UTF-8 string 转 wstring
// std::wstring FreetypeMgr::_utf8_to_wstring(const std::string &str)
// {
//     std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//     return converter.from_bytes(str);
// }

// // // wstring 转 UTF-8 string
// // std::string wstring_to_utf8(const std::wstring& wstr) {
// //     std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
// //     return converter.to_bytes(wstr);
// // }

// // // 使用示例
// // std::string utf8Text = "你好世界";
// // std::wstring wideText = utf8_to_wstring(utf8Text);  // 转换为 L"你好世界"
// //  // 设置字体大小
// //     FT_Set_Pixel_Sizes(face, 0, this->_fontSize);
// //     // 计算图集尺寸（这里以2048x2048为例，实际需要动态计算）
// //     const uint32_t atlasWidth = 8192;
// //     const uint32_t atlasHeight = 8192;
// //     std::vector<uint8_t> atlasData(atlasWidth * atlasHeight, 0);
// //     std::cout << "atlasWidth: " << atlasWidth << std::endl;
// //     std::cout << "atlasHeight: " << atlasHeight << std::endl;

// //     uint32_t x = 0;
// //     uint32_t y = 0;
// //     uint32_t rowHeight = 0;

// //     // 加载常用汉字字符集（Unicode范围）
// //     std::vector<wchar_t> chineseChars;
// //     // for (wchar_t c = 0x4E00; c <= 0x9FFF; c++)
// //     // { // 基本汉字区
// //     //     chineseChars.push_back(c);
// //     // }
// //     // std::cout << "chineseChars1.size(): " << chineseChars.size() << std::endl;

// //     // 加载ASCII字符
// //     for (unsigned char c = 32; c < 128; c++)
// //     {
// //         // std::cout << "c: " << c << std::endl;

// //         chineseChars.push_back(dynamic_cast<wchar_t>(c));
// //     }

// //     for (wchar_t c = 0x4E00; c <= 0x9FA5; c++) {
// //         chineseChars.push_back(c);
// //     }
// //     // std::wstring chineseText = L"你好世界这是一段中文文本欢迎使用FreeType渲染";
// //     // for (wchar_t c : chineseText) {
// //     //     // 避免重复添加
// //     //     chineseChars.push_back(c);
// //     // }

// //     std::cout << "chineseChars2.size(): " << chineseChars.size() << std::endl;

// //     // 将字符打包到纹理图集
// //     for (wchar_t c : chineseChars)
// //     {
// //         if (FT_Load_Char(face, c, FT_LOAD_RENDER))
// //         {
// //             continue;
// //         }
// //         // std::cout << "face->num_glyphs:wchar_t " << std::endl;

// //         FT_GlyphSlot glyph = face->glyph;
// //         uint32_t width = glyph->bitmap.width;
// //         uint32_t height = glyph->bitmap.rows;

// //         // 检查是否需要换行
// //         if (x + width >= atlasWidth)
// //         {
// //             x = 0;
// //             y += rowHeight + 1; // 加1像素间隔
// //             rowHeight = 0;
// //         }

// //         // 更新行高
// //         rowHeight = std::max(rowHeight, height);

// //         // 检查图集是否已满
// //         if (y + height >= atlasHeight)
// //         {
// //             std::cout << "Warning: Font atlas is full!" << std::endl;
// //             break;
// //         }

// //         // 复制字形数据到图集
// //         for (uint32_t i = 0; i < height; i++)
// //         {
// //             for (uint32_t j = 0; j < width; j++)
// //             {
// //                 uint32_t atlasIndex = (y + i) * atlasWidth + (x + j);
// //                 uint32_t glyphIndex = i * width + j;
// //                 atlasData[atlasIndex] = glyph->bitmap.buffer[glyphIndex];
// //             }
// //         }
// //         // 计算纹理坐标
// //         Vec2 coords[4];
// //         coords[0] = Vec2((float)x / atlasWidth, (float)(y + height) / atlasHeight);           // 左下
// //         coords[1] = Vec2((float)(x + width) / atlasWidth, (float)(y + height) / atlasHeight); // 右下
// //         coords[2] = Vec2((float)(x + width) / atlasWidth, (float)y / atlasHeight);            // 右上
// //         coords[3] = Vec2((float)x / atlasWidth, (float)y / atlasHeight);                      // 左上

// //         // 存储字符信息
// //         Character character;
// //         character.texCoords[0] = coords[0]; // 左下
// //         character.texCoords[1] = coords[1]; // 右下
// //         character.texCoords[2] = coords[2]; // 右上
// //         character.texCoords[3] = coords[3]; // 左上
// //         character.width = width;
// //         character.height = height;
// //         character.bearingX = glyph->bitmap_left;
// //         character.bearingY = glyph->bitmap_top;
// //         character.advance = dynamic_cast<uint32_t>(glyph->advance.x);
// //         this->_characters[c] = character;

// //         x += width + 1; // 加1像素间隔
// //     }

// //     // // 创建 Vulkan 纹理
// //     // createVulkanTexture(atlasData.data(), atlasWidth, atlasHeight);
// //     // GfxMgr::getInstance()->createGfxTexture("font_atlas", atlasData.data(), atlasWidth, atlasHeight);
