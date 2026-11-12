/*
 * traffic_light.h
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"

typedef enum {
    LIGHT_OFF = 0,
    LIGHT_RED,
    LIGHT_AMBER,
    LIGHT_GREEN
} LightState_t;

void traffic_set(uint8_t road, LightState_t state);

#endif /* INC_TRAFFIC_LIGHT_H_ */
