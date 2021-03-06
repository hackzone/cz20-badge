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

uint8_t led_order[] = { 21,22,20, 1,2,0,    14,13,15, 26,25,27,
						32,33,23, 4,5,3,    11,10,12, 47,46,24,
						35,36,34, 7,16,6,   8,31,9,   44,43,45,
						38,39,37, 18,19,17, 29,28,30, 41,40,42};

// 3 * 16 bits, on 7 different bitplanes
uint8_t outputmap[7][6] = {0x00};

uint8_t logical_to_physical_sections[8] = {7, 1, 6, 0, 5, 2, 4, 3};

void init_led(SPI_HandleTypeDef* spi_handle, TIM_HandleTypeDef* tim_handle) {
	spi = spi_handle;

//	HAL_TIM_Base_Start_IT(tim_handle);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // ~OE -> enable output
  HAL_SPI_TxCpltCallback(spi);
}

void update_outputmap() {
	uint8_t* first_led = (uint8_t*) getI2CMemory(10);
	for(int bitplane = 1; bitplane < 8; bitplane++) {
		uint32_t *alias_region = (uint32_t *) (((uint32_t) outputmap[bitplane-1]-0x20000000)*32+0x22000000);
		for(int led_index = 0; led_index < 48; led_index++) {
			alias_region[led_order[led_index]] = first_led[led_index] >> bitplane;
		}
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // LE -> Latch
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0); // LE -> Stop latch

  uint8_t bitplane = bitplanes[bitplane_index];

  // Write bitplane to shift registers
  volatile HAL_StatusTypeDef result = HAL_SPI_Transmit_IT(spi, (uint8_t*)outputmap[bitplane], 6);

  bitplane_index++;
  if(bitplane_index >= 128) { bitplane_index = 0; }
}

void led_task() {
}
