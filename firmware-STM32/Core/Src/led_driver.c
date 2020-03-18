/*
 * led_driver.c
 *
 *  Created on: Mar 16, 2020
 *      Author: joris
 */
#include "led_driver.h"

uint8_t bitplane[128] = {6, 5, 4, 3, 2, 1, 0, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5,
		4, 6, 6, 5, 6, 6, 5, 4, 3, 2, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6
		, 5, 6, 6, 5, 4, 3, 2, 1, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5,
		 6, 6, 5, 4, 3, 2, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6};

uint8_t outputmap[8][7];

void init_led() {
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 7; j++) {
			outputmap[i][j] = i + (i<<3);
		}
	}
}

void handle_led() {

}

uint8_t getOutput_led(uint16_t lineselect) {
	static uint8_t pos;
	uint8_t id = 31-__builtin_clz(lineselect >> 8);
	uint8_t return_val = outputmap[id][bitplane[pos]];
	if(lineselect == 0x8000) pos++;
	if(pos == 128) pos = 0;
	return return_val;
}
