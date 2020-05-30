/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "class/dfu/dfu_rt_device.h"
#include "tusb.h"
#include "usb_descriptors.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//						 badge_id,   rev,   bf,  future     ,firmware programmed
uint8_t identifier[8] = {0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
#define URL  "updater.hackz.one"

const tusb_desc_webusb_url_t desc_url =
{
  .bLength         = 3 + sizeof(URL) - 1,
  .bDescriptorType = 3, // WEBUSB URL type
  .bScheme         = 1, // 0: http, 1: https
  .url             = URL
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USB_PCD_Init(void);
/* USER CODE BEGIN PFP */
void webusb_task();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ApplicationAddress 0x08004000
#define VersionAddress     0x08003C00

typedef void (*pFunction)(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
    	__disable_irq();
    	__HAL_RCC_GPIOA_CLK_DISABLE();
    	uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    	pFunction Jump_To_Boot = (pFunction) JumpAddress;
    	__set_MSP(*(__IO uint32_t*) ApplicationAddress);
    	SCB->VTOR = ApplicationAddress;
    	Jump_To_Boot();
    }
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */
  tusb_init();
  uint8_t *app_version = (uint8_t *) VersionAddress;
  memcpy(&identifier[6], app_version, 2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  tud_task();
	  webusb_task();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

uint32_t write_address = 0;
uint32_t data_buf[256];

/* USER CODE BEGIN 4 */
// Invoked when received VENDOR control request
bool tud_vendor_control_request_cb(uint8_t rhport, tusb_control_request_t const * request)
{
	uint32_t wipe_address;
  switch (request->bRequest)
  {
    case VENDOR_REQUEST_WEBUSB:
      // match vendor request in BOS descriptor
      // Get landing page url
      return tud_control_xfer(rhport, request, (void*) &desc_url, desc_url.bLength);

    case VENDOR_REQUEST_MICROSOFT:
      if ( request->wIndex == 7 )
      {
        // Get Microsoft OS 2.0 compatible descriptor
        uint16_t total_len;
        memcpy(&total_len, desc_ms_os_20+8, 2);

        return tud_control_xfer(rhport, request, (void*) desc_ms_os_20, total_len);
      }else
      {
        return false;
      }

    case 0x22:	//Reply badge id
      // response with status OK
      return tud_control_xfer(rhport, request, identifier, 8);

    case 0x23:	//Wipe flash page
    	wipe_address = 0x08000000+1024*request->wValue;
    	if(wipe_address >= ApplicationAddress) {	//Only wipe pages if on the application address
			HAL_FLASH_Unlock();
			FLASH_EraseInitTypeDef erase = {FLASH_TYPEERASE_PAGES, 0, wipe_address, 1};
			uint32_t page;
			HAL_FLASHEx_Erase(&erase, &page);
			HAL_FLASH_Lock();
    	}
    	return tud_control_status(rhport, request);

    case 0x24:	//Write flash page
    	write_address = 0x08000000+1024*request->wValue;
    	return tud_control_status(rhport, request);

    case 0x25:  //Read flash page
    	tud_vendor_write((uint8_t *) 0x08000000+1024*request->wValue, 1024);
    	return tud_control_status(rhport, request);

    case 0x26:
    	NVIC_SystemReset();
    	return false;

    case 0x27:
    	HAL_FLASH_Unlock();
    	FLASH_EraseInitTypeDef erase = {FLASH_TYPEERASE_PAGES, 0, VersionAddress, 1};
		uint32_t page;
		HAL_FLASHEx_Erase(&erase, &page);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, VersionAddress, request->wValue);
		HAL_FLASH_Lock();
    	return tud_control_status(rhport, request);
    case 0x28:
    	return tud_control_xfer(rhport, request, &write_address, 4);

    default:
      // stall unknown request
      return false;
  }

  return true;
}

// Invoked when DATA Stage of VENDOR's request is complete
bool tud_vendor_control_complete_cb(uint8_t rhport, tusb_control_request_t const * request)
{
  (void) rhport;
  (void) request;

  // nothing to do
  return true;
}

void webusb_task() {
	if(tud_vendor_available() >= 1024) {
		tud_vendor_read(data_buf, 1024);
		if(write_address < ApplicationAddress) return; //If writing below applicationaddress return
		HAL_FLASH_Unlock();
		for(int i = 0; i < 256; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_address+i*4, data_buf[i]);
		}
		HAL_FLASH_Lock();
		write_address = 0;
		tud_vendor_write("ok", 3);
	}

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
