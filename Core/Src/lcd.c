/*
 * lcd.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "lcd.h"

extern I2C_HandleTypeDef hi2c1;

static void lcd_send_cmd(char cmd) {
    char data_u = (cmd & 0xF0);
    char data_l = ((cmd << 4) & 0xF0);
    uint8_t data_t[4];

    data_t[0] = data_u | 0x0C;  // en=1, rs=0, backlight=1
    data_t[1] = data_u | 0x08;  // en=0, rs=0, backlight=1
    data_t[2] = data_l | 0x0C;  // en=1, rs=0, backlight=1
    data_t[3] = data_l | 0x08;  // en=0, rs=0, backlight=1

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 50);
}

static void lcd_send_data(char data) {
    char data_u = (data & 0xF0);
    char data_l = ((data << 4) & 0xF0);
    uint8_t data_t[4];

    data_t[0] = data_u | 0x0D;  // en=1, rs=1, backlight=1
    data_t[1] = data_u | 0x09;  // en=0, rs=1, backlight=1
    data_t[2] = data_l | 0x0D;  // en=1, rs=1, backlight=1
    data_t[3] = data_l | 0x09;  // en=0, rs=1, backlight=1

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 50);
}

void lcd_init(void) {
    HAL_Delay(50);
    lcd_send_cmd(0x33);
    lcd_send_cmd(0x32);
    HAL_Delay(50);
    lcd_send_cmd(0x28);
    HAL_Delay(50);
    lcd_send_cmd(0x01);
    HAL_Delay(50);
    lcd_send_cmd(0x06);
    HAL_Delay(50);
    lcd_send_cmd(0x0C);
    HAL_Delay(50);
    lcd_send_cmd(0x02);
    HAL_Delay(50);
    lcd_send_cmd(0x80);
}

void lcd_clear(void) {
    lcd_set_cursor(0, 0);
    lcd_print_str("                ");
    lcd_set_cursor(1, 0);
    lcd_print_str("                ");
    lcd_set_cursor(0, 0);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    if (col > 15) col = 15;
    uint8_t pos_addr = (row == 0) ? (0x80 + col) : (0x80 | (0x40 + col));
    lcd_send_cmd(pos_addr);
}

void lcd_print_str(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void lcd_print_num_2d(int num) {
    if (num < 0) num = 0;
    if (num > 99) num = 99;
    lcd_send_data((num / 10) + '0');
    lcd_send_data((num % 10) + '0');
}

void lcd_print_num_x10(uint16_t val_x10) {
    if (val_x10 > 999) val_x10 = 999;
    lcd_send_data((val_x10 / 100) + '0');
    lcd_send_data(((val_x10 / 10) % 10) + '0');
    lcd_send_data('.');
    lcd_send_data((val_x10 % 10) + '0');
}

