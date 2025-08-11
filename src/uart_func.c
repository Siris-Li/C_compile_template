//////////////////////////////////////////////////////////////////////////////////
// Author:          Mingxuan Li
// Acknowledgement: GitHub Copilot
// Description:     UART Function Test
//////////////////////////////////////////////////////////////////////////////////

#include "uart.h"
#include <stdint.h>
#include <stddef.h>

// 测试数据
static const char* test_strings[] = {
    "Hello World from CVA6!\n",
    "UART Test String 1\n",
    "UART Test String 2\n",
    "Special chars: !@#$%^&*()\n",
    "Numbers: 0123456789\n",
    "End of test\n"
};

// 测试整数输出
void test_uart_32b() {
    print_uart("=== Print 32-Bit Data Test ===\n");

    uint32_t test_ints[] = {0x12345678, 0xABCDEF00, 0xDEADBEEF, 0x00000001, 0xFFFFFFFF};

    for (int i = 0; i < 5; i++) {
        print_uart("Int ");
        print_uart_char(i + '0');
        print_uart("\n");

        print_uart("  Hex: 0x");
        print_uart_hex_32b(test_ints[i]);
        print_uart("\n");

        print_uart("  Dec: ");
        print_uart_dec_32b(test_ints[i]);
        print_uart("\n");

        print_uart("  Bin: ");
        print_uart_bin_32b(test_ints[i]);
        print_uart("\n\n");
    }
}

// 测试地址输出
void test_uart_64b() {
    print_uart("=== Print 64-Bit Data Test ===\n");

    uint64_t test_addrs[] = {
        0x80000000,
        0x10000000,
        0xa0000000,
        0x1234567890abcdef,
        0xfedcba0987654321
    };

    for (int i = 0; i < 5; i++) {
        print_uart("Addr ");
        print_uart_char(i + '0');
        print_uart("\n");

        print_uart("  Hex: 0x");
        print_uart_hex_64b(test_addrs[i]);
        print_uart("\n");

        print_uart("  Dec: ");
        print_uart_dec_64b(test_addrs[i]);
        print_uart("\n");

        print_uart("  Bin: ");
        print_uart_bin_64b(test_addrs[i]);
        print_uart("\n\n");
    }
}

// 测试字节输出
void test_uart_bytes() {
    print_uart("=== Byte Test ===\n");

    uint8_t test_bytes[] = {0x00, 0xFF, 0xAA, 0x55, 0x12, 0x34, 0x56, 0x78};

    print_uart("Bytes: ");
    for (int i = 0; i < 8; i++) {
        print_uart("0x");
        print_uart_byte(test_bytes[i]);
        print_uart(" ");
    }
    print_uart("\n");
}

// 测试内存写入和UART输出结合
void test_memory_uart() {
    print_uart("=== Memory & UART Test ===\n");

    // 写入测试数据到内存
    uint64_t* mem_base = (uint64_t*)0x30000000;

    print_uart("Writing test pattern to memory at 0x");
    print_uart_hex_64b((uint64_t)mem_base);
    print_uart("\n");

    // 写入测试模式
    for (int i = 0; i < 4; i++) {
        *(mem_base + i) = (uint64_t)(0x1122334455667788ULL + i);

        print_uart("Wrote to offset ");
        print_uart_char(i + '0');
        print_uart(": 0x");
        print_uart_hex_64b(*(mem_base + i));
        print_uart("\n");
    }

    print_uart("Reading back from memory:\n");
    for (int i = 0; i < 4; i++) {
        uint64_t read_value = *(mem_base + i);
        print_uart("Read from offset ");
        print_uart_char(i + '0');
        print_uart(": 0x");
        print_uart_hex_64b(read_value);
        print_uart("\n");
    }
}

// 压力测试 - 发送大量数据
void test_uart_stress() {
    print_uart("=== Stress Test ===\n");

    for (int i = 0; i < 10; i++) {
        print_uart("Stress test iteration ");
        print_uart_char((i / 10) + '0');
        print_uart_char((i % 10) + '0');
        print_uart(": ");

        // 发送一串连续字符
        for (char c = 'A'; c <= 'Z'; c++) {
            print_uart_char(c);
        }
        print_uart("\n");
    }
}

// 测试串行读取功能
void test_load_uart_char() {
    print_uart("=== Read Test ===\n");
    print_uart("Type 5 characters (will echo back):\n");

    uint8_t received_char;
    int char_count = 0;

    while (char_count < 5) {
        if (load_uart_char(&received_char)) {
            print_uart("Received: ");
            print_uart_byte(received_char);
            print_uart(" ('");
            print_uart_char(received_char); // Echo the character
            print_uart("')\n");
            char_count++;
        }
        // 简单延时，避免过度轮询
        for (volatile int i = 0; i < 1000; i++);
    }
}

// 测试load_uart_byte函数
void test_load_uart_byte() {
    print_uart("=== Load Byte Test ===\n");
    print_uart("Please input 3 bytes in hex format (e.g., FF A0 12):\n");

    uint8_t loaded_bytes[3];

    for (int i = 0; i < 3; i++) {
        print_uart("Enter byte ");
        print_uart_char(i + '0');
        print_uart(" (2 hex digits): ");

        load_uart_byte(&loaded_bytes[i]);

        print_uart("Loaded: 0x");
        print_uart_byte(loaded_bytes[i]);
        print_uart("\n");
    }

    print_uart("Summary - Loaded bytes: ");
    for (int i = 0; i < 3; i++) {
        print_uart("0x");
        print_uart_byte(loaded_bytes[i]);
        print_uart(" ");
    }
    print_uart("\n");
}

// 测试load_uart_32b函数
void test_load_uart_32b() {
    print_uart("=== Load 32-Bit Data Test ===\n");
    print_uart("Please input 2 integers in hex format (8 digits each):\n");

    uint32_t loaded_ints[2];

    for (int i = 0; i < 2; i++) {
        print_uart("Enter 32-bit integer ");
        print_uart_char(i + '0');
        print_uart(" (8 hex digits): ");

        load_uart_32b(&loaded_ints[i]);

        print_uart("Loaded: 0x");
        print_uart_hex_32b(loaded_ints[i]);
        print_uart("\n");
    }

    print_uart("Summary - Loaded integers:\n");
    for (int i = 0; i < 2; i++) {
        print_uart("Int ");
        print_uart_char(i + '0');
        print_uart(": 0x");
        print_uart_hex_32b(loaded_ints[i]);
        print_uart("\n");
    }
}

// 测试load_uart_64b函数
void test_load_uart_64b() {
    print_uart("=== Load 64-Bit Test ===\n");
    print_uart("Please input 2 addresses in hex format (16 digits each):\n");

    uint64_t loaded_addrs[2];

    for (int i = 0; i < 2; i++) {
        print_uart("Enter 64-bit address ");
        print_uart_char(i + '0');
        print_uart(" (16 hex digits): ");

        loaded_addrs[i] = 0; // 初始化为0
        load_uart_64b(&loaded_addrs[i]);

        print_uart("Loaded: 0x");
        print_uart_hex_64b(loaded_addrs[i]);
        print_uart("\n");
    }

    print_uart("Summary - Loaded addresses:\n");
    for (int i = 0; i < 2; i++) {
        print_uart("Addr ");
        print_uart_char(i + '0');
        print_uart(": 0x");
        print_uart_hex_64b(loaded_addrs[i]);
        print_uart("\n");
    }
}

// 测试load_uart函数（字符串读取）
void test_load_uart_string() {
    print_uart("=== Load String Test ===\n");
    print_uart("Please type strings (end with '#'):\n");

    char loaded_strings[3][64]; // 3个字符串，每个最多63字符

    for (int i = 0; i < 3; i++) {
        print_uart("Enter string ");
        print_uart_char(i + '0');
        print_uart(" (end with '#'): ");

        load_uart(loaded_strings[i], '#');

        print_uart("Loaded: \"");
        print_uart(loaded_strings[i]);
        print_uart("\"\n");
    }

    print_uart("Summary - Loaded strings:\n");
    for (int i = 0; i < 3; i++) {
        print_uart("String ");
        print_uart_char(i + '0');
        print_uart(": \"");
        print_uart(loaded_strings[i]);
        print_uart("\"\n");
    }
}

// 测试load_uart_timeout函数
void test_load_uart_timeout() {
    print_uart("=== Load Timeout Test ===\n");
    print_uart("Type something within 30 seconds (end with '*'):\n");

    char timeout_buffer[100];
    uint32_t timeout = 50000000 * 30;    // 50MHz * 30 seconds

    load_uart_timeout(timeout_buffer, '*', sizeof(timeout_buffer), timeout);

    print_uart("Result: \"");
    print_uart(timeout_buffer);
    print_uart("\"\n");

    if (timeout_buffer[0] == '\0') {
        print_uart("Timeout occurred - no input received\n");
    } else {
        print_uart("Input received successfully\n");
    }
}

// 综合测试 - 回环测试
void test_uart_loopback() {
    print_uart("=== Loopback Test ===\n");
    print_uart("This test will print data and ask you to type it back\n");

    // 测试数据
    uint32_t test_int = 0x12345678;
    uint64_t test_addr = 0x1234567890ABCDEFULL;
    uint8_t test_byte = 0xAB;

    // 输出测试数据
    print_uart("Please type back the following data:\n");

    print_uart("1. Integer (8 hex digits): ");
    print_uart_hex_32b(test_int);
    print_uart("\n");

    print_uart("2. Address (16 hex digits): ");
    print_uart_hex_64b(test_addr);
    print_uart("\n");

    print_uart("3. Byte (2 hex digits): ");
    print_uart_byte(test_byte);
    print_uart("\n");

    // 读取用户输入
    uint32_t input_int;
    uint64_t input_addr = 0;
    uint8_t input_byte;

    print_uart("\nNow type them back:\n");

    print_uart("Enter integer:\n");
    load_uart_32b(&input_int);

    print_uart("Enter address:\n");
    load_uart_64b(&input_addr);

    print_uart("Enter byte:\n");
    load_uart_byte(&input_byte);

    // 验证结果
    print_uart("\n=== Verification ===\n");

    print_uart("Integer - Expected: 0x");
    print_uart_hex_32b(test_int);
    print_uart(", Got: 0x");
    print_uart_hex_32b(input_int);
    print_uart(input_int == test_int ? " ✓\n" : " ✗\n");

    print_uart("Address - Expected: 0x");
    print_uart_hex_64b(test_addr);
    print_uart(", Got: 0x");
    print_uart_hex_64b(input_addr);
    print_uart(input_addr == test_addr ? " ✓\n" : " ✗\n");

    print_uart("Byte - Expected: 0x");
    print_uart_byte(test_byte);
    print_uart(", Got: 0x");
    print_uart_byte(input_byte);
    print_uart(input_byte == test_byte ? " ✓\n" : " ✗\n");
}

// 测试printf_uart函数
void test_printf_uart() {
    printf_uart("=== printf_uart Function Test ===\n");

    // 测试基本字符串输出
    printf_uart("Basic string test: %s\n", "Hello from printf_uart!");

    // 测试整数输出
    printf_uart("=== Integer Tests ===\n");
    int32_t test_positive = 42;
    int32_t test_negative = -123;
    uint32_t test_unsigned = 0x12345678;

    printf_uart("Positive integer: %d\n", test_positive);
    printf_uart("Negative integer: %d\n", test_negative);
    printf_uart("Unsigned integer: %u\n", test_unsigned);
    printf_uart("Hex: %x\n", test_unsigned);

    // 测试字符输出
    printf_uart("=== Character Tests ===\n");
    char test_char = 'A';
    printf_uart("Single character: %c\n", test_char);
    printf_uart("Multiple chars: %c%c%c\n", 'X', 'Y', 'Z');

    // 测试字符串输出
    printf_uart("=== String Tests ===\n");
    const char* test_str1 = "Test String 1";
    const char* test_str2 = "Test String 2";
    const char* null_str = NULL;

    printf_uart("String 1: %s\n", test_str1);
    printf_uart("String 2: %s\n", test_str2);
    printf_uart("Null string: %s\n", null_str);
    printf_uart("Combined: %s and %s\n", test_str1, test_str2);

    // 测试指针地址输出
    printf_uart("=== Pointer Tests ===\n");
    uint32_t* ptr1 = (uint32_t*)0x80000000;
    uint64_t* ptr2 = (uint64_t*)0x1234567890ABCDEF;
    void* ptr3 = (void*)0x10000000;

    printf_uart("Pointer 1: %p\n", ptr1);
    printf_uart("Pointer 2: %p\n", ptr2);
    printf_uart("Void pointer: %p\n", ptr3);

    // 测试字节输出
    printf_uart("=== Byte Tests ===\n");
    uint8_t test_bytes[] = {0x00, 0xFF, 0xAA, 0x55, 0x12, 0x34};

    printf_uart("Byte 0: %b\n", test_bytes[0]);
    printf_uart("Byte 1: %b\n", test_bytes[1]);
    printf_uart("Multiple bytes: %b %b %b %b %b %b\n",
                test_bytes[0], test_bytes[1], test_bytes[2],
                test_bytes[3], test_bytes[4], test_bytes[5]);

    // 测试特殊字符
    printf_uart("=== Special Character Tests ===\n");
    printf_uart("Percent literal: 100%% complete\n");
    printf_uart("Unknown format: %z (should show as %%z)\n");

    // 测试混合格式
    printf_uart("=== Mixed Format Tests ===\n");
    printf_uart("Mixed 1: Int=%d, Str=%s, Char=%c\n",
                42, "Hello", 'X');
    printf_uart("Mixed 2: Addr=%p, Byte=%b, Hex=%x\n",
                (void*)0x12345678, (uint8_t)0xAB, 0xDEADBEEF);
    printf_uart("Mixed 3: %s: %d items at %p (status: %c)\n",
                "Result", 5, (void*)0x80000000, 'O');

    // 测试边界情况
    printf_uart("=== Boundary Tests ===\n");
    printf_uart("Max uint32: %u\n", 0xFFFFFFFF);
    printf_uart("Min int32: %d\n", (int32_t)0x80000000);
    printf_uart("Zero values: %d %u %x %b\n", 0, 0U, 0U, (uint8_t)0);

    // 测试数组输出
    printf_uart("=== Array Output Tests ===\n");
    uint32_t int_array[] = {0x11111111, 0x22222222, 0x33333333};
    printf_uart("Int array: ");
    for (int i = 0; i < 3; i++) {
        printf_uart("%x ", int_array[i]);
    }
    printf_uart("\n");

    uint64_t addr_array[] = {0x8000000000000000, 0x4000000000000000, 0x2000000000000000};
    printf_uart("Addr array: ");
    for (int i = 0; i < 3; i++) {
        printf_uart("%p ", (void*)addr_array[i]);
    }
    printf_uart("\n");

    // 对比传统方式和printf_uart方式
    printf_uart("=== Comparison Test ===\n");
    printf_uart("Traditional way:\n");
    print_uart("Value: 0d");
    print_uart_dec_32b(0x12345678);
    print_uart(", Addr: 0x");
    print_uart_hex_64b(0x1234567890ABCDEF);
    print_uart(", Byte: 0x");
    print_uart_byte(0xAB);
    print_uart("\n");

    printf_uart("printf_uart way:\n");
    printf_uart("Value: 0d%u, Addr: 0x%p, Byte: 0x%b\n",
                0x12345678, (void*)0x1234567890ABCDEF, (uint8_t)0xAB);

    // 性能测试
    printf_uart("=== Performance Test ===\n");
    printf_uart("Sending 10 formatted lines...\n");
    for (int i = 0; i < 16; i++) {
        printf_uart("Line %d: Count=%d, Hex=%x, Status=%c\n",
                    i, i * 10, i * 0x1111, 'A' + (i % 26));
    }

    printf_uart("printf_uart test completed!\n");
}

int main() {
    // 初始化UART
    print_uart("Initializing UART...\n");
    init_uart(115000000, 115200);

    print_uart("\n");
    print_uart("========================================\n");
    print_uart("      UART Function Test Suite\n");
    print_uart("========================================\n");

    // 测试基本字符串输出
    print_uart("=== String Test ===\n");
    for (int i = 0; i < 6; i++) {
        print_uart(test_strings[i]);
    }

    // 测试整数输出
    test_uart_32b();

    // 测试地址输出
    test_uart_64b();

    // 测试字节输出
    test_uart_bytes();

    // 测试内存和UART结合
    test_memory_uart();

    // 压力测试
    test_uart_stress();

    print_uart("\n");
    print_uart("========================================\n");
    print_uart("      UART Load Function Tests\n");
    print_uart("========================================\n");

    // 输入测试（交互式）
    test_load_uart_char();
    test_load_uart_byte();
    test_load_uart_string();
    test_load_uart_32b();
    test_load_uart_64b();
    test_load_uart_timeout();
    test_uart_loopback();

    print_uart("\n");
    print_uart("========================================\n");
    print_uart("      In-House Printf Function Tests\n");
    print_uart("========================================\n");

    // in-house printf_uart function tests
    test_printf_uart();

    print_uart("\n");
    print_uart("========================================\n");
    print_uart("      All UART Tests Completed!\n");
    print_uart("========================================\n");
    print_uart("\n\n");

    return 0;
}
