/*
 * interrupt_pin.c
 *
 *  Created on: 2 Apr 2020
 *      Author: tom
 */


#include "interrupt_pin.h"
#include "i2c_handler.h"


void setInterruptPin(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0);
}

void resetInterruptPin(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1);
}

void addInterruptReason(uint8_t new_reason){
	uint8_t* reason = getI2CMemory(0);
	*reason |= new_reason;
}

void clearInterruptReason(){
	uint8_t* reason = getI2CMemory(0);
	*reason = 0;
}
