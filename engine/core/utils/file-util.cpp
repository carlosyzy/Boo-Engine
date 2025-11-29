#include "file-util.h"
#include "../../libs/stb/stb_image_write.h"


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
void FileUtil::saveJsonToBinary(const std::string &filename, const json &data)
{
    try
    {
        // 将JSON转换为MessagePack二进制
        std::vector<uint8_t> msgpack = json::to_msgpack(data);

        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            return;
        }
        // 写入二进制数据
        file.write(reinterpret_cast<const char *>(msgpack.data()), msgpack.size());
        if (file.good())
        {
            std::cerr << "保存成功: " << filename << std::endl;
        }
        else if (!file.fail())
        {
            std::cerr << "保存失败: " << filename << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "保存失败: " << e.what() << std::endl;
        return;
    }
}
// 从MessagePack加载
json FileUtil::readJsonFromBinary(const std::string &filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return json::object(); // 返回空对象
        }
        // 读取文件内容
        size_t file_size = file.tellg();
        file.seekg(0);

        std::vector<uint8_t> buffer(file_size);
        file.read(reinterpret_cast<char *>(buffer.data()), file_size);
        // 从MessagePack解析为JSON
        return json::from_msgpack(buffer);
    }
    catch (const std::exception &e)
    {
        std::cerr << "加载失败: " << e.what() << std::endl;
        return json::object();
    }
}
void FileUtil::saveJsonToText(const std::string &filename, const json &data)
{
    try
    {
        std::ofstream json_file(filename);
        json_file << data.dump(4);
    }
    catch (const std::exception &e)
    {
        std::cerr << "保存失败: " << e.what() << std::endl;
        return;
    }
}

void FileUtil::copyFile(const std::string &src, const std::string &dst)
{
    try
    {
        std::cout << "FileUtil::copyFile: " << src << " -> " << dst << std::endl;
        // 检查源文件是否存在
        if (!std::filesystem::exists(src))
        {
            std::cerr << "源文件不存在: " << src << std::endl;
            return;
        }
        // 创建目标目录（如果不存在）
        std::filesystem::create_directories(std::filesystem::path(dst).parent_path());
        // 拷贝文件
        std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing);

        std::cout << "文件拷贝成功: " << std::endl;
        return;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "文件拷贝错误: " << e.what() << std::endl;
        return;
    }
}
void FileUtil::removeFile(const std::string &path)
{
    try
    {
        // 检查文件是否存在
        if (!std::filesystem::exists(path))
        {
            std::cerr << "文件不存在: " << path << std::endl;
            return;
        }
        // 删除文件
        std::filesystem::remove(path);
        std::cout << "文件删除成功: " << path << std::endl;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "文件删除错误: " << e.what() << std::endl;
    }
    //  if (std::filesystem::exists(libraryPath))
    //             {
    //                 std::filesystem::remove(libraryPath);
    //             }
}

// static bool saveBinary(const json& data, const std::string& filename) {
//         try {
//             // 将JSON转换为MessagePack二进制
//             std::vector<uint8_t> msgpack = json::to_msgpack(data);

//             std::ofstream file(filename, std::ios::binary);
//             if (!file.is_open()) return false;

//             // 写入二进制数据
//             file.write(reinterpret_cast<const char*>(msgpack.data()), msgpack.size());
//             return !file.fail();

//         } catch (const std::exception& e) {
//             std::cerr << "保存失败: " << e.what() << std::endl;
//             return false;
//         }
//     }

//     // 同时保存JSON和二进制版本（用于调试）
//     static bool saveBoth(const json& data, const std::string& base_filename) {
//         // 保存可读的JSON（调试用）
//         std::ofstream json_file(base_filename + ".json");
//         json_file << data.dump(4);

//         // 保存二进制的MessagePack（生产用）
//         return saveBinary(data, base_filename + ".bin");
//     }