##################################################################################
# Author:          Mingxuan Li
# Acknowledgement: Cursor + Claude
# Description:     Convert assembly code to hex code for simulation
##################################################################################

import os
import sys

if len(sys.argv) != 3:
    print("Usage: python asm2hex.py <input_asm_file> <output_hex_file>")
    sys.exit(1)

input_asm_file = sys.argv[1]
output_hex_file = sys.argv[2]

if not os.path.isfile(input_asm_file):
    print(f"Error: {input_asm_file} does not exist.")
    sys.exit(1)

data_sections = ['.text', '.rodata']
data = {'.text': [], '.rodata': []}
current_section = None

def big_to_little_endian(hex_str):
    # 将大端表示的16进制字符串转换为小端表示的16进制字符串
    byte_array = bytearray.fromhex(hex_str)
    byte_array.reverse()
    return ''.join(format(x, '02x') for x in byte_array)

with open(input_asm_file, 'r') as f:
    for line in f:
        line = line.strip()
        # 检查是否进入新的段
        if line.startswith('Contents of section'):
            for sec in data_sections:
                if line.endswith(f'{sec}:'):
                    current_section = sec
                    break
            else:
                current_section = None
        # 如果在目标段内，处理数据行
        elif current_section:
            if line:
                parts = line.split()
                if len(parts) >= 5:
                    data_values = [big_to_little_endian(val) for val in parts[1:5] if all(c in '0123456789abcdefABCDEF' for c in val)]  # 提取并转换4个数据
                    data[current_section].append(data_values)
                elif len(parts) > 1:
                    address = parts[0]
                    data_values = [big_to_little_endian(val) for val in parts[1:] if all(c in '0123456789abcdefABCDEF' for c in val)]  # 提取并转换所有数据
                    data[current_section].append(data_values)
            else:
                # 空行表示段的结束
                current_section = None

# 将数据两两配对并输出到文件
with open(output_hex_file, 'w') as output_file:
    for section in data_sections:
        for data_values in data[section]:
            # 检查data_values的数目是否为偶数，如果不是，补齐0
            if len(data_values) % 2 != 0:
                data_values.append('00000000')
            for i in range(0, len(data_values), 2):
                paired_value = data_values[i+1] + data_values[i]
                output_file.write(paired_value + '\n')