// Copyright OpenHW Group contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
// Modifier: Mingxuan Li <mingxuanli_siris@163.com> [Peking University]

#include "uart.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void write_reg_u8(uintptr_t addr, uint8_t value)
{
    volatile uint8_t *loc_addr = (volatile uint8_t *)addr;
    *loc_addr = value;
}

uint8_t read_reg_u8(uintptr_t addr)
{
    return *(volatile uint8_t *)addr;
}

int is_transmit_empty()
{
    return read_reg_u8(UART_LINE_STATUS) & 0x20;
}

char is_transmit_empty_altera()
{
    return ((read_reg_u8(UART_THR+7) << 8 ) + read_reg_u8(UART_THR+6));
}

int is_receive_empty()
{
    #ifndef PLAT_AGILEX
        return !(read_reg_u8(UART_LINE_STATUS) & 0x1);
    #else
        return (read_reg_u8(UART_THR) == 0);
    #endif
}

void print_uart_char(char a)
{
    #ifndef PLAT_AGILEX
        while (is_transmit_empty() == 0) {};
    #else
        while (is_transmit_empty_altera() < 8) {};
    #endif
    write_reg_u8(UART_THR, a);
}

int load_uart_char(uint8_t *res)
{
    if(is_receive_empty()) {
        return 0;
    }

    *res = read_reg_u8(UART_RBR);
    return 1;
}

uint8_t bin_to_hex_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void bin_to_hex(uint8_t inp, uint8_t res[2])
{
    res[1] = bin_to_hex_table[inp & 0xf];
    res[0] = bin_to_hex_table[(inp >> 4) & 0xf];
    return;
}

uint8_t hex_to_bin(char hex_char) {
    if (hex_char >= '0' && hex_char <= '9')
        return hex_char - '0';
    else if (hex_char >= 'A' && hex_char <= 'F')
        return hex_char - 'A' + 10;
    else if (hex_char >= 'a' && hex_char <= 'f')
        return hex_char - 'a' + 10;
    return 0;
}

void init_uart(uint32_t freq, uint32_t baud)
{
    uint32_t divisor = freq / (baud << 4);

    write_reg_u8(UART_INTERRUPT_ENABLE, 0x00); // Disable all interrupts
    write_reg_u8(UART_LINE_CONTROL, 0x80);     // Enable DLAB (set baud rate divisor)
    write_reg_u8(UART_DLAB_LSB, divisor);         // divisor (lo byte)
    write_reg_u8(UART_DLAB_MSB, (divisor >> 8) & 0xFF);  // divisor (hi byte)
    write_reg_u8(UART_LINE_CONTROL, 0x03);     // 8 bits, no parity, one stop bit
    write_reg_u8(UART_FIFO_CONTROL, 0xC7);     // Enable FIFO, clear them, with 14-byte threshold
    write_reg_u8(UART_MODEM_CONTROL, 0x20);    // Autoflow mode
}

void print_uart(const char *str)
{
    const char *cur = &str[0];
    while (*cur != '\0')
    {
        print_uart_char((uint8_t)*cur);
        ++cur;
    }
}

void print_uart_hex_32b(uint32_t data)
{
    for (int i = 3; i > -1; i--)
    {
        uint8_t cur = (data >> (i * 8)) & 0xff;
        uint8_t hex[2];
        bin_to_hex(cur, hex);
        print_uart_char(hex[0]);
        print_uart_char(hex[1]);
    }
}

void print_uart_dec_32b(uint32_t data)
{
    if (data == 0) {
        print_uart_char('0');
        return;
    }

    // 计算位数
    uint32_t temp = data;
    int digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    // 从最高位开始打印
    for (int i = digits - 1; i >= 0; i--) {
        uint32_t divisor = 1;
        for (int j = 0; j < i; j++) {
            divisor *= 10;
        }
        uint8_t digit = (data / divisor) % 10;
        print_uart_char('0' + digit);
    }
}

void print_uart_bin_32b(uint32_t data)
{
    for (int i = 31; i >= 0; i--) {
        uint8_t bit = (data >> i) & 1;
        print_uart_char('0' + bit);

        // 每4位添加一个下划线，方便阅读
        if (i > 0 && i % 4 == 0) {
            print_uart_char('_');
        }
    }
}

void print_uart_hex_64b(uint64_t data)
{
    for (int i = 7; i > -1; i--)
    {
        uint8_t cur = (data >> (i * 8)) & 0xff;
        uint8_t hex[2];
        bin_to_hex(cur, hex);
        print_uart_char(hex[0]);
        print_uart_char(hex[1]);
    }
}

void print_uart_dec_64b(uint64_t data)
{
    if (data == 0) {
        print_uart_char('0');
        return;
    }

    // 计算位数
    uint64_t temp = data;
    int digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    // 从最高位开始打印
    for (int i = digits - 1; i >= 0; i--) {
        uint64_t divisor = 1;
        for (int j = 0; j < i; j++) {
            divisor *= 10;
        }
        uint8_t digit = (data / divisor) % 10;
        print_uart_char('0' + digit);
    }
}

void print_uart_bin_64b(uint64_t data)
{
    for (int i = 63; i >= 0; i--) {
        uint8_t bit = (data >> i) & 1;
        print_uart_char('0' + bit);

        // 每4位添加一个下划线，方便阅读
        if (i > 0 && i % 4 == 0) {
            print_uart_char('_');
        }
    }
}

void print_uart_byte(uint8_t byte)
{
    uint8_t hex[2];
    bin_to_hex(byte, hex);
    print_uart_char(hex[0]);
    print_uart_char(hex[1]);
}

void load_uart(char *str, char terminator)
{
    uint8_t c;
    int i = 0;
    while (1)
    {
        if (load_uart_char(&c))
        {
            if (c == terminator)
            {
                str[i] = '\0';
                break;
            }
            str[i++] = c;
        }
    }
}

void load_uart_32b(uint32_t *data)
{
    *data = 0;
    for (int i = 3; i > -1; i--)
    {
        uint8_t byte;
        uint8_t hex[2];
        while (!load_uart_char(&hex[0])){}
        while (!load_uart_char(&hex[1])){}
        if (hex[0] == '\n' || hex[1] == '\n')
            byte = 0;
        else
            byte = (hex_to_bin(hex[0]) << 4) | hex_to_bin(hex[1]);
        *data |= ((uint32_t)byte << (i * 8));
    }
}

void load_uart_64b(uint64_t *data)
{
    *data = 0;
    for (int i = 7; i > -1; i--)
    {
        uint8_t byte;
        uint8_t hex[2];
        while (!load_uart_char(&hex[0])){}
        while (!load_uart_char(&hex[1])){}
        if (hex[0] == '\n' || hex[1] == '\n')
            byte = 0;
        else
            byte = (hex_to_bin(hex[0]) << 4) | hex_to_bin(hex[1]);
        *data |= ((uint64_t)byte << (i * 8));
    }
}

void load_uart_byte(uint8_t *byte)
{
    uint8_t hex[2];
    while (!load_uart_char(&hex[0])){}
    while (!load_uart_char(&hex[1])){}
    if (hex[0] == '\n' || hex[1] == '\n')
        *byte = 0;
    else
        *byte = (hex_to_bin(hex[0]) << 4) | hex_to_bin(hex[1]);
}

void load_uart_timeout(char *str, char terminator, int max_len, uint32_t timeout)
{
    uint8_t c;
    int i = 0;
    int count = 0;
    uint32_t timer = 0;
    while (count < max_len && timer < timeout)
    {
        if (load_uart_char(&c))
        {
            if (c == terminator)
            {
                str[i] = '\0';
                break;
            }
            str[i++] = c;
            timer = 0;
            count++;
        }
        else
            timer++;
        if (count == max_len)
            print_uart("ERROR! Maximum length reached, terminating input.\n");
        if (timer == timeout)
            print_uart("ERROR! Input timed out, terminating input.\n");
    }
}

void printf_uart(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    const char* p = format;

    while (*p != '\0') {
        if (*p == '%' && *(p + 1) != '\0') {
            p++; // 跳过 '%'
            switch (*p) {
                case 'd':
                case 'i': {
                    // 有符号整数
                    int32_t val = va_arg(args, int32_t);
                    if (val < 0) {
                        print_uart_char('-');
                        val = -val;
                    }
                    print_uart_dec_32b((uint32_t)val);
                    break;
                }
                case 'u': {
                    // 无符号整数
                    uint32_t val = va_arg(args, uint32_t);
                    print_uart_dec_32b(val);
                    break;
                }
                case 'x': {
                    // 十六进制整数
                    uint32_t val = va_arg(args, uint32_t);
                    print_uart_hex_32b(val);
                    break;
                }
                case 'p': {
                    // 指针地址
                    void* ptr = va_arg(args, void*);
                    uint64_t val = (uint64_t)(uintptr_t)ptr;
                    print_uart("0x");
                    print_uart_hex_64b(val);
                    break;
                }
                case 'c': {
                    // 字符 - 注意：char会被提升为int
                    int val = va_arg(args, int);
                    print_uart_char((char)val);
                    break;
                }
                case 's': {
                    // 字符串
                    const char* val = va_arg(args, const char*);
                    if (val != NULL) {
                        print_uart(val);
                    } else {
                        print_uart("(null)");
                    }
                    break;
                }
                case 'b': {
                    // 字节（自定义格式） - 注意：uint8_t会被提升为int
                    int val = va_arg(args, int);
                    print_uart_byte((uint8_t)val);
                    break;
                }
                case '%': {
                    // 字面量 '%'
                    print_uart_char('%');
                    break;
                }
                default: {
                    // 未知格式，直接输出
                    print_uart_char('%');
                    print_uart_char(*p);
                    break;
                }
            }
        } else {
            print_uart_char(*p);
        }
        p++;
    }

    va_end(args);
}
