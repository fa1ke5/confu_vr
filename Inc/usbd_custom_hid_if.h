/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.h
  * @version        : v2.0_Cube
  * @brief          : Header for usbd_custom_hid_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CUSTOM_HID_IF_H__
#define __USBD_CUSTOM_HID_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_CUSTOM_HID USBD_CUSTOM_HID
  * @brief Usb custom human interface device module.
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Defines USBD_CUSTOM_HID_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */
/*
#define cs_set()  HAL_GPIO_WritePin(SPI_CS_Port, SPI_CS_Pin, GPIO_PIN_RESET)
#define cs_reset()  HAL_GPIO_WritePin(SPI_CS_Port, SPI_CS_Pin, GPIO_PIN_SET)
*/

#define USB_SETTINGS			1
#define USB_ANSWER				2
#define USB_UART_TX				3
#define USB_UART_RX				4
#define USB_SPI_TX				5
#define USB_SPI_RX				6

#define UART_BUSY_RESET		0
#define UART_BUSY_SET			1
#define CS_SET						2
#define CS_RESET					3
/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Types USBD_CUSTOM_HID_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */
typedef enum _CUSTOMHID_REPORT_ID
{
  REP_ID_IR_CODE_INTERRUPT       = 1,
  REP_ID_GET_FIRMWARE_VERSION    = 0x10,
  REP_ID_CONTROL_PC_ENABLE       = 0x11,
  REP_ID_FORWARD_IR_ENABLE       = 0x12,
  REP_ID_POWER_ON_IR_CODE        = 0x13,
  REP_ID_POWER_OFF_IR_CODE       = 0x14,
  REP_ID_RESET_IR_CODE           = 0x15,
  REP_ID_MINIMUM_REPEATS         = 0x16,
  REP_ID_CURRENT_TIME            = 0x17,
  REP_ID_CLOCK_CORRECTION        = 0x18,
  REP_ID_WAKEUP_TIME             = 0x19,
  REP_ID_WAKEUP_TIME_SPAN        = 0x1A,
  REP_ID_REQUEST_BOOTLOADER      = 0x50,
  REP_ID_WATCHDOG_ENABLE         = 0x51,
  REP_ID_WATCHDOG_RESET          = 0x52
} CUSTOMHID_REPORT_ID;
/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Macros USBD_CUSTOM_HID_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** CUSTOMHID Interface callback. */
extern USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_FunctionsPrototype USBD_CUSTOM_HID_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CUSTOM_HID_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
