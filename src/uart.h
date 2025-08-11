// Copyright OpenHW Group contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
// Modifier: Mingxuan Li <mingxuanli_siris@163.com> [Peking University]

#pragma once

#include <stdint.h>

#define UART_BASE 0x10000000

#define UART_RBR UART_BASE + 0
#define UART_THR UART_BASE + 0
#define UART_INTERRUPT_ENABLE UART_BASE + 4
#define UART_INTERRUPT_IDENT UART_BASE + 8
#define UART_FIFO_CONTROL UART_BASE + 8
#define UART_LINE_CONTROL UART_BASE + 12
#define UART_MODEM_CONTROL UART_BASE + 16
#define UART_LINE_STATUS UART_BASE + 20
#define UART_MODEM_STATUS UART_BASE + 24
#define UART_DLAB_LSB UART_BASE + 0
#define UART_DLAB_MSB UART_BASE + 4

void init_uart(uint32_t freq, uint32_t baud);

void print_uart(const char* str);

void print_uart_hex_32b(uint32_t data);

void print_uart_dec_32b(uint32_t data);

void print_uart_bin_32b(uint32_t data);

void print_uart_hex_64b(uint64_t data);

void print_uart_dec_64b(uint64_t data);

void print_uart_bin_64b(uint64_t data);

void print_uart_byte(uint8_t byte);

void print_uart_char(char c);

void load_uart(char *str, char terminator);

void load_uart_32b(uint32_t *data);

void load_uart_64b(uint64_t *data);

void load_uart_byte(uint8_t *byte);

int load_uart_char(uint8_t *res);

void load_uart_timeout(char *str, char terminator, int max_len, uint32_t timeout);

void printf_uart(const char* format, ...);

// 支持的格式说明符：
// - %d, %i : 有符号32位整数（十进制），负数会显示负号
// - %u     : 无符号32位整数（十进制格式，8位）
// - %x     : 无符号32位整数（十六进制格式，8位）
// - %p     : 指针地址（64位，格式为0x+16位十六进制）
// - %c     : 单个字符
// - %s     : 字符串（C风格，以\0结尾）
// - %b     : 单个字节（十六进制格式，2位）
// - %%     : 字面量百分号
