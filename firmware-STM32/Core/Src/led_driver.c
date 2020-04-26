/*
 * led_driver.c
 *
 *  Created on: Mar 16, 2020
 *      Author: joris
 */
#include "stm32f1xx_hal.h"
#include "led_driver.h"
#include "i2c_handler.h"

uint8_t bitplane[128] = {6, 5, 4, 3, 2, 1, 0, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5,
		4, 6, 6, 5, 6, 6, 5, 4, 3, 2, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6
		, 5, 6, 6, 5, 4, 3, 2, 1, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5,
		 6, 6, 5, 4, 3, 2, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6};

uint8_t outputmap[8][7] = {0};

uint8_t logical_to_physical_sections[8] = {7, 1, 6, 0, 5, 2, 4, 3};

void init_led() {
	// Debug lines
//	uint8_t* dirty_byte = (uint8_t*) getI2CMemory(58);
//	uint8_t* first_led = (uint8_t*) getI2CMemory(10);
//	*first_led = 0xFF; // 1st led R
//	*(first_led+4) = 0xFF; // 2nd led G
//	*(first_led+8) = 0xFF; // 3rd led B
//	*dirty_byte = 1;
}

void led_task() {

}

uint8_t getOutput_led(uint16_t lineselect) {

	return 0;
}
