/*
 * lcd.h
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

#define SLAVE_ADDRESS_LCD   0x4E

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print_str(const char *str);
void lcd_print_num_2d(int num);
void lcd_print_num_x10(uint16_t val_x10);

#endif /* INC_LCD_H_ */   
