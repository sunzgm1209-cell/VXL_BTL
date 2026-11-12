/*
 * global.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "global.h"

SystemState_t status = INIT;

int time_red   = 10;
int time_amber = 2;
int time_green = 8;

int countdown1 = 0;
int countdown2 = 0;

int timer_traffic = -1;
int timer_blink   = -1;

float env_temp = 0.0f;
float env_humi = 0.0f;

void global_init(void) {
    status = INIT;
    time_red   = 10;
    time_amber = 2;
    time_green = 8;
    timer_traffic = requestTimer();
    timer_blink   = requestTimer();
}
