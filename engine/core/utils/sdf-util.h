#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstdint>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

struct SDFStruct
{
    int dx;
    int dy;
    float dis;
    float sdfValue;
}; 

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};
class SDFUtil {
private:
    static void _initialize(const std::vector<uint8_t> &pixels,int width, int height,std::vector<SDFStruct> &sdfStructs);
    static void _updatePixel(std::vector<SDFStruct> &sdfStructs,int width, int height,int x,int y,int nx,int ny);
    static void _logSDF(const std::vector<SDFStruct> &sdfStructs, int width, int height);
    static void _logSDF(const std::vector<uint8_t> &pixels, int width, int height);
    
public:
    static std::vector<uint8_t> generateSDF(const std::vector<uint8_t> &pixels, int width, int height);
};

// class SDFGenerator {
// private:
//     int width, height;
//     std::vector<float> sdf;
//     std::vector<Point> nearestPoints;
//     std::vector<float> distSquared;

//     // 计算两点之间距离的平方
//     float distanceSquared(int x1, int y1, int x2, int y2) const {
//         int dx = x1 - x2;
//         int dy = y1 - y2;
//         return static_cast<float>(dx * dx + dy * dy);
//     }

//     // 检查坐标是否在图像范围内
//     bool inBounds(int x, int y) const {
//         return x >= 0 && x < width && y >= 0 && y < height;
//     }

//     // 更新当前像素的距离信息
//     void updatePixel(int x, int y, int nx, int ny) {
//         if (!inBounds(nx, ny)) return;

//         int idx = y * width + x;
//         int nidx = ny * width + nx;

//         // 获取邻居的最近点
//         Point neighborPoint = nearestPoints[nidx];
//         if (neighborPoint.x == -1) return; // 跳过无效点

//         // 计算从邻居最近点到当前点的距离平方
//         float d = distanceSquared(x, y, neighborPoint.x, neighborPoint.y);
        
//         // 如果找到更近的点，更新当前像素
//         if (d < distSquared[idx]) {
//             distSquared[idx] = d;
//             nearestPoints[idx] = neighborPoint;
//         }
//     }

// public:
//     SDFGenerator(int w, int h) : width(w), height(h) {
//         sdf.resize(width * height, 0.0f);
//         nearestPoints.resize(width * height, Point(-1, -1));
//         distSquared.resize(width * height, std::numeric_limits<float>::max());
//     }

//     // 生成SDF
//     void generateSDF(const std::vector<bool>& binaryImage) {
//         if (binaryImage.size() != width * height) {
//             std::cerr << "Error: Binary image size doesn't match dimensions!" << std::endl;
//             return;
//         }

//         // 初始化阶段
//         initialize(binaryImage);

//         // 第一次遍历：从左到右，从上到下
//         forwardPass();

//         // 第二次遍历：从右到左，从下到上
//         backwardPass();

//         // 计算最终的有向距离场
//         calculateFinalSDF(binaryImage);
//     }

//     // 获取生成的SDF
//     const std::vector<float>& getSDF() const {
//         return sdf;
//     }

// private:
//     void initialize(const std::vector<bool>& binaryImage) {
//         // 初始化距离和最近点
//         for (int y = 0; y < height; ++y) {
//             for (int x = 0; x < width; ++x) {
//                 int idx = y * width + x;
                
//                 if (binaryImage[idx]) { // 物体内部
//                     // 检查是否是边界点（4邻域内有背景）
//                     bool isBorder = false;
//                     if ((x > 0 && !binaryImage[y * width + (x - 1)]) ||        // 左
//                         (x < width - 1 && !binaryImage[y * width + (x + 1)]) || // 右
//                         (y > 0 && !binaryImage[(y - 1) * width + x]) ||        // 上
//                         (y < height - 1 && !binaryImage[(y + 1) * width + x])) { // 下
//                         isBorder = true;
//                     }

//                     if (isBorder) {
//                         distSquared[idx] = 0.0f;
//                         nearestPoints[idx] = Point(x, y);
//                     } else {
//                         distSquared[idx] = std::numeric_limits<float>::max();
//                         nearestPoints[idx] = Point(-1, -1);
//                     }
//                 } else { // 背景外部
//                     distSquared[idx] = std::numeric_limits<float>::max();
//                     nearestPoints[idx] = Point(-1, -1);
//                 }
//             }
//         }
//     }

//     void forwardPass() {
//         // 第一次遍历：从上到下，从左到右
//         for (int y = 0; y < height; ++y) {
//             for (int x = 0; x < width; ++x) {
//                 int idx = y * width + x;
                
//                 // 检查左上方向的邻居
//                 updatePixel(x, y, x - 1, y);     // 左
//                 updatePixel(x, y, x, y - 1);     // 上
//                 updatePixel(x, y, x - 1, y - 1); // 左上
//                 updatePixel(x, y, x + 1, y - 1); // 右上
//             }
//         }
//     }

//     void backwardPass() {
//         // 第二次遍历：从下到上，从右到左
//         for (int y = height - 1; y >= 0; --y) {
//             for (int x = width - 1; x >= 0; --x) {
//                 int idx = y * width + x;
                
//                 // 检查右下方向的邻居
//                 updatePixel(x, y, x + 1, y);     // 右
//                 updatePixel(x, y, x, y + 1);     // 下
//                 updatePixel(x, y, x + 1, y + 1); // 右下
//                 updatePixel(x, y, x - 1, y + 1); // 左下
//             }
//         }
//     }

//     void calculateFinalSDF(const std::vector<bool>& binaryImage) {
//         for (int y = 0; y < height; ++y) {
//             for (int x = 0; x < width; ++x) {
//                 int idx = y * width + x;
                
//                 if (distSquared[idx] < std::numeric_limits<float>::max()) {
//                     // 计算实际距离
//                     float distance = std::sqrt(distSquared[idx]);
                    
//                     // 添加符号：物体内部为正，背景外部为负
//                     sdf[idx] = binaryImage[idx] ? distance : -distance;
//                 } else {
//                     // 处理无穷远的情况（理论上不应该发生）
//                     sdf[idx] = binaryImage[idx] ? 
//                         std::numeric_limits<float>::max() : 
//                         -std::numeric_limits<float>::max();
//                 }
//             }
//         }
//     }
// };
