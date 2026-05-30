#include "file-util.h"
#include "libs/stb/stb_image_write.h"
#include "log.h"
namespace Boo
{

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
                std::cout << "File :Save Success:: " << filename << std::endl;
            }
            else
            {
                std::cerr << "File :Save Failed:: " << filename << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "File :Save Failed:: " << e.what() << std::endl;
            return;
        }
    }
    // 读取文本文件内容
    std::string FileUtil::readTextFile(const std::string &filename)
    {
        try
        {
            std::ifstream file(filename);
            if (!file.is_open())
            {
                return ""; // 返回空字符串
            }
            // 读取文件内容
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
        catch (const std::exception &e)
        {
            std::cerr << "File :Load Failed:: " << e.what() << std::endl;
            return ""; // 返回空字符串
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
            std::cerr << "File :Load Failed:: " << e.what() << std::endl;
            return json::object();
        }
    }
    json FileUtil::readJsonFromText(const std::string &filename)
    {
        try
        {
            // 检查文件是否存在
            if (!std::filesystem::exists(filename))
            {
                std::cerr << "File :Load Failed:: not exist: " << filename << std::endl;
                return json::object();
            }

            std::ifstream json_file(filename);
            json data = json::parse(json_file);
            return data;
        }
        catch (const std::exception &e)
        {
            std::cerr << "File :Load Failed:: " << e.what() << std::endl;
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
            std::cerr << "File :Save Failed:: " << e.what() << std::endl;
            return;
        }
    }

    void FileUtil::copyFile(const std::string &src, const std::string &dst)
    {
        try
        {
            // 检查源文件是否存在
            if (!std::filesystem::exists(src))
            {
                std::cerr << "File :copyFile:: src not exist: " << src << std::endl;
                return;
            }
            // 创建目标目录（如果不存在）
            std::filesystem::create_directories(std::filesystem::path(dst).parent_path());
            // 拷贝文件
            std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing);
            return;
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "File :copyFile:: Failed: " << e.what() << std::endl;
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
                std::cerr << "File :removeFile:: not exist: " << path << std::endl;
                return;
            }
            // 删除文件
            std::filesystem::remove(path);
            std::cout << "File :removeFile:: Success: " << path << std::endl;
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "File :removeFile:: Failed: " << e.what() << std::endl;
        }
    }

} // namespace Boo