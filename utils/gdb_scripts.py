
##################################################################################
# Author:          Mingxuan Li
# Acknowledgement: VSCode GitHub Copilot
# Description:     GDB Script Generator for RISC-V Debugging
##################################################################################

import os
import argparse

def generate_gdb_script(main_name, output_file):
    """
    生成GDB调试脚本
    """
    script_content = f"""# Auto-generated GDB script for {main_name}
# Author:           Mingxuan Li
# Acknowledgement:  VSCode GitHub Copilot

# 加载ELF文件
file bin/{main_name}.elf

# 加载程序到目标
load

# 在loop处设置断点
break loop

# 运行程序
continue

# 显示完整信息
echo {main_name.upper()} testing completed\\n
"""

    # 创建输出目录
    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    # 写入文件
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(script_content)


def main():
    parser = argparse.ArgumentParser(description='Generate GDB script for RISC-V debugging')
    parser.add_argument('MAIN', help='Main function file name (without .c extension)')

    args = parser.parse_args()

    # 生成GDB脚本
    output_file = os.path.join('scripts', f"{args.MAIN}.gdb")
    generate_gdb_script(args.MAIN, output_file)

if __name__ == "__main__":
    main()