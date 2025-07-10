// Copyright OpenHW Group contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
// Modifier: Mingxuan Li <mingxuanli_siris@163.com> [Peking University]

#include "uart.h"

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

void print_uart_int(uint32_t data)
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

void print_uart_addr(uint64_t addr)
{
    for (int i = 7; i > -1; i--)
    {
        uint8_t cur = (addr >> (i * 8)) & 0xff;
        uint8_t hex[2];
        bin_to_hex(cur, hex);
        print_uart_char(hex[0]);
        print_uart_char(hex[1]);
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

void load_uart_int(uint32_t *data)
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

void load_uart_addr(uint64_t *addr)
{
    *addr = 0;
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
        *addr |= ((uint64_t)byte << (i * 8));
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
