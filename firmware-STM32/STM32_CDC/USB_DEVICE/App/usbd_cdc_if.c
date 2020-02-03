/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_cdc_if.c
 * @version        : v2.0_Cube
 * @brief          : Usb device for Virtual Com Port.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  512
#define APP_TX_DATA_SIZE  512
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

USBD_CDC_LineCodingTypeDef LineCoding = { 115200, /* baud rate*/
0x00, /* stop bits-1*/
0x00, /* parity - none*/
0x08 /* nb. of bits 8*/
};

uint32_t bufferpos;

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
static void ComPort_Config(void);
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
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

	/* Set Application Buffers */
	USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);

	bufferpos = 0;

	if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) UserTxBufferFS,
			APP_RX_DATA_SIZE / 2) != HAL_OK) {
		/* Transfer error in reception process */
		Error_Handler();
	}

	return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
	if (HAL_UART_DeInit(&huart2) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}
	return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
	uint8_t wValue;
	uint8_t dtr;
	uint8_t rts;
	switch (cmd) {
	case CDC_SEND_ENCAPSULATED_COMMAND:

		break;

	case CDC_GET_ENCAPSULATED_RESPONSE:

		break;

	case CDC_SET_COMM_FEATURE:

		break;

	case CDC_GET_COMM_FEATURE:

		break;

	case CDC_CLEAR_COMM_FEATURE:

		break;

		/*******************************************************************************/
		/* Line Coding Structure                                                       */
		/*-----------------------------------------------------------------------------*/
		/* Offset | Field       | Size | Value  | Description                          */
		/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
		/* 4      | bCharFormat |   1  | Number | Stop bits                            */
		/*                                        0 - 1 Stop bit                       */
		/*                                        1 - 1.5 Stop bits                    */
		/*                                        2 - 2 Stop bits                      */
		/* 5      | bParityType |  1   | Number | Parity                               */
		/*                                        0 - None                             */
		/*                                        1 - Odd                              */
		/*                                        2 - Even                             */
		/*                                        3 - Mark                             */
		/*                                        4 - Space                            */
		/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
		/*******************************************************************************/
	case CDC_SET_LINE_CODING:
		LineCoding.bitrate = (uint32_t) (pbuf[0] | (pbuf[1] << 8)
				| (pbuf[2] << 16) | (pbuf[3] << 24));
		LineCoding.format = pbuf[4];
		LineCoding.paritytype = pbuf[5];
		LineCoding.datatype = pbuf[6];
		ComPort_Config();
		break;

	case CDC_GET_LINE_CODING:
		pbuf[0] = (uint8_t) (LineCoding.bitrate);
		pbuf[1] = (uint8_t) (LineCoding.bitrate >> 8);
		pbuf[2] = (uint8_t) (LineCoding.bitrate >> 16);
		pbuf[3] = (uint8_t) (LineCoding.bitrate >> 24);
		pbuf[4] = LineCoding.format;
		pbuf[5] = LineCoding.paritytype;
		pbuf[6] = LineCoding.datatype;

		break;

	case CDC_SET_CONTROL_LINE_STATE:

		wValue = pbuf[2];
		dtr = (wValue & 0x01) ? 1 : 0;
		rts = (wValue & 0x02) ? 1 : 0;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, rts);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, dtr);
		break;

	case CDC_SEND_BREAK:

		break;

	default:
		break;
	}

	return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
	HAL_UART_Transmit_DMA(&huart2, Buf, *Len);
	return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
	USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*) Buf, Len);
	USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
 * @brief  ComPort_Config
 *         Configure the COM Port with the parameters received from host.
 * @param  None.
 * @retval None.
 * @note   When a configuration is not supported, a default value is used.
 */
static void ComPort_Config(void) {
	if (HAL_UART_DeInit(&huart2) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	/* set the Stop bit */
	switch (LineCoding.format) {
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
	switch (LineCoding.paritytype) {
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
	switch (LineCoding.datatype) {
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

	huart2.Init.BaudRate = LineCoding.bitrate;
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

/**
 * @brief  Rx Transfer completed callback
 * @param  huart: UART handle
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	bufferpos = !bufferpos;
	if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) &UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2],
				APP_RX_DATA_SIZE / 2) != HAL_OK) {
		Error_Handler();
	}
	CDC_Transmit_FS(&UserTxBufferFS[!bufferpos*APP_TX_DATA_SIZE/2+APP_TX_DATA_SIZE/4], APP_TX_DATA_SIZE/4); //Invert bufferpos again because we inverted it for the receive call

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

		CDC_Transmit_FS(&UserTxBufferFS[!bufferpos*APP_TX_DATA_SIZE/2+offset], len); //Invert bufferpos again because we inverted it for the receive call
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	CDC_Transmit_FS(&UserTxBufferFS[bufferpos*APP_TX_DATA_SIZE/2], APP_TX_DATA_SIZE/4);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	USBD_CDC_ReceivePacket(&hUsbDeviceFS);
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
