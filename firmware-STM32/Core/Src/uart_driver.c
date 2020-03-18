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

uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

uint32_t bufferpos;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

void ComPort_Config(void);

void uart_init(void) {
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if (HAL_UART_Init(&huart2) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	bufferpos = 0;

	if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) UserTxBufferFS, APP_RX_DATA_SIZE / 2) != HAL_OK) {
		/* Transfer error in reception process */
		Error_Handler();
	}

}


void cdc_task(void)
{
    // connected and there are data available
    if ( tud_cdc_available() && huart2.gState == HAL_UART_STATE_READY)
    {
      // read and echo back
      uint16_t count = tud_cdc_read(UserRxBufferFS, sizeof(APP_RX_DATA_SIZE));
      if(HAL_UART_Transmit_DMA(&huart2, UserRxBufferFS, count) != HAL_OK) {
    	  count++;
      }

    }
    tud_cdc_write_flush();

}

// Invoked when received new data
void tud_cdc_rx_cb(uint8_t itf) {

}

// Invoked when received `wanted_char`
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char) {

}

// Invoked when line state DTR & RTS are changed via SET_CONTROL_LINE_STATE
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {

}

// Invoked when line coding is change via SET_LINE_CODING
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding) {
	if (HAL_UART_DeInit(&huart2) != HAL_OK) {
			/* Initialization Error */
			Error_Handler();
		}

		/* set the Stop bit */
		switch (p_line_coding->stop_bits) {
		case 0:
			huart2.Init.StopBits = UART_STOPBITS_1;
			break;
		case 2:
			huart2.Init.StopBits = UART_STOPBITS_2;
			break;
		default:
			huart2.Init.StopBits = UART_STOPBITS_1;
			break;
		}

		/* set the parity bit*/
		switch (p_line_coding->parity) {
		case 0:
			huart2.Init.Parity = UART_PARITY_NONE;
			break;
		case 1:
			huart2.Init.Parity = UART_PARITY_ODD;
			break;
		case 2:
			huart2.Init.Parity = UART_PARITY_EVEN;
			break;
		default:
			huart2.Init.Parity = UART_PARITY_NONE;
			break;
		}

		/*set the data type : only 8bits and 9bits is supported */
		switch (p_line_coding->data_bits) {
		case 0x07:
			/* With this configuration a parity (Even or Odd) must be set */
			huart2.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		case 0x08:
			if (huart2.Init.Parity == UART_PARITY_NONE) {
				huart2.Init.WordLength = UART_WORDLENGTH_8B;
			} else {
				huart2.Init.WordLength = UART_WORDLENGTH_9B;
			}

			break;
		default:
			huart2.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		}


		huart2.Init.BaudRate = p_line_coding->bit_rate;
		huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart2.Init.Mode = UART_MODE_TX_RX;

		if (HAL_UART_Init(&huart2) != HAL_OK) {
			/* Initialization Error */
			Error_Handler();
		}

		/* Start reception: provide the buffer pointer with offset and the buffer size */
		bufferpos = 0;
		HAL_UART_Receive_DMA(&huart2, (uint8_t*) UserTxBufferFS, APP_RX_DATA_SIZE / 2);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	bufferpos = !bufferpos;
	if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
				APP_RX_DATA_SIZE / 2) != HAL_OK) {
		Error_Handler();
	}
	tud_cdc_write(&UserTxBufferFS[!bufferpos*APP_TX_DATA_SIZE/2+APP_TX_DATA_SIZE/4], APP_TX_DATA_SIZE/4); //Invert bufferpos again because we inverted it for the receive call
}

void UART_Early_Exit(UART_HandleTypeDef *huart, uint32_t CNDTR)  {
	bufferpos = !bufferpos;
		if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
					APP_RX_DATA_SIZE / 2) != HAL_OK) {
			Error_Handler();
		}
		uint32_t len = APP_TX_DATA_SIZE/2 - CNDTR; //Received number of bytes by the DMA
		uint32_t offset = len > APP_TX_DATA_SIZE/4 ? APP_TX_DATA_SIZE/4 : 0;
		len = len % (APP_TX_DATA_SIZE/4); //Remove the Half way callback

		tud_cdc_write(&UserTxBufferFS[!bufferpos*APP_TX_DATA_SIZE/2+offset], len); //Invert bufferpos again because we inverted it for the receive call
}

void UART_Reset() {
	bufferpos = 0;
	if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
					APP_RX_DATA_SIZE / 2) != HAL_OK) {
			Error_Handler();
	}
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	tud_cdc_write(&UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2], APP_TX_DATA_SIZE/4);
}
