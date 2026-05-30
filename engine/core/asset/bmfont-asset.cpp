#include "bmfont-asset.h"
#include "log.h"
#include "boo.h"
#include "core/util/xlm-util.h"
namespace Boo
{
    BMFontAsset::BMFontAsset() : Asset()
    {
        this->_type = EAssetType::BMFont;
    }
    BMFontAsset::BMFontAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = EAssetType::BMFont;
    }
    BMFontAsset::BMFontAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = EAssetType::BMFont;
    }
    /**
     * @brief 创建BMFont资产
     *
     * @param bmFontContent BMFont资产内容
     */
    void BMFontAsset::create(std::string bmFontContent)
    {
        this->_lineHeight = 0;
        this->_base = 0;
        this->_pageFiles.clear();
        this->_glyphs.clear();
        this->_parseContent(bmFontContent);
        if(this->_fontSize<=0||this->_lineHeight<=0||this->_glyphs.size()<=0){
            LOGW("BMFontAsset::parse fnt fail..");
            return;
        }
        LOGI("BMFontAsset::parse fnt success..");
        this->_parseBMPageTexture();
    }
    void BMFontAsset::_parseContent(std::string &bmFontContent)
    {
        if (bmFontContent.empty())
        {
            return;
        }
        if (bmFontContent.find("<?xml") == 0 || bmFontContent.find("<font>") != std::string::npos)
        {
            return _parseBMFontXML(bmFontContent);
        }
        else
        {
            return _parseBMFont(bmFontContent);
        }
    }
    void BMFontAsset::_parseBMFontXML(std::string &bmFontContent)
    {
        // 解析BMFont XML内容
        tinyxml2::XMLDocument doc;
        doc.Parse(bmFontContent.c_str());
        if (doc.ErrorID() != tinyxml2::XML_SUCCESS)
        {
            LOGE("BMFont XML解析失败");
            return;
        }
        // 获取根节点
        auto *font = doc.FirstChildElement("font");
        if (!font)
            return;
        // 读取info属性
        auto *info = font->FirstChildElement("info");
        int fontSize = info->IntAttribute("size");
        this->_fontSize = fontSize;
        // 读取 common 属性
        auto *common = font->FirstChildElement("common");
        int lineHeight = common->IntAttribute("lineHeight");
        int base = common->IntAttribute("base");
        int textureWidth = common->IntAttribute("scaleW");
        int textureHeight = common->IntAttribute("scaleH");
        this->_lineHeight = lineHeight;
        this->_base = base;
        this->_textureWidth = textureWidth;
        this->_textureHeight = textureHeight;
        // 读取 page 属性
        // 2. 读取 pages 信息（纹理文件列表）
        auto *pagesNode = font->FirstChildElement("pages");
        if (pagesNode)
        {
            int pageId = 0;
            for (auto *page = pagesNode->FirstChildElement("page"); page; page = page->NextSiblingElement("page"))
            {

                int id = page->IntAttribute("id");
                const char *file = page->Attribute("file");
                BMFontPage bmFontPage;
                bmFontPage.id = id;
                bmFontPage.file = file;
                bmFontPage.texture = nullptr;
                this->_pageFiles[id] = bmFontPage;
                pageId++;
            }
        }

        // 遍历所有字符
        BMGlyphData bmGlyphData;
        auto *chars = font->FirstChildElement("chars");
        for (auto *ch = chars->FirstChildElement("char");
             ch;
             ch = ch->NextSiblingElement("char"))
        {

            int id = ch->IntAttribute("id");
            int x = ch->IntAttribute("x");
            int y = ch->IntAttribute("y");
            int width = ch->IntAttribute("width");
            int height = ch->IntAttribute("height");
            int xoffset = ch->IntAttribute("xoffset");
            int yoffset = ch->IntAttribute("yoffset");
            int xadvance = ch->IntAttribute("xadvance");
            int page = ch->IntAttribute("page");
            int chnl = ch->IntAttribute("chnl");
            bmGlyphData.id = id;
            bmGlyphData.x = x;
            bmGlyphData.y = y;
            bmGlyphData.width = width;
            bmGlyphData.height = height;
            bmGlyphData.xoffset = xoffset;
            bmGlyphData.yoffset = yoffset;
            bmGlyphData.xadvance = xadvance;
            bmGlyphData.page = page;
            bmGlyphData.chnl = chnl;
            this->_glyphs[id] = bmGlyphData;
            // printf("字符 %d 位置: (%d, %d)\n", id, x, y);
        }
    }
    void BMFontAsset::_parseBMFont(std::string &bmFontContent)
    {
        // 解析BMFont内容
        std::istringstream iss(bmFontContent);
        std::string line;
        while (std::getline(iss, line))
        {
            if (line.empty())
                continue;
            // std::cout << line << std::endl;
            // 解析 info 行
            if (line.find("info") == 0)
            {
                char face[256];
                std::sscanf(line.c_str(),
                            "info face=\"%255[^\"]\" size=%d",
                            face,
                            &this->_fontSize);
                // std::cout << "face: " << face << ", fontSize: " << this->_fontSize << std::endl;
            }
            // 解析 common 行
            else if (line.find("common") == 0)
            {
                std::sscanf(line.c_str(),
                            "common lineHeight=%d base=%d scaleW=%d scaleH=%d",
                            &this->_lineHeight, &this->_base, &this->_textureWidth, &this->_textureHeight);
                // std::cout << "lineHeight: " << this->_lineHeight << ", base: " << this->_base << ", textureWidth: " << this->_textureWidth << ", textureHeight: " << this->_textureHeight << std::endl;
            }
            // 解析 page 行
            else if (line.find("page") == 0)
            {
                char file[256];
                int id;
                std::sscanf(line.c_str(), "page id=%d file=\"%255[^\"]\"",
                            &id, file); // 这里 pages 会被覆盖，实际上应该是 page id
                BMFontPage bmFontPage;
                bmFontPage.id = id;
                bmFontPage.file = file;
                bmFontPage.texture = nullptr;
                this->_pageFiles[id] = bmFontPage;
                // std::cout << "page id: " << id << ", file: " << file << std::endl;
            }
            // 解析 char 行
            else if (line.find("char") == 0)
            {
                BMGlyphData ch;
                std::sscanf(line.c_str(),
                            "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d",
                            &ch.id, &ch.x, &ch.y, &ch.width, &ch.height,
                            &ch.xoffset, &ch.yoffset, &ch.xadvance, &ch.page, &ch.chnl);
                this->_glyphs[ch.id] = ch;
                // std::cout << "char id: " << ch.id << ", x: " << ch.x << ", y: " << ch.y << ", width: " << ch.width << ", height: " << ch.height << ", xoffset: " << ch.xoffset << ", yoffset: " << ch.yoffset << ", xadvance: " << ch.xadvance << ", page: " << ch.page << ", chnl: " << ch.chnl << std::endl;
            }
        }
    }
    void BMFontAsset::_parseBMPageTexture()
    {
        if (this->_pageFiles.empty())
        {
            return;
        }
        std::filesystem::path parentPath = "";
        if (!this->_path.empty())
        {
            parentPath = std::filesystem::path(this->_path).parent_path();
        }
        for (auto &[id, page] : this->_pageFiles)
        {
            // 根据文件名加载纹理资源
            if (page.texture)
            {
                continue;
            }
            if (page.file.empty())
            {
                continue;
            }
            std::string texturePath = page.file;
            if (!parentPath.empty())
            {
                texturePath = (std::filesystem::path(this->_path).parent_path() / texturePath).string();
            }
            Asset *asset = assetManager->getAsset(texturePath, true);
            if (asset)
            {
                page.texture = static_cast<TextureAsset *>(asset);
            }
            else
            {
                printf("加载BMFontPage纹理 %s 失败\n", texturePath.c_str());
            }
        }
    }
    const int BMFontAsset::getFontSize()
    {
        return this->_fontSize;
    }
    const float BMFontAsset::getCharWidth(wchar_t ch)
    {
        int id = ch;
        if (this->_glyphs.find(id) == this->_glyphs.end())
        {
            return 0.0f;
        }
        return this->_glyphs[id].xadvance;
    }
    BMGlyphData BMFontAsset::getGlyphData(wchar_t ch)
    {
        int id = ch;
        if (this->_glyphs.find(id) == this->_glyphs.end())
        {
            return BMGlyphData();
        }
        return this->_glyphs[id];
    }
    const float BMFontAsset::getLineHeight()
    {
        return this->_lineHeight;
    }
    TextureAsset *BMFontAsset::getTexture()
    {
        return this->_pageFiles[0].texture;
    }
    const int BMFontAsset::getTextureWidth()
    {
        return this->_textureWidth;
    }
    const int BMFontAsset::getTextureHeight()
    {
        return this->_textureHeight;
    }
    void BMFontAsset::destroy()
    {
    }
    BMFontAsset::~BMFontAsset()
    {
    }

} // namespace Boo