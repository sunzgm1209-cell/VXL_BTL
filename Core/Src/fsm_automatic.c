/*
 * fsm_automatic.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "fsm_automatic.h"

static void auto_update_lcd(void){
	lcd_set_cursor(0, 0);
	lcd_print_str("AUTO  T1:");
	lcd_print_num_2d(countdown1);
	lcd_print_str(" T2:");
	lcd_print_num_2d(countdown2);


	lcd_set_cursor(1, 0);
	lcd_print_str("T:");
	lcd_print_num_2d((int)env_temp);
	lcd_print_str("C  H:");
	lcd_print_num_2d((int)env_humi);
	lcd_print_str("%   ");
}

void fsm_automatic_run(void) {
    switch (status) {
        case INIT:
            status = AUTO_RED1_GREEN2;
            countdown1 = time_red;
            countdown2 = time_green;
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);
            setTimer(timer_traffic, 1000);
            auto_update_lcd();
            break;

        case AUTO_RED1_GREEN2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);

            if (timer_flags[timer_traffic] == 1) {
                setTimer(timer_traffic, 1000);
                countdown1--;
                countdown2--;
                if (countdown2 <= 0) {
                    status = AUTO_RED1_AMBER2;
                    countdown2 = time_amber;
                }
                auto_update_lcd();
            }

            if (is_button_pressed(0)) {
                status = MAN_INIT;
            }
            break;

        case AUTO_RED1_AMBER2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_AMBER);

            if (timer_flags[timer_traffic] == 1) {
                setTimer(timer_traffic, 1000);
                countdown1--;
                countdown2--;
                if (countdown2 <= 0) {
                    status = AUTO_GREEN1_RED2;
                    countdown1 = time_green;
                    countdown2 = time_red;
                }
                auto_update_lcd();
            }

            if (is_button_pressed(0)) {
                status = MAN_INIT;
            }
            break;

        case AUTO_GREEN1_RED2:
            traffic_set(1, LIGHT_GREEN);
            traffic_set(2, LIGHT_RED);

            if (timer_flags[timer_traffic] == 1) {
                setTimer(timer_traffic, 1000);
                countdown1--;
                countdown2--;
                if (countdown1 <= 0) {
                    status = AUTO_AMBER1_RED2;
                    countdown1 = time_amber;
                }
                auto_update_lcd();
            }

            if (is_button_pressed(0)) {
                status = MAN_INIT;
            }
            break;

        case AUTO_AMBER1_RED2:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_RED);

            if (timer_flags[timer_traffic] == 1) {
                setTimer(timer_traffic, 1000);
                countdown1--;
                countdown2--;
                if (countdown1 <= 0) {
                    status = AUTO_RED1_GREEN2;
                    countdown1 = time_red;
                    countdown2 = time_green;
                }
                auto_update_lcd();
            }

            if (is_button_pressed(0)) {
                status = MAN_INIT;
            }
            break;

        default:
            break;
    }
}
