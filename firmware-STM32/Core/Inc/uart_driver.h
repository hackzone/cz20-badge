/*
 * uart_driver.h
 *
 *  Created on: Mar 18, 2020
 *      Author: joris
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_

#include "main.h"
#include "tusb.h"

void cdc_task(void);
void uart_init(void);
void UART_Early_Exit(UART_HandleTypeDef *huart, uint32_t CNDTR);
void UART_Reset(void);

#endif /* INC_UART_DRIVER_H_ */
