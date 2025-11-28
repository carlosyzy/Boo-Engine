#include "uuid-util.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

std::string UuidUtil::generateUUID()
{
    // // 随机数生成器问题
    // static std::random_device rd;
    // static std::mt19937_64 gen(rd());
    // static std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());
    // uint64_t randomPart = dis(gen);

    // // 时间戳 count() 返回的是纳秒数
    // auto timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    // std::ostringstream oss;
    // oss << std::hex << std::setfill('0')
    //     << std::setw(16) << timestamp
    //     << "-"
    //     << std::setw(16) << randomPart;
    // return oss.str();
    
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;

    uint64_t part1 = dis(gen);
    uint64_t part2 = dis(gen);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(8) << (part1 & 0xFFFFFFFF) << "-"
        << std::setw(4) << ((part1 >> 32) & 0xFFFF) << "-"
        << std::setw(4) << (((part2 >> 48) & 0x0FFF) | 0x4000) << "-" // 版本4
        << std::setw(4) << (((part2 >> 32) & 0x3FFF) | 0x8000) << "-" // 变体
        << std::setw(8) << (part2 & 0xFFFFFFFF)
        << std::setw(4) << ((part1 >> 48) & 0xFFFF);

    return oss.str();
}