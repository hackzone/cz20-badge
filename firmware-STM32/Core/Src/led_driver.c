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
	uint8_t* dirty_byte = (uint8_t*) getI2CMemory(58);
	if(*dirty_byte) {
		// 4 rows, divided in 2 parts each (8 total)
		for(uint8_t section = 0; section < 8; section++) {
			// Sections are not wired from top left to bottom right
			uint8_t physical_section = logical_to_physical_sections[section];

			uint8_t* first_led = (uint8_t*) getI2CMemory(10) + section * 6;
			uint8_t* second_led = first_led + 3;

			// Right-shift all values by one, to transform them to 7 bitplanes instead of 8
			uint8_t first_R = (*first_led) >> 1;
			uint8_t first_G = (*(first_led+1)) >> 1;
			uint8_t first_B = (*(first_led+2)) >> 1;

			uint8_t second_R = (*second_led) >> 1;
			uint8_t second_G = (*(second_led+1)) >> 1;
			uint8_t second_B = (*(second_led+2)) >> 1;

			for(uint8_t bitplane = 0; bitplane < 7; bitplane++) {
				outputmap[physical_section][bitplane] =
						((first_R & (1 << bitplane)) >> bitplane) +
						(((first_G & (1 << bitplane)) >> bitplane) << 1) +
						(((first_B & (1 << bitplane)) >> bitplane) << 2) +
						(((second_B & (1 << bitplane)) >> bitplane) << 3) +
						(((second_G & (1 << bitplane)) >> bitplane) << 4) +
						(((second_R & (1 << bitplane)) >> bitplane) << 5);
			}
		}

		*dirty_byte = 0;
	}
}

uint8_t getOutput_led(uint16_t lineselect) {
	static uint8_t pos;
	uint8_t id = 31-__builtin_clz(lineselect >> 8);
	uint8_t return_val = outputmap[id][bitplane[pos]];
	if(lineselect == 0x8000) pos++; // If the current row is the last (at gpio PB15), get next bitplane
	if(pos == 128) pos = 0;
	return return_val;
}
