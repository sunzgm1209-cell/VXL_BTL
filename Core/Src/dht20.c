/*
 * dht20.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "dht20.h"
#include "global.h"

extern I2C_HandleTypeDef hi2c1;

uint16_t dht20_humidity_x10 = 0;
int16_t dht20_temperature_x10 = 0;
static uint8_t dht20_step = 0;

static void dht20_reset(uint8_t regist) {
    uint8_t reset[3], reply[3];

    reset[0] = regist;
    reset[1] = 0x00;
    reset[2] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, reset, 3, 50);
    HAL_Delay(10);

    HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, reply, 3, 50);
    reset[0] = 0xB0 | regist;
    reset[1] = reply[1];
    reset[2] = reply[2];
    HAL_Delay(10);

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, reset, 3, 50);
}

void dht20_init(void) {
    uint8_t init[3];

    init[0] = 0xA8;
    init[1] = 0x00;
    init[2] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, init, 3, 50);
    HAL_Delay(20);

    init[0] = 0xBE;
    init[1] = 0x08;
    init[2] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, init, 3, 50);
    HAL_Delay(20);

    uint8_t status_byte = 0;
    HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, &status_byte, 1, 50);
    if ((status_byte & 0x18) != 0x18) {
        dht20_reset(0x1B);
        dht20_reset(0x1C);
        dht20_reset(0x1E);
    }
    dht20_step = 0;
}

void dht20_task_run(void) {
    if (dht20_step == 0) {
        uint8_t trigger[3] = {0xAC, 0x33, 0x00};
        if (HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, trigger, 3, 50) == HAL_OK) {
            dht20_step = 1;
        }
    } else {
        uint8_t data[7] = {0};
        if (HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, data, 7, 50) == HAL_OK) {
            if ((data[0] & 0x80) == 0) {
                uint32_t Humid = ((uint32_t)data[1] << 12) |
                                 ((uint32_t)data[2] << 4)  |
                                 ((uint32_t)data[3] >> 4);
                dht20_humidity_x10 = (Humid * 1000) / 1048576;

                uint32_t Temper = (((uint32_t)data[3] & 0x0F) << 16) |
                                  ((uint32_t)data[4] << 8)           |
                                  ((uint32_t)data[5]);
                dht20_temperature_x10 = (int16_t)(((int64_t)Temper * 2000 / 1048576) - 500);
            }
        }
        dht20_step = 0;
    }
}

