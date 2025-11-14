#include "sdf-util.h"
#include <cmath>
#include "file-util.h"
std::vector<uint8_t> SDFUtil::generateSDF(const std::vector<uint8_t> &pixels, int width, int height)
{
    std::vector<uint8_t> outDatas(width * height, 0);
    std::vector<SDFStruct> sdfStructs(width * height);
    // 初始化SDFStruct
    SDFUtil::_initialize(pixels, width, height, sdfStructs);
    // SDFUtil::_logSDF(sdfStructs, width, height);
    // 第一次遍历：从左到右，从上到下
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int idx = y * width + x;
            // 更新当前像素的距离信息
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x - 1, y);     // 左
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x, y - 1);     // 上
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x - 1, y - 1); // 左上
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x + 1, y - 1); // 右上
        }
    }
    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = width - 1; x >= 0; --x)
        {
            int idx = y * width + x;

            // 检查右下方向的邻居
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x + 1, y);     // 右
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x, y + 1);     // 下
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x + 1, y + 1); // 右下
            SDFUtil::_updatePixel(sdfStructs, width, height, x, y, x - 1, y + 1); // 左下
        }
    }
    float maxDistance = 0;
    // 计算最终的有向距离场
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int idx = y * width + x;
            if (sdfStructs[idx].dis < std::numeric_limits<float>::max())
            {
                // 计算实际距离
                // 物体内部为正，背景外部为负
                float distance = std::sqrt(sdfStructs[idx].dis);
                sdfStructs[idx].sdfValue = pixels[idx] > 128 ? distance : -distance;
            }else{
                //处理无穷远的情况（理论上不应该发生）
                sdfStructs[idx].sdfValue = pixels[idx] > 128 ?  std::numeric_limits<float>::max()  : -std::numeric_limits<float>::max();
            }
            // 找到最大距离
            if (std::abs(sdfStructs[idx].sdfValue) > maxDistance)
            {
                maxDistance = std::abs(sdfStructs[idx].sdfValue);
            }
        }
    }
    // SDFUtil::_logSDF(sdfStructs, width, height);
    //归一化 颜色值
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int idx = y * width + x;
            float normalized = (maxDistance > 0) ? (sdfStructs[idx].sdfValue / maxDistance) : 0;
            normalized = std::max(-1.0f, std::min(1.0f, normalized));
            outDatas[idx] = static_cast<uint8_t>((normalized + 1.0f) * 0.5f * 255);
        }
    }
    // SDFUtil::_logSDF(outDatas, width, height);
    // FileUtil::saveAtlasAsPGM("sdf1.pgm", outDatas, width, height);
    return outDatas;
}
/**
 * @brief 初始化SDFStruct
 *
 */
void SDFUtil::_initialize(const std::vector<uint8_t> &pixels, int width, int height, std::vector<SDFStruct> &sdfStructs)
{
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            int index = y * width + x;
            sdfStructs[index].dx = -1;
            sdfStructs[index].dy = -1;
            sdfStructs[index].dis = std::numeric_limits<float>::max();
            sdfStructs[index].sdfValue = 0.0f;
        }
    }
    // SDFUtil::_logSDF(sdfStructs, width, height);
    // 初始化距离和最近点
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            int index = y * width + x;
            if (pixels[index] >= 128)
            {
                // 内部
                //  检查是否是边界点（4邻域内有背景）
                bool isBorder = false;
                if ((x > 0 && pixels[y * width + (x - 1)] < 128) ||
                    (x < width - 1 && pixels[y * width + (x + 1)] < 128) ||
                    (y > 0 && pixels[(y - 1) * width + x] < 128) ||
                    (y < height - 1 && pixels[(y + 1) * width + x] < 128))
                {
                    isBorder = true;
                }
                if (isBorder)
                {
                    sdfStructs[index].dx = x;
                    sdfStructs[index].dy = y;
                    sdfStructs[index].dis = 0.0f;
                }
                else
                {
                    // 内部
                    sdfStructs[index].dx = -1;
                    sdfStructs[index].dy = -1;
                    sdfStructs[index].dis = std::numeric_limits<float>::max();
                }
            }
            else
            {
                // 外部
                sdfStructs[index].dx = -1;
                sdfStructs[index].dy = -1;
                sdfStructs[index].dis = std::numeric_limits<float>::max();
            }
        }
    }
}
void SDFUtil::_updatePixel(std::vector<SDFStruct> &sdfStructs, int width, int height, int x, int y, int nx, int ny)
{
    if (nx < 0 || nx >= width || ny < 0 || ny >= height)
    {
        return;
    }
    int idx = y * width + x;    // 当前像素的索引
    int nidx = ny * width + nx; // 邻居像素的索引
                                //  std::cout<<"now x:"<<x<<" y:"<<y<<" nx:"<<nx<<" ny:"<<ny<<std::endl;
    SDFStruct &neighbor = sdfStructs[nidx];
    if (neighbor.dx == -1)
    {
        return;
    }
    SDFStruct &current = sdfStructs[idx];
    //  计算从邻居最近点到当前点的距离平方
    float distanceSquared = (x - neighbor.dx) * (x - neighbor.dx) + (y - neighbor.dy) * (y - neighbor.dy);
    if (distanceSquared < current.dis)
    {
        current.dis = distanceSquared;
        current.dx = neighbor.dx;
        current.dy = neighbor.dy;
    }
}
/**
 * @brief 日志SDF
 *
 * @param sdfStructs
 * @param width
 * @param height
 */
void SDFUtil::_logSDF(const std::vector<SDFStruct> &sdfStructs, int width, int height)
{
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            const int index = y * width + x;
            const SDFStruct &sdf = sdfStructs[index];
            std::string color = RESET;
            if (sdf.dis <= 0.0)
                color = RED;
            std::string sdfInfo = std::to_string(sdf.dis);
            // if (sdf.dis > 1000.0)
            //     sdfInfo = "x";
            // 根据SDFStruct的内容输出，这里假设你有一个数值字段叫value
            std::cout << color << std::setw(6) << std::fixed << std::setprecision(1) << sdf.sdfValue << RESET;
        }
        std::cout << std::endl; // 每行结束后换行
    }
}
/**
 * @brief 日志SDF
 *
 * @param sdfStructs
 * @param width
 * @param height
 */
void SDFUtil::_logSDF(const std::vector<uint8_t> &pixels, int width, int height)
{
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            const int index = y * width + x;
            const uint8_t &pixel = pixels[index];
            std::string color = RESET;
            if (pixel >= 128)
                color = RED;
            // 根据SDFStruct的内容输出，这里假设你有一个数值字段叫value
            std::cout << color << std::setw(10) << std::fixed << std::setprecision(3) << (int)pixel << RESET;
        }
        std::cout << std::endl; // 每行结束后换行
    }
}
