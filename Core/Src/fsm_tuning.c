/*
 * fsm_tuning.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "fsm_tuning.h"

static int temp_red   = 10;
static int temp_amber = 2;
static int temp_green = 8;
static uint8_t blink_state = 0;

static void tun_update_lcd(const char* color_name, int val) {
    lcd_set_cursor(0, 0);
    lcd_print_str("TUNING: ");
    lcd_print_str(color_name);
    lcd_print_str("   ");

    lcd_set_cursor(1, 0);
    lcd_print_str("TIME: ");
    lcd_print_num_2d(val);
    lcd_print_str("s [B3:OK]");
}

static void tun_blink_light(LightState_t color) {
    if (timer_flags[timer_blink] == 1) {
        setTimer(timer_blink, 500);
        blink_state = !blink_state;
        if (blink_state) {
            traffic_set(1, color);
            traffic_set(2, color);
        } else {
            traffic_set(1, LIGHT_OFF);
            traffic_set(2, LIGHT_OFF);
        }
    }
}

void fsm_tuning_run(void) {
    switch (status) {
        case TUN_INIT:
            temp_red   = time_red;
            temp_amber = time_amber;
            temp_green = time_green;
            blink_state = 1;
            status = TUN_RED;
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_RED);
            setTimer(timer_blink, 500);
            tun_update_lcd("RED  ", temp_red);
            break;

        case TUN_RED:
            tun_blink_light(LIGHT_RED);

            if (is_button_pressed(2)) {
                temp_red++;
                if (temp_red > 99) temp_red = 2;
                tun_update_lcd("RED  ", temp_red);
            }
            if (is_button_pressed_1s(2)) {
                temp_red += 5;
                if (temp_red > 99) temp_red = 2;
                tun_update_lcd("RED  ", temp_red);
            }

            if (is_button_pressed(3)) {
                if (temp_red < 2) temp_red = 2;
                time_red = temp_red;
                if (time_red > time_amber) {
                    time_green = time_red - time_amber;
                } else {
                    time_amber = 1;
                    time_green = time_red - 1;
                }
                lcd_set_cursor(1, 0);
                lcd_print_str("SAVED RED!      ");
            }

            if (is_button_pressed(1)) {
                status = TUN_AMBER;
                temp_amber = time_amber;
                tun_update_lcd("AMBER", temp_amber);
            }

            if (is_button_pressed(0)) {
                status = INIT;
            }
            break;

        case TUN_AMBER:
            tun_blink_light(LIGHT_AMBER);

            if (is_button_pressed(2)) {
                temp_amber++;
                if (temp_amber > 98) temp_amber = 1;
                tun_update_lcd("AMBER", temp_amber);
            }
            if (is_button_pressed_1s(2)) {
                temp_amber += 5;
                if (temp_amber > 98) temp_amber = 1;
                tun_update_lcd("AMBER", temp_amber);
            }

            if (is_button_pressed(3)) {
                time_amber = temp_amber;
                if (time_amber + time_green > 99) {
                    time_green = 99 - time_amber;
                }
                time_red = time_amber + time_green;
                lcd_set_cursor(1, 0);
                lcd_print_str("SAVED AMBER!    ");
            }

            if (is_button_pressed(1)) {
                status = TUN_GREEN;
                temp_green = time_green;
                tun_update_lcd("GREEN", temp_green);
            }

            if (is_button_pressed(0)) {
                status = INIT;
            }
            break;

        case TUN_GREEN:
            tun_blink_light(LIGHT_GREEN);

            if (is_button_pressed(2)) {
                temp_green++;
                if (temp_green > 98) temp_green = 1;
                tun_update_lcd("GREEN", temp_green);
            }
            if (is_button_pressed_1s(2)) {
                temp_green += 5;
                if (temp_green > 98) temp_green = 1;
                tun_update_lcd("GREEN", temp_green);
            }

            if (is_button_pressed(3)) {
                time_green = temp_green;
                if (time_amber + time_green > 99) {
                    time_amber = 99 - time_green;
                }
                time_red = time_amber + time_green;
                lcd_set_cursor(1, 0);
                lcd_print_str("SAVED GREEN!    ");
            }

            if (is_button_pressed(1)) {
                status = TUN_RED;
                temp_red = time_red;
                tun_update_lcd("RED  ", temp_red);
            }

            if (is_button_pressed(0)) {
                status = INIT;
            }
            break;

        default:
            break;
    }
}
