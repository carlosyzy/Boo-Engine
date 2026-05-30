#!/usr/bin/env python3
"""
Image to C++ Array Converter
将图片文件转换为 C++ 数组代码，支持 stbi_load_from_memory 加载
"""

import sys
import os
from pathlib import Path
import glob


def image_to_cpp(image_file_path, output_file_path=None, variable_name=None, namespace="EmbeddedImages"):
    """
    将图片文件转换为 C++ 数组

    Args:
        image_file_path: 输入的图片文件路径
        output_file_path: 输出的 .h 文件路径（可选）
        variable_name: C++ 数组变量名（可选，默认根据文件名生成）
        namespace: C++ 命名空间（可选，默认为 "EmbeddedImages"）
    """
    image_path = Path(image_file_path)

    if not image_path.exists():
        print(f"错误: 文件不存在 '{image_file_path}'")
        return False

    # 检查文件格式
    valid_extensions = {'.jpg', '.jpeg', '.png', '.bmp', '.tga', '.psd', '.gif', '.hdr', '.pic'}
    if image_path.suffix.lower() not in valid_extensions:
        print(f"警告: 文件格式 '{image_path.suffix}' 可能不是支持的图片格式")
        print(f"支持的格式: {', '.join(valid_extensions)}")

    # 生成变量名
    if variable_name is None:
        # 例如: ui-background.jpg -> ui_background_jpg
        variable_name = image_path.stem.replace('-', '_').replace('.', '_')
        if image_path.suffix:
            suffix = image_path.suffix.lower().lstrip('.')
            variable_name += f'_{suffix}'
        variable_name = variable_name.upper() + '_DATA'

    # 生成输出文件路径
    if output_file_path is None:
        output_file_path = image_path.with_suffix('.h')

    # 读取图片文件（二进制）
    with open(image_path, 'rb') as f:
        image_data = f.read()

    # 生成头文件保护宏
    guard_name = variable_name.upper() + '_H'

    # 生成 C++ 代码 - 头文件
    header_code = f"""// Auto-generated from {image_path.name}
// Do not edit manually
// 使用 stbi_load_from_memory 加载此数据

#ifndef {guard_name}
#define {guard_name}

#include <cstddef>

namespace {namespace} {{
    // 图片: {image_path.name}
    // 原始大小: {len(image_data)} bytes
    extern const unsigned char {variable_name}[];
    extern const size_t {variable_name}_SIZE;
}} // namespace {namespace}

#endif // {guard_name}
"""

    # 生成 C++ 代码 - 实现文件
    cpp_file_path = output_file_path.with_suffix('.cpp')
    cpp_code = f"""// Auto-generated from {image_path.name}
// Do not edit manually

#include "{output_file_path.name}"

namespace {namespace} {{

// 图片: {image_path.name}
// 原始大小: {len(image_data)} bytes
const unsigned char {variable_name}[] = {{
"""

    # 将字节数据转换为十六进制数组
    bytes_per_line = 16
    total_bytes = len(image_data)

    for i in range(0, total_bytes, bytes_per_line):
        line_bytes = image_data[i:i + bytes_per_line]
        hex_values = ', '.join(f'0x{byte:02x}' for byte in line_bytes)
        cpp_code += f"    {hex_values}"

        if i + bytes_per_line < total_bytes:
            cpp_code += ","

        cpp_code += "\n"

    cpp_code += f"""}};

const size_t {variable_name}_SIZE = {total_bytes};

}} // namespace {namespace}
"""

    # 写入头文件
    with open(output_file_path, 'w') as f:
        f.write(header_code)

    # 写入实现文件
    with open(cpp_file_path, 'w') as f:
        f.write(cpp_code)

    print(f"✓ 转换成功:")
    print(f"  输入: {image_path}")
    print(f"  输出: {output_file_path}")
    print(f"  实现: {cpp_file_path}")
    print(f"  变量名: {namespace}::{variable_name}")
    print(f"  大小: {total_bytes} bytes")
    print(f"  命名空间: {namespace}")

    return True


def select_image_file():
    """
    交互式选择图片文件
    返回选择的文件路径，如果取消则返回 None
    """
    # 支持的图片格式
    image_extensions = ['*.jpg', '*.jpeg', '*.png', '*.bmp', '*.tga', '*.psd', '*.gif', '*.hdr', '*.pic']
    
    # 查找当前目录及子目录下的所有图片文件
    image_files = []

    # 当前目录
    for ext in image_extensions:
        image_files.extend(glob.glob(ext))
        image_files.extend(glob.glob(ext.upper()))

    # 子目录
    for ext in image_extensions:
        image_files.extend(glob.glob(f"**/{ext}", recursive=True))
        image_files.extend(glob.glob(f"**/{ext.upper()}", recursive=True))

    if not image_files:
        print("错误: 当前目录下没有找到支持的图片文件")
        print("支持的格式: JPG, JPEG, PNG, BMP, TGA, PSD, GIF, HDR, PIC")
        return None

    # 去重并排序
    image_files = sorted(list(set(image_files)))

    print("=" * 60)
    print("找到以下图片文件:")
    print("=" * 60)

    for i, file in enumerate(image_files, 1):
        file_size = os.path.getsize(file)
        file_ext = Path(file).suffix.upper()
        print(f"  [{i}] {file:40s} {file_ext:>6s} ({file_size:8d} bytes)")

    print("=" * 60)
    print("  [0] 退出")
    print("=" * 60)

    while True:
        try:
            choice = input("\n请输入文件编号: ").strip()

            if not choice:
                continue

            idx = int(choice)

            if idx == 0:
                print("已取消")
                return None

            if 1 <= idx <= len(image_files):
                selected_file = image_files[idx - 1]
                print(f"\n已选择: {selected_file}")
                return selected_file
            else:
                print(f"错误: 请输入 0-{len(image_files)} 之间的数字")

        except ValueError:
            print("错误: 请输入有效的数字")
        except KeyboardInterrupt:
            print("\n\n已取消")
            return None


def batch_convert_images(directory="."):
    """
    批量转换目录下的所有图片文件
    """
    image_extensions = ['*.jpg', '*.jpeg', '*.png', '*.bmp', '*.tga', '*.psd', '*.gif', '*.hdr', '*.pic']
    image_files = []

    for ext in image_extensions:
        image_files.extend(glob.glob(os.path.join(directory, ext)))
        image_files.extend(glob.glob(os.path.join(directory, ext.upper())))

    if not image_files:
        print("错误: 指定目录下没有找到支持的图片文件")
        return False

    success_count = 0
    for image_file in sorted(image_files):
        print(f"\n处理: {image_file}")
        if image_to_cpp(image_file):
            success_count += 1

    print(f"\n✓ 批量转换完成: {success_count}/{len(image_files)} 个文件转换成功")
    return success_count > 0


def main():
    print("\n╔════════════════════════════════════════════════════════════╗")
    print("║           Image to C++ Array Converter                    ║")
    print("║             将图片文件转换为 C++ 数组                     ║")
    print("║         支持 stbi_load_from_memory 加载                   ║")
    print("╚════════════════════════════════════════════════════════════╝\n")

    # 如果命令行提供了参数，使用命令行模式
    if len(sys.argv) >= 2:
        # 批量转换模式
        if sys.argv[1] == "--batch" or sys.argv[1] == "-b":
            directory = sys.argv[2] if len(sys.argv) > 2 else "."
            return 0 if batch_convert_images(directory) else 1
        
        # 单个文件转换
        image_file = sys.argv[1]
        output_file = sys.argv[2] if len(sys.argv) > 2 else None
        var_name = sys.argv[3] if len(sys.argv) > 3 else None
        namespace = sys.argv[4] if len(sys.argv) > 4 else "EmbeddedImages"
    else:
        # 交互式模式
        print("选择模式:")
        print("  1. 单个文件转换")
        print("  2. 批量转换当前目录")
        print("  0. 退出")
        
        while True:
            try:
                choice = input("\n请选择模式 [1]: ").strip()
                if not choice:
                    choice = "1"
                
                if choice == "0":
                    print("已退出")
                    return 0
                elif choice == "1":
                    image_file = select_image_file()
                    if image_file is None:
                        return 0
                    break
                elif choice == "2":
                    return 0 if batch_convert_images() else 1
                else:
                    print("错误: 请输入 0, 1 或 2")
            except (ValueError, KeyboardInterrupt):
                print("错误: 请输入有效的选择")

        # 使用默认配置
        output_file = None
        var_name = None
        namespace = "EmbeddedImages"

    print("\n开始转换...\n")
    success = image_to_cpp(image_file, output_file, var_name, namespace)

    if success:
        print("\n✓ 转换完成!")
        print("\n使用示例:")
        print(f"  #include \"{Path(image_file).with_suffix('.h').name}\"")
        print(f"  auto image_data = EmbeddedImages::{variable_name if 'variable_name' in locals() else 'YOUR_VARIABLE_NAME'};")
        print(f"  auto image_size = EmbeddedImages::{variable_name if 'variable_name' in locals() else 'YOUR_VARIABLE_NAME'}_SIZE;")
        print("\n使用 stbi_load_from_memory 加载:")
        print("  stbi_load_from_memory(image_data, image_size, &width, &height, &channels, 0);")

    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main())