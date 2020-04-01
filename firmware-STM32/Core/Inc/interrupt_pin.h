/*
 * interrupt_pin.h
 *
 *  Created on: 2 Apr 2020
 *      Author: tom
 */

#ifndef INC_INTERRUPT_PIN_H_
#define INC_INTERRUPT_PIN_H_

#include "stm32f1xx_hal.h"

#define INTERRUPT_REASON_GENERAL 1
#define INTERRUPT_REASON_BUTTON 2
#define INTERRUPT_REASON_MIDI 4

void setInterruptPin();
void resetInterruptPin();
void addInterruptReason(uint8_t reason);
void clearInterruptReason();

#endif /* INC_INTERRUPT_PIN_H_ */
