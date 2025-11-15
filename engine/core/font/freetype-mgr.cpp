#include "freetype-mgr.h"
#include "../gfx/gfx-mgr.h"

FreetypeMgr::FreetypeMgr()
{
}

void FreetypeMgr::init()
{

#if defined(_WIN32) || defined(_WIN64)
    this->_fontPath = "C:\\Windows\\Fonts\\simhei.ttf";
#elif defined(__APPLE__)
    this->_fontPath = "/System/Library/Fonts/STHeiti Light.ttc";
#else
    std::cout << "其他系统" << std::endl;
#endif
    // 初始化 FreeType
    if (FT_Init_FreeType(&this->_ft))
    {
        throw std::runtime_error("could not init FreeType Library");
    }
    // 加载字体

    if (FT_New_Face(this->_ft, this->_fontPath.c_str(), 0, &this->_face))
    {
        throw std::runtime_error("failed to load font");
    }
    TextTexture textTexture{};
    this->crateFont(textTexture, "你好,Hello World", 20, 20);
}
void FreetypeMgr::crateFont(TextTexture &textTexture, const std::string &str, uint32_t fontSize, uint32_t lineHeight)
{
    std::cout << "crateFont: " << str << std::endl;
    std::string textureUuid = "font-";
    FT_Set_Pixel_Sizes(this->_face, 0, fontSize);
    // 开启抗锯齿
    std::vector<wchar_t> chars;
    std::wstring wstr = FmtUtil::utf8_to_wstring(str);
    for (wchar_t c : wstr)
    {
        // 避免重复添加
        chars.push_back(c);
    }
    std::cout << "crateFont1: " << str << std::endl;
    uint32_t x = 3;
    uint32_t y = 3;
    uint32_t atlasWidth = 6;
    uint32_t atlasHeight = 6;
    uint32_t datumLine = 0;              // 基准线
    uint32_t datumLine_below_height = 0; // 基准线下边的高度
    uint32_t space = fontSize * 0.1;     // 空格宽度
    std::cout << "space: " << space << std::endl;
    // 提前与计算宽高
    for (wchar_t c : chars)
    {
        if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))
        {
            continue;
        }
        textureUuid += std::to_string(c);
        FT_GlyphSlot glyph = this->_face->glyph;
        // 最大宽度
        uint32_t width = glyph->bitmap.width;
        atlasWidth += width + space; // 加1像素间隔
        // 最大高度
        uint32_t height = glyph->bitmap.rows; // 文字的高度
        uint32_t top = glyph->bitmap_top;     // 文字的基准线距离顶部的距离
        datumLine = std::max(datumLine, top);
        if (height > top)
        {
            datumLine_below_height = std::max(datumLine_below_height, height - top);
        }
    }
    atlasHeight += datumLine + datumLine_below_height;
    std::vector<uint8_t> atlasData(atlasWidth * atlasHeight * 1, 0);
    for (wchar_t c : chars)
    {
        if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))

        {
            continue;
        }

        FT_GlyphSlot glyph = this->_face->glyph;
        uint32_t width = glyph->bitmap.width;
        uint32_t height = glyph->bitmap.rows;
        int offsetY = (datumLine - glyph->bitmap_top);
        // 复制字形数据到图集
        for (uint32_t i = 0; i < height; i++)
        {
            for (uint32_t j = 0; j < width; j++)
            {
                // 计算当前文字的x和y坐标的颜色值  0或1
                uint32_t glyphIndex = i * width + j;

                uint8_t gray = glyph->bitmap.buffer[glyphIndex];
                // 填充到atlasData
                uint32_t _x = x + j;
                uint32_t _y = y + i + offsetY; // 高度需要进行偏移
                uint32_t atlasIndex = (_y)*atlasWidth + (_x);
                if (atlasIndex < atlasData.size())
                {
                    atlasData[atlasIndex] = gray;
                }
            }
        }
        x += width + space; // 加1像素间隔
    }
    FileUtil::saveAtlasAsPGM(textureUuid + ".ppm", atlasData, atlasWidth, atlasHeight);
    std::cout << "datumLine: " << datumLine << std::endl;
    std::cout << "datumLine_below_height: " << datumLine_below_height << std::endl;
    // 转换为RGBA格式
    std::vector<uint8_t> rgbaDatas(atlasWidth * atlasHeight * 4, 0);
    for (uint32_t i = 0; i < atlasWidth * atlasHeight; i++)
    {
        rgbaDatas[i * 4 + 0] = 255;
        rgbaDatas[i * 4 + 1] = 255;
        rgbaDatas[i * 4 + 2] = 255;
        rgbaDatas[i * 4 + 3] = atlasData[i] > 100 ? 255 : 0;
    }
    textureUuid += ".png";
    textTexture.uuid = textureUuid;
    textTexture.width = atlasWidth;
    textTexture.height = atlasHeight;
    textTexture.channels = 4;
    textTexture.datas = rgbaDatas;
}


FreetypeMgr::~FreetypeMgr()
{
    FT_Done_Face(this->_face);
    FT_Done_FreeType(this->_ft);
}
