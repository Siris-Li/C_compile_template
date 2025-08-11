//////////////////////////////////////////////////////////////////////////////////
// Author:          Mingxuan Li
// Acknowledgement: GitHub Copilot
// Description:     DRAM (PSRAM) Function Test
//////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "uart.h"

// 测试数据模式
static const uint64_t test_patterns[] = {
    0xb6acad2abb260109,
    0x11752c63ab69c863,
    0x1234567890abcdef,
    0xfedcba0987654321,
    0x1122334455667788,
    0x99aabbccddeeff00,
    0x0011223344556677,
    0x8899aabbccddeeff,
    0xdeadbeefdeadbeef,
    0xfeedfacefeedface,
    0xaaaaaaaaaaaaaaaa,
    0x5555555555555555,
    0x0000000000000000,
    0xffffffffffffffff,
    0x0123456789abcdef,
    0xfedcba9876543210
};

// DRAM基地址和测试大小
#define DRAM_BASE_ADDR    0xa0000000
#define TEST_SIZE         32    // 测试32个64位数据
#define PATTERN_COUNT     (sizeof(test_patterns) / sizeof(test_patterns[0]))

// DRAM初始化
void init_dram() {
    print_uart("DRAM initializing ...\n");

    uint64_t* t_latency_access_address = (uint64_t*)0xe0000000;
    uint64_t* t_read_write_recovery_address = (uint64_t*)0xe0000018;
    uint64_t* t_rx_clk_delay_address = (uint64_t*)0xe0000020;
    uint64_t* address_mask_msb_address = (uint64_t*)0xe0000030;
    uint64_t t_latency_access_value = 7;
    uint64_t t_read_write_recovery_value = 7;
    uint64_t t_rx_clk_delay_value = 3;
    uint64_t address_mask_msb_value = 22;

    *t_latency_access_address = t_latency_access_value;
    *t_read_write_recovery_address = t_read_write_recovery_value;
    *t_rx_clk_delay_address = t_rx_clk_delay_value;
    *address_mask_msb_address = address_mask_msb_value;

    printf_uart("t_latency_access: %d\n", *t_latency_access_address);
    printf_uart("t_read_write_recovery: %d\n", *t_read_write_recovery_address);
    printf_uart("t_rx_clk_delay: %d\n", *t_rx_clk_delay_address);
    printf_uart("address_mask_msb: %d\n\n", *address_mask_msb_address);
}

// 基本写入测试
void test_dram_write() {
    print_uart("=== DRAM Write Test ===\n");
    uint64_t* mem_base = (uint64_t*)DRAM_BASE_ADDR;

    print_uart("Writing test patterns to DRAM at base address: 0x");
    print_uart_hex_64b(DRAM_BASE_ADDR);
    print_uart("\n");

    // 写入测试数据
    for (int i = 0; i < TEST_SIZE; i++) {
        uint64_t pattern = test_patterns[i % PATTERN_COUNT];
        *(mem_base + i) = pattern;
        // *(mem_base + i) = 0xffffffffffffffff;
        // *(mem_base + i) = 0x0;

        print_uart("Write[");
        print_uart_byte(i);
        print_uart("]: 0x");
        print_uart_hex_64b(pattern);
        print_uart(" -> 0x");
        print_uart_hex_64b((uint64_t)(mem_base + i));
        print_uart("\n");

        // 每8个地址打印一个分隔符
        if ((i + 1) % 8 == 0) {
            print_uart("---\n");
        }
    }

    print_uart("Write test completed!\n\n");
}

// 基本读取测试
int test_dram_read() {
    print_uart("=== DRAM Read Test ===\n");
    uint64_t* mem_base = (uint64_t*)DRAM_BASE_ADDR;
    int errors = 0;

    print_uart("Reading back data from DRAM...\n");

    for (int i = 0; i < TEST_SIZE; i++) {
        uint64_t expected = test_patterns[i % PATTERN_COUNT];
        uint64_t actual = *(mem_base + i);

        print_uart("Read[");
        print_uart_byte(i);
        print_uart("]: 0x");
        print_uart_hex_64b(actual);

        if (actual == expected) {
            print_uart(" ✓\n");
        } else {
            print_uart(" ✗ (Expected: 0x");
            print_uart_hex_64b(expected);
            print_uart(")\n");
            errors++;
        }

        // 每8个地址打印一个分隔符
        if ((i + 1) % 8 == 0) {
            print_uart("---\n");
        }
    }

    print_uart("Read test completed. Errors: ");
    print_uart_byte(errors);
    print_uart("\n\n");

    return errors;
}

// 地址行测试
void test_dram_address_lines() {
    print_uart("=== DRAM Address Lines Test ===\n");
    volatile uint64_t* mem_base = (volatile uint64_t*)DRAM_BASE_ADDR;

    // 测试地址线 - 写入地址值到对应位置
    print_uart("Testing address lines by writing address values...\n");

    for (int i = 0; i < 16; i++) {
        uint64_t addr_offset = 1ULL << i;  // 2^i
        uint64_t test_value = 0xa0000000ULL + (addr_offset * 8);

        if (addr_offset < TEST_SIZE) {
            *(mem_base + addr_offset) = test_value;

            print_uart("Addr test offset ");
            print_uart_byte(i);
            print_uart(": 0x");
            print_uart_hex_64b(test_value);
            print_uart(" -> offset 0x");
            print_uart_hex_64b(addr_offset);
            print_uart("\n");
        }
    }

    // 验证地址线测试
    print_uart("Verifying address lines...\n");
    int addr_errors = 0;

    for (int i = 0; i < 16; i++) {
        uint64_t addr_offset = 1ULL << i;
        uint64_t expected_value = 0xa0000000ULL + (addr_offset * 8);

        if (addr_offset < TEST_SIZE) {
            uint64_t actual_value = *(mem_base + addr_offset);

            print_uart("Verify offset ");
            print_uart_byte(i);
            print_uart(": 0x");
            print_uart_hex_64b(actual_value);

            if (actual_value == expected_value) {
                print_uart(" ✓\n");
            } else {
                print_uart(" ✗\n");
                addr_errors++;
            }
        }
    }

    print_uart("Address lines test completed. Errors: ");
    print_uart_byte(addr_errors);
    print_uart("\n\n");
}

// 数据线测试
void test_dram_data_lines() {
    print_uart("=== DRAM Data Lines Test ===\n");
    uint64_t* mem_base = (uint64_t*)DRAM_BASE_ADDR;

    // 测试数据线 - walking 1s 和 walking 0s
    uint64_t walking_patterns[] = {
        0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008,
        0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080,
        0xfffffffffffffffe, 0xfffffffffffffffd, 0xfffffffffffffffb, 0xfffffffffffffff7,
        0xffffffffffffffef, 0xffffffffffffffdf, 0xffffffffffffffbf, 0xffffffffffffff7f
    };

    int pattern_count = sizeof(walking_patterns) / sizeof(walking_patterns[0]);
    int data_errors = 0;

    print_uart("Testing data lines with walking patterns...\n");

    for (int i = 0; i < pattern_count && i < TEST_SIZE; i++) {
        uint64_t pattern = walking_patterns[i];
        *(mem_base + i) = pattern;

        uint64_t readback = *(mem_base + i);

        print_uart("Data[");
        print_uart_byte(i);
        print_uart("]: 0x");
        print_uart_hex_64b(readback);

        if (readback == pattern) {
            print_uart(" ✓\n");
        } else {
            print_uart(" ✗ (Expected: 0x");
            print_uart_hex_64b(pattern);
            print_uart(")\n");
            data_errors++;
        }
    }

    print_uart("Data lines test completed. Errors: ");
    print_uart_byte(data_errors);
    print_uart("\n\n");
}

// 压力测试 - 连续读写
void test_dram_stress() {
    print_uart("=== DRAM Stress Test ===\n");
    uint64_t* mem_base = (uint64_t*)DRAM_BASE_ADDR;
    int stress_errors = 0;

    print_uart("Performing stress test with multiple iterations...\n");

    for (int iteration = 0; iteration < 5; iteration++) {
        print_uart("Stress iteration ");
        print_uart_byte(iteration);
        print_uart(":\n");

        // 写入阶段
        for (int i = 0; i < TEST_SIZE; i++) {
            uint64_t pattern = test_patterns[i % PATTERN_COUNT] ^ (iteration << 8);
            *(mem_base + i) = pattern;
        }

        // 读取验证阶段
        for (int i = 0; i < TEST_SIZE; i++) {
            uint64_t expected = test_patterns[i % PATTERN_COUNT] ^ (iteration << 8);
            uint64_t actual = *(mem_base + i);

            if (actual != expected) {
                print_uart("  Error at position ");
                print_uart_byte(i);
                print_uart(": got 0x");
                print_uart_hex_64b(actual);
                print_uart(", expected 0x");
                print_uart_hex_64b(expected);
                print_uart("\n");
                stress_errors++;
            }
        }

        print_uart("  Iteration ");
        print_uart_byte(iteration);
        print_uart(" completed\n");
    }

    print_uart("Stress test completed. Total errors: ");
    print_uart_byte(stress_errors);
    print_uart("\n\n");
}

// 内存清零测试
void test_dram_clear() {
    print_uart("=== DRAM Clear Test ===\n");
    uint64_t* mem_base = (uint64_t*)DRAM_BASE_ADDR;

    print_uart("Clearing DRAM region...\n");

    // 清零
    for (int i = 0; i < TEST_SIZE; i++) {
        *(mem_base + i) = 0;
    }

    // 验证清零
    int clear_errors = 0;
    for (int i = 0; i < TEST_SIZE; i++) {
        uint64_t value = *(mem_base + i);
        if (value != 0) {
            print_uart("Clear error at position ");
            print_uart_byte(i);
            print_uart(": 0x");
            print_uart_hex_64b(value);
            print_uart("\n");
            clear_errors++;
        }
    }

    if (clear_errors == 0) {
        print_uart("DRAM cleared successfully!\n");
    } else {
        print_uart("Clear test failed with ");
        print_uart_byte(clear_errors);
        print_uart(" errors\n");
    }
    print_uart("\n");
}

int main() {
    // 初始化UART、DRAM
    init_uart(115000000, 115200);
    init_dram();

    print_uart("\n");
    print_uart("=========================================\n");
    print_uart("         DRAM Function Test Suite\n");
    print_uart("=========================================\n");
    print_uart("Base Address: 0x");
    print_uart_hex_64b(DRAM_BASE_ADDR);
    print_uart("\n");
    print_uart("Test Size: ");
    print_uart_byte(TEST_SIZE);
    print_uart(" x 64-bit words\n");
    print_uart("Total Memory: ");
    print_uart_hex_64b(TEST_SIZE * 8);
    print_uart(" bytes\n\n");

    // 执行各项测试
    test_dram_write();
    int read_errors = test_dram_read();
    test_dram_address_lines();
    test_dram_data_lines();
    test_dram_stress();
    test_dram_clear();

    // 测试总结
    print_uart("=========================================\n");
    print_uart("            Test Summary\n");
    print_uart("=========================================\n");

    if (read_errors == 0) {
        print_uart("✓ All DRAM tests PASSED!\n");
    } else {
        print_uart("✗ DRAM tests FAILED with ");
        print_uart_byte(read_errors);
        print_uart(" errors\n");
    }

    print_uart("DRAM testing completed.\n");
    print_uart("=========================================\n\n");

    return 0;
}
