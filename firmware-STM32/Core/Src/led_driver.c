/*
 * led_driver.c
 *
 *  Created on: Mar 16, 2020
 *      Author: joris
 */
#include "stm32f1xx_hal.h"
#include "led_driver.h"
#include "i2c_handler.h"

SPI_HandleTypeDef* spi;
uint8_t bitplane_index = 0;
uint8_t bitplanes[128] = {6, 5, 4, 3, 2, 1, 0, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5,
		4, 6, 6, 5, 6, 6, 5, 4, 3, 2, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6
		, 5, 6, 6, 5, 4, 3, 2, 1, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5,
		 6, 6, 5, 4, 3, 2, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6, 6, 5, 4, 3, 6, 6, 5, 6, 6, 5, 4, 6, 6, 5, 6};

// 3 * 16 bits, on 7 different bitplanes
uint16_t outputmap[7][3] = {{0xFFFF,0xFFFF,0xFFFF},
		{0x0000,0x0000,0x0000},
		{0x0000,0x0000,0x0000},
		{0x0000,0x0000,0x0000},
		{0x0000,0x0000,0x0000},
		{0x0000,0x0000,0x0000},
		{0x0000,0x0000,0x0000}};

uint8_t logical_to_physical_sections[8] = {7, 1, 6, 0, 5, 2, 4, 3};

void init_led(SPI_HandleTypeDef* spi_handle, TIM_HandleTypeDef* tim_handle) {
	spi = spi_handle;
	//HAL_TIM_Base_Start_IT(tim_handle);
	// Debug lines
//	uint8_t* dirty_byte = (uint8_t*) getI2CMemory(58);
//	uint8_t* first_led = (uint8_t*) getI2CMemory(10);
//	*first_led = 0xFF; // 1st led R
//	*(first_led+4) = 0xFF; // 2nd led G
//	*(first_led+8) = 0xFF; // 3rd led B
//	*dirty_byte = 1;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // ~OE -> enable output
}

void led_task() {
	uint8_t bitplane = bitplanes[bitplane_index];

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // ~OE -> enable output


	// Write bitplane to shift registers, latch, and enable output again
	volatile HAL_StatusTypeDef result = HAL_SPI_Transmit(spi, (uint8_t*)outputmap[bitplane], 6, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // ~OE -> disable output
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // LE -> Latch
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1); // SPI_CLK rising
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // SPI_CLK falling
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // ~OE -> enable output
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // LE -> Latch
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0); // LE -> Stop latch


	bitplane_index++;
	if(bitplane_index >= 128) { bitplane_index = 0; }
}
