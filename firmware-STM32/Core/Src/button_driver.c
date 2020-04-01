/*
 * button_driver.c
 *
 *  Created on: Mar 16, 2020
 *      Author: joris
 */
#include "button_driver.h"
#include "i2c_handler.h"
#include "interrupt_pin.h"

uint32_t lastbutton_tick = 0;
uint32_t button_row = 0; //Row selector
uint32_t button_state = 0;
uint32_t button_state_prev = 0;
uint16_t *i2c_buttonstate;

void button_init() {
	i2c_buttonstate = (uint16_t *) getI2CMemory(4);
}

void button_task() {
	if((HAL_GetTick() - lastbutton_tick) > BUTTON_TICK) { //BUTTON task
		lastbutton_tick = HAL_GetTick();

		switch(button_row) {
		case 0:
			button_state = ((GPIOA->IDR >> 4) & 0x000F);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
			break;
		case 1:
			button_state |= ((GPIOA->IDR >> 4) & 0x000F) << 4;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
			break;
		case 2:
			button_state |= ((GPIOA->IDR >> 4) & 0x000F) << 8;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
			break;
		case 3:
			button_state |= ((GPIOA->IDR >> 4) & 0x000F) << 12;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
			if(button_state_prev != button_state) {	//Changes in button matrix
				*i2c_buttonstate = button_state;
				button_state_prev = button_state;

				addInterruptReason(INTERRUPT_REASON_BUTTON);
				setInterruptPin(); // Assert the interrupt pin to let the ESP32 know there is new data
			}
			break;
		}

		button_row++;
		if(button_row > 3) { button_row = 0; }
	}
}
