/*
 * traffic_light.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "traffic_light.h"

static const GPIO_PinState LIGHT_CODE[4][2] = {
    {GPIO_PIN_RESET, GPIO_PIN_RESET}, // 00: LIGHT_OFF
	{GPIO_PIN_SET,   GPIO_PIN_SET},   // 11: LIGHT_RED
	{GPIO_PIN_RESET, GPIO_PIN_SET},   // 01: LIGHT_AMBER
	{GPIO_PIN_SET,   GPIO_PIN_RESET}  // 11: LIGHT_GREEN
};

void traffic_set(uint8_t road, LightState_t state) {
    if (state > LIGHT_GREEN) return;

    GPIO_PinState pinA = LIGHT_CODE[state][0];
    GPIO_PinState pinB = LIGHT_CODE[state][1];

    if (road == 1) {
        HAL_GPIO_WritePin(TL1_A_GPIO_Port, TL1_A_Pin, pinA);
        HAL_GPIO_WritePin(TL1_B_GPIO_Port, TL1_B_Pin, pinB);
    } else if (road == 2) {
        HAL_GPIO_WritePin(TL2_A_GPIO_Port, TL2_A_Pin, pinA);
        HAL_GPIO_WritePin(TL2_B_GPIO_Port, TL2_B_Pin, pinB);
    }
}

