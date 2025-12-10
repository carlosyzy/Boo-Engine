#!/usr/bin/env python3
"""
SPIR-V to C++ Array Converter
将编译后的 SPIR-V shader 文件转换为 C++ 数组代码
"""

import sys
import os
from pathlib import Path
import glob


def spv_to_cpp(spv_file_path, output_file_path=None, variable_name=None):
    """
    将 SPIR-V 文件转换为 C++ 数组

    Args:
        spv_file_path: 输入的 .spv 文件路径
        output_file_path: 输出的 .h 文件路径（可选）
        variable_name: C++ 数组变量名（可选，默认根据文件名生成）
    """
    spv_path = Path(spv_file_path)

    if not spv_path.exists():
        print(f"错误: 文件不存在 '{spv_file_path}'")
        return False

    # 生成变量名
    if variable_name is None:
        # 例如: ui-mask.frag.spv -> ui_mask_frag_spv
        variable_name = spv_path.stem.replace('-', '_').replace('.', '_')
        if spv_path.suffix == '.spv':
            variable_name += '_spv'

    # 生成输出文件路径
    if output_file_path is None:
        output_file_path = spv_path.with_suffix('.h')

    # 读取 SPIR-V 文件（二进制）
    with open(spv_path, 'rb') as f:
        spv_data = f.read()

    # 生成头文件保护宏
    guard_name = variable_name.upper() + '_H'

    # 生成 C++ 代码
    cpp_code = f"""// Auto-generated from {spv_path.name}
// Do not edit manually

#pragma once

#include <cstdint>

// Shader: {spv_path.name}
// Size: {len(spv_data)} bytes
static const uint32_t {variable_name}[] = {{
"""

    # 将字节数据转换为 uint32_t 数组
    # SPIR-V 是以 uint32_t 为单位的
    uint32_count = len(spv_data) // 4

    for i in range(uint32_count):
        # 读取4个字节，按小端序组合成 uint32_t
        offset = i * 4
        value = (spv_data[offset] |
                 (spv_data[offset + 1] << 8) |
                 (spv_data[offset + 2] << 16) |
                 (spv_data[offset + 3] << 24))

        # 每行8个数字
        if i % 8 == 0:
            cpp_code += "    "

        cpp_code += f"0x{value:08x}"

        if i < uint32_count - 1:
            cpp_code += ", "

        if (i + 1) % 8 == 0:
            cpp_code += "\n"

    if uint32_count % 8 != 0:
        cpp_code += "\n"

    cpp_code += f"""}};\n
static const uint32_t {variable_name}_size = sizeof({variable_name});
static const uint32_t {variable_name}_count = {uint32_count};
"""

    # 写入头文件
    with open(output_file_path, 'w') as f:
        f.write(cpp_code)

    print(f"✓ 转换成功:")
    print(f"  输入: {spv_path}")
    print(f"  输出: {output_file_path}")
    print(f"  变量名: {variable_name}")
    print(f"  大小: {len(spv_data)} bytes ({uint32_count} uint32_t)")

    return True


def select_spv_file():
    """
    交互式选择 .spv 文件
    返回选择的文件路径，如果取消则返回 None
    """
    # 查找当前目录及子目录下的所有 .spv 文件
    spv_files = []

    # 当前目录
    spv_files.extend(glob.glob("*.spv"))
    # 子目录
    spv_files.extend(glob.glob("**/*.spv", recursive=True))

    if not spv_files:
        print("错误: 当前目录下没有找到 .spv 文件")
        return None

    # 去重并排序
    spv_files = sorted(list(set(spv_files)))

    print("=" * 60)
    print("找到以下 SPIR-V shader 文件:")
    print("=" * 60)

    for i, file in enumerate(spv_files, 1):
        file_size = os.path.getsize(file)
        print(f"  [{i}] {file:40s} ({file_size:6d} bytes)")

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

            if 1 <= idx <= len(spv_files):
                selected_file = spv_files[idx - 1]
                print(f"\n已选择: {selected_file}")
                return selected_file
            else:
                print(f"错误: 请输入 0-{len(spv_files)} 之间的数字")

        except ValueError:
            print("错误: 请输入有效的数字")
        except KeyboardInterrupt:
            print("\n\n已取消")
            return None


def main():
    print("\n╔════════════════════════════════════════════════════════════╗")
    print("║       SPIR-V to C++ Array Converter                      ║")
    print("║       将 SPIR-V shader 转换为 C++ 数组                   ║")
    print("╚════════════════════════════════════════════════════════════╝\n")

    # 如果命令行提供了参数，使用命令行模式
    if len(sys.argv) >= 2:
        spv_file = sys.argv[1]
        output_file = sys.argv[2] if len(sys.argv) > 2 else None
        var_name = sys.argv[3] if len(sys.argv) > 3 else None
    else:
        # 否则使用交互式模式
        spv_file = select_spv_file()
        if spv_file is None:
            return 0

        # 使用默认配置
        output_file = None
        var_name = None

    print("\n开始转换...\n")
    success = spv_to_cpp(spv_file, output_file, var_name)

    if success:
        print("\n✓ 转换完成!\n")

    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main())
