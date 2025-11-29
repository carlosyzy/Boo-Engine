#pragma once
#include "asset.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class TextureAsset : public Asset {
private:
  int _width = 0;
  int _height = 0;
  int _channels = 0;
  std::vector<uint8_t> _pixelsVector;

protected:
public:
  TextureAsset();
  TextureAsset(const std::string uuid);
  void create(std::string path) override;
  /**
   * @brief 创建纹理资产
   * @param width 纹理宽度
   * @param height 纹理高度
   * @param channels 纹理通道数
   * @param pixelsVector 像素数据
   */
  void create(int width, int height, int channels,
              std::vector<uint8_t> pixelsVector);
  /**
   * @brief 创建纹理资产
   * @param pixels 像素数据
   * @param size 像素数据大小
   */
  void create(const unsigned char *data, size_t size);

  //   /**
  //    * @brief 构造函数
  //    * @param uuid 资产唯一标识
  //    * @param path 资产路径
  //    */
  //   TextureAsset(const std::string uuid, const std::string path);
  //   TextureAsset(const std::string uuid, const unsigned char *data,
  //                const size_t size);
  //   /**
  //    * @brief 构造函数
  //    * @param uuid 资产唯一标识
  //    * @param width 纹理宽度
  //    * @param height 纹理高度
  //    * @param channels 纹理通道数
  //    * @param pixelsVector 像素数据
  //    */
  //   TextureAsset(const std::string uuid, int width, int height, int channels,
  //                std::vector<uint8_t> pixelsVector);

  /**
   * @brief 获取纹理宽度
   *
   * @return int
   */
  const int width() const { return _width; }
  /**
   * @brief 获取纹理高度
   *
   * @return int
   */
  const int height() const { return _height; }
  /**
   * @brief 获取纹理通道数
   *
   * @return int
   */
  int channels() const { return _channels; }

  void destroy() override;
  ~TextureAsset() {}
};