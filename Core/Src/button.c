/*
 * input_reading.c
 *
 *  Created on: Oct 25, 2026
 *      Author: ASUS
 */


#include <button.h>

#define NO_OF_BUTTONS 4
#define DURATION_FOR_PRESS_1S 100
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET


GPIO_TypeDef* buttonPort[NO_OF_BUTTONS] = {BTN_1_GPIO_Port, BTN_2_GPIO_Port, BTN_3_GPIO_Port, BTN_4_GPIO_Port};
uint16_t buttonPin[NO_OF_BUTTONS] = {BTN_1_Pin, BTN_2_Pin, BTN_3_Pin, BTN_4_Pin};

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS];
static GPIO_PinState debounceBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceBuffer2[NO_OF_BUTTONS];

static uint8_t  button_flag[NO_OF_BUTTONS];
static uint8_t  button_flag_1s[NO_OF_BUTTONS];
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};

void button_reading(void) {
    for (uint8_t i = 0; i < NO_OF_BUTTONS; i++) {
        debounceBuffer2[i] = debounceBuffer1[i];
        debounceBuffer1[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);

        if (debounceBuffer1[i] == debounceBuffer2[i]) {
            if (buttonBuffer[i] != debounceBuffer1[i]) {
                buttonBuffer[i] = debounceBuffer1[i];
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    button_flag[i] = 1;
                    counterForButtonPress1s[i] = DURATION_FOR_PRESS_1S;
                }
            } else {
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    if (counterForButtonPress1s[i] > 0) {
                        counterForButtonPress1s[i]--;
                        if (counterForButtonPress1s[i] == 0) {
                            button_flag_1s[i] = 1;
                        }
                    }
                } else {
                    button_flag_1s[i] = 0;
                }
            }
        }
    }
}

uint8_t is_button_pressed(uint8_t index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (button_flag[index] == 1) {
        button_flag[index] = 0;
        return 1;
    }
    return 0;
}

uint8_t is_button_pressed_1s(uint8_t index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (button_flag_1s[index] == 1) {
        button_flag_1s[index] = 0;
        return 1;
    }
    return 0;
}
