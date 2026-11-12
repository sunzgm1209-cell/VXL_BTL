/*
 * dht20.c
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */
#include "dht20.h"
#include "global.h"

extern I2C_HandleTypeDef hi2c1;

static uint8_t dht20_state = 0;

static void dht20_reset_reg(uint8_t reg) {
    uint8_t cmd[3] = {reg, 0x00, 0x00};
    uint8_t buf[3] = {0};

    HAL_I2C_Master_Transmit(&hi2c1, DHT20_I2C_ADDR, cmd, 3, 50);
    HAL_Delay(5);
    HAL_I2C_Master_Receive(&hi2c1, DHT20_I2C_ADDR, buf, 3, 50);
    HAL_Delay(10);

    uint8_t write_back[3] = {0xB0 | reg, buf[1], buf[2]};
    HAL_I2C_Master_Transmit(&hi2c1, DHT20_I2C_ADDR, write_back, 3, 50);
}

void dht20_init(void) {
    HAL_Delay(100);
    uint8_t status = 0;
    uint8_t cmd_status = 0x71;

    HAL_I2C_Master_Transmit(&hi2c1, DHT20_I2C_ADDR, &cmd_status, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, DHT20_I2C_ADDR, &status, 1, 50);

    if ((status & 0x18) != 0x18) {
        dht20_reset_reg(0x1B);
        dht20_reset_reg(0x1C);
        dht20_reset_reg(0x1E);
    }
    dht20_state = 0;
}

void dht20_task_run(void) {
    if (dht20_state == 0) {
        uint8_t trigger_cmd[3] = {0xAC, 0x33, 0x00};
        if (HAL_I2C_Master_Transmit(&hi2c1, DHT20_I2C_ADDR, trigger_cmd, 3, 50) == HAL_OK) {
            dht20_state = 1;
        }
    } else {
        uint8_t rx_buf[7] = {0};
        if (HAL_I2C_Master_Receive(&hi2c1, DHT20_I2C_ADDR, rx_buf, 7, 50) == HAL_OK) {
            if ((rx_buf[0] & 0x80) == 0) {
                uint32_t raw_humi = ((uint32_t)rx_buf[1] << 12) |
                                    ((uint32_t)rx_buf[2] << 4)  |
                                    ((uint32_t)rx_buf[3] >> 4);

                uint32_t raw_temp = (((uint32_t)rx_buf[3] & 0x0F) << 16) |
                                    ((uint32_t)rx_buf[4] << 8)           |
                                    ((uint32_t)rx_buf[5]);

                env_humi = ((float)raw_humi / 1048576.0f) * 100.0f;
                env_temp = ((float)raw_temp / 1048576.0f) * 200.0f - 50.0f;
            }
        }
        dht20_state = 0;
    }
}

