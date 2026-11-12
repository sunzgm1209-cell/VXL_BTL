/*
 * fsm_manual.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "fsm_manual.h"

static void man_update_lcd(const char* state_str) {
    lcd_set_cursor(0, 0);
    lcd_print_str("MODE: MANUAL    ");
    lcd_set_cursor(1, 0);
    lcd_print_str(state_str);
}

void fsm_manual_run(void) {
    switch (status) {
        case MAN_INIT:
            status = MAN_RED1_GREEN2;
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);
            man_update_lcd("R1:RED  R2:GRN  ");
            break;

        case MAN_RED1_GREEN2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);

            if (is_button_pressed(1)) {
                status = MAN_RED1_AMBER2;
                man_update_lcd("R1:RED  R2:AMB  ");
            }
            if (is_button_pressed(0)) {
                status = TUN_INIT;
            }
            break;

        case MAN_RED1_AMBER2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_AMBER);

            if (is_button_pressed(1)) {
                status = MAN_GREEN1_RED2;
                man_update_lcd("R1:GRN  R2:RED  ");
            }
            if (is_button_pressed(0)) {
                status = TUN_INIT;
            }
            break;

        case MAN_GREEN1_RED2:
            traffic_set(1, LIGHT_GREEN);
            traffic_set(2, LIGHT_RED);

            if (is_button_pressed(1)) {
                status = MAN_AMBER1_RED2;
                man_update_lcd("R1:AMB  R2:RED  ");
            }
            if (is_button_pressed(0)) {
                status = TUN_INIT;
            }
            break;

        case MAN_AMBER1_RED2:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_RED);

            if (is_button_pressed(1)) {
                status = MAN_RED1_GREEN2;
                man_update_lcd("R1:RED  R2:GRN  ");
            }
            if (is_button_pressed(0)) {
                status = TUN_INIT;
            }
            break;

        default:
            break;
    }
}
