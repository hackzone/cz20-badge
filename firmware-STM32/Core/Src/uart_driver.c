/*
 * uart_driver.c
 *
 *  Created on: Mar 18, 2020
 *      Author: joris
 */
#include "uart_driver.h"

/* Define size for the receive and transmit buffer over CDC */
#define APP_RX_DATA_SIZE  512
#define APP_TX_DATA_SIZE  512

#define WEBUSB_RX_SIZE 32
#define WEBUSB_TX_SIZE 512

uint8_t WebusbRxBuffer[WEBUSB_RX_SIZE];
uint8_t WebusbTxBuffer[WEBUSB_TX_SIZE];

uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

uint32_t bufferpos;
uint32_t bufferpos_webusb;

uint32_t disablepins;
uint32_t disabletimeout;


extern UART_HandleTypeDef UART_SERIAL;
extern UART_HandleTypeDef UART_WEBUSB;

void ComPort_Config(void);

void uart_init(void) {
	UART_SERIAL.Instance = USART1;
	UART_SERIAL.Init.BaudRate = 115200;
	UART_SERIAL.Init.WordLength = UART_WORDLENGTH_8B;
	UART_SERIAL.Init.StopBits = UART_STOPBITS_1;
	UART_SERIAL.Init.Parity = UART_PARITY_NONE;
	UART_SERIAL.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_SERIAL.Init.Mode = UART_MODE_TX_RX;

	if (HAL_UART_Init(&UART_SERIAL) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	bufferpos = 0;

	if (HAL_UART_Receive_DMA(&UART_SERIAL, (uint8_t*) UserTxBufferFS, APP_TX_DATA_SIZE / 2) != HAL_OK) {
		/* Transfer error in reception process */
		Error_Handler();
	}

	UART_WEBUSB.Instance = USART2;
	UART_WEBUSB.Init.BaudRate = 115200;
	UART_WEBUSB.Init.WordLength = UART_WORDLENGTH_8B;
	UART_WEBUSB.Init.StopBits = UART_STOPBITS_1;
	UART_WEBUSB.Init.Parity = UART_PARITY_NONE;
	UART_WEBUSB.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_WEBUSB.Init.Mode = UART_MODE_TX_RX;

	if (HAL_UART_Init(&UART_WEBUSB) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	bufferpos_webusb = 0;

	if (HAL_UART_Receive_DMA(&UART_WEBUSB, (uint8_t*) WebusbTxBuffer, WEBUSB_TX_SIZE / 2) != HAL_OK) {
		/* Transfer error in reception process */
		Error_Handler();
	}

}


void cdc_task(void)
{
    // connected and there are data available
    if ( tud_cdc_available() && UART_SERIAL.gState == HAL_UART_STATE_READY)
    {
      // read and echo back
      uint16_t count = tud_cdc_read(UserRxBufferFS, APP_RX_DATA_SIZE);
      if(HAL_UART_Transmit_DMA(&UART_SERIAL, UserRxBufferFS, count) != HAL_OK) {
    	  count++;
      }

    }
    tud_cdc_write_flush();
    if(disablepins && HAL_GetTick() > disabletimeout) {
    	disablepins = 0;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1);
    }

}

void webusb_task(void)
{
    // connected and there are data available
    if ( tud_vendor_available() && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0) {
    	if(UART_WEBUSB.gState == HAL_UART_STATE_READY) {
      // read and echo back
      uint16_t count = tud_vendor_read(WebusbRxBuffer, WEBUSB_RX_SIZE);
      if(HAL_UART_Transmit_DMA(&UART_WEBUSB, WebusbRxBuffer, count) != HAL_OK) {
    	  count++;
      }
     }
    }
    //tud_vendor_write_flush();

}

// Invoked when received new data
void tud_cdc_rx_cb(uint8_t itf) {

}

// Invoked when received `wanted_char`
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char) {

}

// Invoked when line state DTR & RTS are changed via SET_CONTROL_LINE_STATE
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
	if((dtr && rts)) {
		disablepins = 1;
		disabletimeout = HAL_GetTick()+10;
	} else {
		disablepins = 0;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, !rts);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, !dtr);
}

// Invoked when line coding is change via SET_LINE_CODING
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding) {
//		if (HAL_UART_DeInit(&UART_SERIAL) != HAL_OK) {
//			/* Initialization Error */
//			Error_Handler();
//		}
		HAL_UART_AbortReceive(&UART_SERIAL);

		/* set the Stop bit */
		switch (p_line_coding->stop_bits) {
		case 0:
			UART_SERIAL.Init.StopBits = UART_STOPBITS_1;
			break;
		case 2:
			UART_SERIAL.Init.StopBits = UART_STOPBITS_2;
			break;
		default:
			UART_SERIAL.Init.StopBits = UART_STOPBITS_1;
			break;
		}

		/* set the parity bit*/
		switch (p_line_coding->parity) {
		case 0:
			UART_SERIAL.Init.Parity = UART_PARITY_NONE;
			break;
		case 1:
			UART_SERIAL.Init.Parity = UART_PARITY_ODD;
			break;
		case 2:
			UART_SERIAL.Init.Parity = UART_PARITY_EVEN;
			break;
		default:
			UART_SERIAL.Init.Parity = UART_PARITY_NONE;
			break;
		}

		/*set the data type : only 8bits and 9bits is supported */
		switch (p_line_coding->data_bits) {
		case 0x07:
			/* With this configuration a parity (Even or Odd) must be set */
			UART_SERIAL.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		case 0x08:
			if (UART_SERIAL.Init.Parity == UART_PARITY_NONE) {
				UART_SERIAL.Init.WordLength = UART_WORDLENGTH_8B;
			} else {
				UART_SERIAL.Init.WordLength = UART_WORDLENGTH_9B;
			}

			break;
		default:
			UART_SERIAL.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		}


		UART_SERIAL.Init.BaudRate = p_line_coding->bit_rate;
		UART_SERIAL.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART_SERIAL.Init.Mode = UART_MODE_TX_RX;

//		if (HAL_UART_Init(&UART_SERIAL) != HAL_OK) {
//			/* Initialization Error */
//			Error_Handler();
//		}

		UART_SERIAL.Instance->BRR = UART_BRR_SAMPLING8(HAL_RCC_GetPCLK2Freq(), UART_SERIAL.Init.BaudRate);

		/* Start reception: provide the buffer pointer with offset and the buffer size */
		bufferpos = 0;
		HAL_UART_Receive_DMA(&UART_SERIAL, (uint8_t*) UserTxBufferFS, APP_TX_DATA_SIZE / 2);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart == &UART_SERIAL) {
		bufferpos = !bufferpos;
		if (HAL_UART_Receive_DMA(&UART_SERIAL, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
					APP_TX_DATA_SIZE / 2) != HAL_OK) {
			Error_Handler();
		}
		tud_cdc_write(&UserTxBufferFS[!bufferpos*APP_TX_DATA_SIZE/2+APP_TX_DATA_SIZE/4], APP_TX_DATA_SIZE/4); //Invert bufferpos again because we inverted it for the receive call
	} else if(huart == &UART_WEBUSB) {
		bufferpos_webusb = !bufferpos_webusb;
		if (HAL_UART_Receive_DMA(&UART_WEBUSB, (uint8_t*) &WebusbTxBuffer[bufferpos_webusb*WEBUSB_TX_SIZE/2], WEBUSB_TX_SIZE / 2) != HAL_OK) {
			Error_Handler();
		}
		tud_vendor_write(&WebusbTxBuffer[!bufferpos_webusb*WEBUSB_TX_SIZE/2+WEBUSB_TX_SIZE/4], WEBUSB_TX_SIZE/4); //Invert bufferpos again because we inverted it for the receive call
	}
}

void UART_Early_Exit(UART_HandleTypeDef *huart, uint32_t CNDTR)  {
	if(huart == &UART_SERIAL) {
		bufferpos = !bufferpos;
		if (HAL_UART_Receive_DMA(&UART_SERIAL, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
					APP_TX_DATA_SIZE / 2) != HAL_OK) {
			Error_Handler();
		}
		uint32_t len = APP_TX_DATA_SIZE/2 - CNDTR; //Received number of bytes by the DMA
		uint32_t offset = len > APP_TX_DATA_SIZE/4 ? APP_TX_DATA_SIZE/4 : 0;
		len = len % (APP_TX_DATA_SIZE/4); //Remove the Half way callback

		tud_cdc_write(&UserTxBufferFS[!bufferpos*APP_TX_DATA_SIZE/2+offset], len); //Invert bufferpos again because we inverted it for the receive call
	} else if(huart == &UART_WEBUSB) {
		bufferpos_webusb = !bufferpos_webusb;
		if (HAL_UART_Receive_DMA(&UART_WEBUSB, (uint8_t*) &WebusbTxBuffer[bufferpos_webusb*WEBUSB_TX_SIZE/2], WEBUSB_TX_SIZE / 2) != HAL_OK) {
			Error_Handler();
		}
		uint32_t len = WEBUSB_TX_SIZE/2 - CNDTR; //Received number of bytes by the DMA
		uint32_t offset = len > WEBUSB_TX_SIZE/4 ? WEBUSB_TX_SIZE/4 : 0;
		len = len % (WEBUSB_TX_SIZE/4); //Remove the Half way callback

		tud_vendor_write(&WebusbTxBuffer[!bufferpos_webusb*WEBUSB_TX_SIZE/2+offset], len); //Invert bufferpos again because we inverted it for the receive call
	}
}

void UART_Reset() {
	bufferpos = 0;
	if (HAL_UART_Receive_DMA(&UART_SERIAL, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
					APP_TX_DATA_SIZE / 2) != HAL_OK) {
			Error_Handler();
	}
}

void WebUSB_Reset() {
	bufferpos_webusb = 0;
	if (HAL_UART_Receive_DMA(&UART_WEBUSB, (uint8_t*) &WebusbTxBuffer[bufferpos_webusb*WEBUSB_TX_SIZE/2], WEBUSB_TX_SIZE / 2) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if(huart == &UART_SERIAL) {
		tud_cdc_write(&UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2], APP_TX_DATA_SIZE/4);
	} else if(huart == &UART_WEBUSB) {
		tud_vendor_write(&WebusbTxBuffer[bufferpos_webusb*WEBUSB_TX_SIZE/2], WEBUSB_TX_SIZE/4);
	}
}
