##################################################################################
# Author:          Mingxuan Li, Zhantong Zhu
# Acknowledgement: Cursor + Claude, GitHub Copilot
# Description:     Convert a 64-bit hex file to a 128-bit hex file by concatenating two lines.  
##################################################################################

def process_hex_file(file_path):
    with open(file_path, 'r') as f:
        lines = [line.strip() for line in f.readlines()]

    new_lines = []
    
    i = 0
    while i < len(lines):
        if (len(lines[i]) == 32):
            # If the line is already 128 bits, just add it to the new lines
            new_lines.append(lines[i])
            i += 1
            continue
        
        elif len(lines[i]) == 16:
            if (i + 1 < len(lines) and len(lines[i+1]) == 16):
                new_line = lines[i+1] + lines[i]
                new_lines.append(new_line)
                i += 2
            else:
                padding_needed = 32 - len(lines[i])
                new_line = '0' * padding_needed + lines[i]
                new_lines.append(new_line)
                i += 1
        else:
            # Last line is not 16 or 32 bits, just add it to the new lines with padding
            assert i == len(lines) - 1, f"Unexpected line length {len(lines[i])} at line {i+1}"
            padding_needed = 32 - len(lines[i])
            new_line = '0' * padding_needed + lines[i]
            new_lines.append(new_line)
            i += 1
    
    return new_lines

if __name__ == "__main__":
    file_path = 'build/program.hex'
    result = process_hex_file(file_path)
    print("Conversion finished!")
    with open('build/program_128b.hex', 'w') as f:
        for line in result:
            f.write(line + '\n')