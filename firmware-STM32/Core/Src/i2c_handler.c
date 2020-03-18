/*
 * i2c_handler.c
 *
 *  Created on: Feb 25, 2020
 *      Author: joris
 */
#include "stm32f1xx_hal.h"
#include "i2c_handler.h"

uint8_t i2cbuf[30];
uint8_t config_map[64] = {1, 2, 3, 4};
uint8_t first = 1;
uint8_t address;

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  UNUSED(AddrMatchCode);
  first = 1;
  if(hi2c->Instance == I2C1) {
	  if(TransferDirection == 1) {	//Write
		  HAL_I2C_Slave_Seq_Receive_IT(hi2c, i2cbuf, 1, I2C_FIRST_FRAME);
	  } else {	//Read
		  HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &config_map[address], 1, I2C_FIRST_FRAME);
	  }
  }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	address++;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &config_map[address], 1, I2C_FIRST_FRAME);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if(first) {
		first = 0;
		address = i2cbuf[0];
	} else {
		config_map[address] = i2cbuf[0];
		address++;
	}
	HAL_I2C_Slave_Seq_Receive_IT(hi2c, i2cbuf, 1, I2C_FIRST_FRAME);
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_EnableListen_IT(hi2c);
}

uint8_t* getMemory(uint32_t pos) {
	return &config_map[pos];
}
