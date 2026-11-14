/*
 * dht20.h
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */

#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include "main.h"

#define SLAVE_ADDRESS_DHT20 (0x38 << 1)

extern uint16_t dht20_humidity_x10;
extern int16_t  dht20_temperature_x10;

void dht20_init(void);   
void dht20_task_run(void);

#endif /* INC_DHT20_H_ */
