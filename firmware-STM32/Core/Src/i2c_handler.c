/*
 * i2c_handler.c
 *
 *  Created on: Feb 25, 2020
 *      Author: joris
 */
#include "stm32f1xx_hal.h"
#include "i2c_handler.h"
#include "interrupt_pin.h"

/************* I2C Register space mapping *********************
 * Byte		Description
 *
 * 0: 		Interrupt pin reason bitmask (0 = no interrupt, 1 = general, 2 = button state change, 4 = incoming USB MIDI message
 * 1-3:		Reserved
 *
 * 4-5:		Keypad touch state bitmask (little endian, 1 = top left, 32768 = bottom right)
 * 6-9:		Reserved
 *
 * 10-57:	LED PWM values (3-byte RGB per LED, starting from top left, row-wise)
 * 58:		LED dirty byte (0 = data is unchanged, 1 = LEDs should be updated by STM32)
 * 59-63:	Reserved
 *
 * 64:		USB HID key modifier to send over USB
 * 65-70:	USB HID keycodes to send over USB (max 6 concurrent ones)
 * 71:		USB HID keyboard dirty byte (0 = data is unchanged, 1 = keycodes should be sent by STM32 over USB)
 *
 * 72:		USB HID mouse button bitmask (1 = left mouse, 2 = right mouse, 4 = middle mouse, etc)
 * 73:		USB HID mouse relative x movement (signed byte)
 * 74:		USB HID mouse relative y movement (signed byte)
 * 75:		USB HID mouse wheel (signed byte)
 * 76:		USB HID mouse horizontal pan (signed byte)
 * 77:		USB HID mouse dirty byte (0 = data is unchanged, 1 = mouse data should be sent by STM32 over USB)
 *
 * 78-80:	USB MIDI data slot 1 (commonly 0x90 Note On or 0x80 Note Off, then a frequency, then a volume)
 * 81:		USB MIDI dirty byte 1 (0 = data is unchanged, 1 = midi data slot 1 should be sent by STM32 over USB)
 * 82-84:	USB MIDI data slot 2 (commonly 0x90 Note On or 0x80 Note Off, then a frequency, then a volume)
 * 85:		USB MIDI dirty byte 2 (0 = data is unchanged, 1 = midi data slot 2 should be sent by STM32 over USB)
 * 86-88:	USB MIDI data slot 3 (commonly 0x90 Note On or 0x80 Note Off, then a frequency, then a volume)
 * 89:		USB MIDI dirty byte 3 (0 = data is unchanged, 1 = midi data slot 3 should be sent by STM32 over USB)
 * 90-92:	USB MIDI data slot 4 (commonly 0x90 Note On or 0x80 Note Off, then a frequency, then a volume)
 * 93:		USB MIDI dirty byte 4 (0 = data is unchanged, 1 = midi data slot 4 should be sent by STM32 over USB)
 *
 * 94-96:	USB MIDI incoming data
 * 97-128:	Reserved
 *
 ***************************************************************/

uint8_t i2cbuf[30];
uint8_t i2cregisters[128] = {0};
uint8_t first = 1;
uint8_t address;

//uint32_t startTick;

//void i2c_watchdog(I2C_HandleTypeDef *hi2c) {
//	uint8_t* dirty_byte = (uint8_t*) getI2CMemory(58);

//	if(hi2c->State != HAL_I2C_STATE_LISTEN && (HAL_GetTick() - startTick) > I2C_Timeout) {
//		HAL_I2C_Init(hi2c);
//		HAL_I2C_EnableListen_IT(hi2c);
//	}
//}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  UNUSED(AddrMatchCode);
  first = 1;
  //startTick = HAL_GetTick();
  if(hi2c->Instance == I2C1) {
	  if(TransferDirection == 1) {	//Write
		  HAL_I2C_Slave_Seq_Receive_IT(hi2c, i2cbuf, 1, I2C_FIRST_FRAME);
	  } else {	//Read
		  HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &i2cregisters[address], 1, I2C_FIRST_FRAME);
	  }
  }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if(address == 0) {
	    resetInterruptPin(); // Reset the interrupt pin when the pin reason register has been read
	    clearInterruptReason();
    }

	address++;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &i2cregisters[address], 1, I2C_FIRST_FRAME);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if(first) {
		first = 0;
		address = i2cbuf[0];
	} else {
		i2cregisters[address] = i2cbuf[0];
		address++;
	}
	HAL_I2C_Slave_Seq_Receive_IT(hi2c, i2cbuf, 1, I2C_FIRST_FRAME);
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_EnableListen_IT(hi2c);
}

uint8_t* getI2CMemory(uint32_t pos) {
	return &i2cregisters[pos];
}
