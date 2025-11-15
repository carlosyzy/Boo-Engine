#include "file-util.h"
#include  "../../libs/stb/stb_image_write.h"



// 保存单通道数据为PGM文件（更高效，文件更小）
void FileUtil::saveAtlasAsPGM(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height)
{
    if (atlasData.size() != width * height)
    {
        throw std::runtime_error("Atlas data size doesn't match dimensions");
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // 写入PGM文件头
    file << "P5\n"
         << width << " " << height << "\n255\n";

    // 直接写入单通道数据
    file.write(reinterpret_cast<const char *>(atlasData.data()), atlasData.size());

    file.close();
}
void FileUtil::saveAtlasAsPNG(const std::string &filename,
                                 const std::vector<uint8_t> &atlasData,
                                 int width, int height)
{
    // 由于 atlasData 是 int8_t（有符号字符），我们需要转换为 unsigned char
    std::vector<unsigned char> unsignedData(atlasData.size());
    for (size_t i = 0; i < atlasData.size(); ++i)
    {
        // 将有符号转换为无符号，保持相同的位模式
        unsignedData[i] = static_cast<unsigned char>(atlasData[i]);
    }

    // 保存为 PNG
    int result = stbi_write_png(
        filename.c_str(),    // 文件名
        width,               // 宽度
        height,              // 高度
        4,                   // 通道数 (RGBA)
        unsignedData.data(), // 数据指针
        width * 4            // 每行的字节数 (宽度 * 4通道)
    );
}

