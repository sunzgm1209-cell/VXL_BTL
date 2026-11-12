/*
 * dht20.h
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */

#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include "main.h"

#define DHT20_I2C_ADDR  (0x38 << 1)

void dht20_init(void);
void dht20_task_run(void);

#endif /* INC_DHT20_H_ */
