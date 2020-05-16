/*
 * led_driver.h
 *
 *  Created on: Mar 16, 2020
 *      Author: joris
 */

#ifndef INC_LED_DRIVER_H_
#define INC_LED_DRIVER_H_

#include "main.h"

void init_led(SPI_HandleTypeDef* spi_handle, TIM_HandleTypeDef* tim_handle);
void led_task();
uint8_t getOutput_led(uint16_t lineselect);
#endif /* INC_LED_DRIVER_H_ */
