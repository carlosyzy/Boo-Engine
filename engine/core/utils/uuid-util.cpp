#include "uuid-util.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

std::string UuidUtil::generateUUID()
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    auto timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    uint64_t randomPart = dis(gen);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(16) << timestamp
        << "-"
        << std::setw(16) << randomPart;

    return oss.str();
}

//  static size_t _fnv1aHash(const std::string& str);
//      static std::string hash(const std::string &input);

// size_t SHA::_fnv1aHash(const std::string& str) {
//     const size_t prime = 0x100000001b3;
//     size_t hash = 0xcbf29ce484222325;

//     for (char c : str) {
//         hash ^= c;
//         hash *= prime;
//     }
//     return hash;
// }
// /**
//  * 双哈希key 生成
//  */
// std::string SHA::hash(const std::string& str) {
//     size_t h1 = std::hash<std::string>{}(str);
//     size_t h2 = SHA::_fnv1aHash(str);
//     return std::to_string(h1) + "_" + std::to_string(h2);
// }
