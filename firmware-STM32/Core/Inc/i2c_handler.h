/*
 * i2c_handler.h
 *
 *  Created on: Mar 16, 2020
 *      Author: joris
 */

#ifndef INC_I2C_HANDLER_H_
#define INC_I2C_HANDLER_H_

#define I2C_Timeout 50

void i2c_watchdog();
uint8_t* getI2CMemory(uint32_t pos);

#endif /* INC_I2C_HANDLER_H_ */
