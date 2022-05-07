/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"
#include "Handler.h"
#include "hardware.h"
#include "SerialTrans.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
 /* USER CODE BEGIN 0 */ 
	//from PC
0x06, 0xFF, 0xFF,	// 04|2   , Usage Page (vendor defined?)
        0x09, 0x01,	// 08|1   , Usage      (vendor defined
        0xA1, 0x01,	// A0|1   , Collection (Application)
        // IN report
        0x09, 0x02,	// 08|1   , Usage      (vendor defined)
        0x09, 0x03,	// 08|1   , Usage      (vendor defined)
        0x15, 0x00,	// 14|1   , Logical Minimum(0 for signed byte?)
        0x26, 0xFF, 0x00,	// 24|1   , Logical Maximum(255 for signed byte?)
        0x75, 0x08,	// 74|1   , Report Size(8) = field size in bits = 1 byte
        // 94|1   , ReportCount(size) = repeat count of previous item
        0x95, 0x10, //sizeof(udi_hid_generic_report_in), //Out report size 16*********************************
        0x81, 0x02,	// 80|1   , IN report (Data,Variable, Absolute)
        // OUT report
        0x09, 0x04,	// 08|1   , Usage      (vendor defined)
        0x09, 0x05,	// 08|1   , Usage      (vendor defined)
        0x15, 0x00,	// 14|1   , Logical Minimum(0 for signed byte?)
        0x26, 0xFF, 0x00,	// 24|1   , Logical Maximum(255 for signed byte?)
        0x75, 0x08,	// 74|1   , Report Size(8) = field size in bits = 1 byte
        // 94|1   , ReportCount(size) = repeat count of previous item
        0x95, 0x40,// sizeof(udi_hid_generic_report_out), //In report size 64***********************************
        0x91, 0x02,	// 90|1   , OUT report (Data,Variable, Absolute)
        // Feature report
        0x09, 0x06,	// 08|1   , Usage      (vendor defined)
        0x09, 0x07,	// 08|1   , Usage      (vendor defined)
        0x15, 0x00,	// 14|1   , LogicalMinimum(0 for signed byte)
        0x26, 0xFF, 0x00,	// 24|1   , Logical Maximum(255 for signed byte)
        0x75, 0x08,	// 74|1   , Report Size(8) =field size in bits = 1 byte
        0x95, 0x10, //sizeof(udi_hid_generic_report_feature),	// 94|x   , ReportCount in byte //UDI_HID_REPORT_FEATURE_SIZE        16
        0xB1, 0x02,	// B0|1   , Feature report
        0xC0	// C0|0   , End Collection

};

/* USER CODE BEGIN PRIVATE_VARIABLES */
//extern char uart_tx[BUF_SIZE], spi_tx[BUF_SIZE];
extern uint8_t countTx, writePointerTx, spiCountTx, spiWritePointerTx;
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
extern uint8_t uartBusy;
/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

//static int8_t CustomHID_OutEvent    (uint8_t event_idx, uint8_t* buffer);
static int8_t CustomHID_SetFeature  (uint8_t event_idx, uint8_t* buffer);
static int8_t CustomHID_GetFeature  (uint8_t event_idx, uint8_t* buffer, uint16_t* length);
static uint16_t CustomHID_FeatureReportLength(uint8_t event_idx);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS,
  CustomHID_SetFeature,
  CustomHID_GetFeature
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{

  
  USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef*)hUsbDeviceFS.pClassData;
  uint8_t id = hhid->Report_buf[0];
  uint8_t id1 = hhid->Report_buf[1];

  //uint8_t informByte = hhid->Report_buf[1];

//SerialPrint("HID Packet recieved");////
	if ((id == 0) && (id1 == 1))
	{
		
      //    HMDready = false;      
   //   stage = 1;
    //Display_Off(Display1);
		// The report is correct
	}
    

		

  return (USBD_OK);
  /* USER CODE END 6 */ 
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/

/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
 * @brief  CustomHID_OutEvent
 *         Manage the CUSTOM HID class Out Event
 *         Host -> Device
 * @param  event_idx: Report Number
 * @param  buffer: Received Data
 * @retval USBD_OK
 */
/*
static int8_t CustomHID_OutEvent(uint8_t event_idx, uint8_t* buffer)
{

   switch(event_idx)
   {
   case REP_ID_CONTROL_PC_ENABLE:
       write command to FIFO from where it will be sent later
      FIFO_Write(&irsnd_fifo, (fifo_entry_t*)buffer);
      break;

   default: 
   }

   return (USBD_OK);
}
*/

/**
 * @brief  CustomHID_SetFeature
 *         Manage the CUSTOM HID SetFeature request.
 *         Host -> Device
 * @param  event_idx: Report Number
 * @param  buffer: Received Data
 * @retval USBD_OK
 */
static int8_t CustomHID_SetFeature(uint8_t event_idx, uint8_t* buffer)
{
 //  swrtc_time_t   time;
 //  uint32_t       alarm;

   switch(event_idx)
   {
   case REP_ID_CONTROL_PC_ENABLE:
    //  hidirt_data_shadow.control_pc_enable = buffer[0];
     // hidirt_data_shadow.data_update_pending = REP_ID_CONTROL_PC_ENABLE;
      break;

   case REP_ID_FORWARD_IR_ENABLE:
     // hidirt_data_shadow.forward_ir_enable = buffer[0];
     // hidirt_data_shadow.data_update_pending = REP_ID_FORWARD_IR_ENABLE;
      break;



   default: /* Report does not exist */

   if ((buffer[0] == 0x71) && (buffer[1] == 0x25))
	{
		// The report feature is correct
		if (buffer[2] == 1)
		{

			if (buffer[3] == 0)
			{
        SerialPrint("SxS_Disable();");
        //SxS_Disable();  // normal mode
			}
			else
			{
        SerialPrint("SxS_Enable();");
				//SxS_Enable();  // side by side
			}
    }
  }



      break;
   }

   return (USBD_OK);
}

/**
 * @brief  CustomHID_GetFeature
 *         Manage the CUSTOM HID GetFeature request.
 *         Device -> Host
 * @param  event_idx: Requested Report Number
 * @param  buffer: Data to transmit including ReportID
 * @param  length: Length of the buffer
 * @retval length: Number of bytes to send
 * @retval USBD_OK
 */
static int8_t CustomHID_GetFeature(uint8_t event_idx, uint8_t* buffer, uint16_t* length)
{
   //swrtc_time_t   time;
  // uint32_t       alarm;

   // clear transmission data array
   memset(buffer, 0x00, *length);

   // retrieve currently valid config
 //  GetHidirtConfig(&hidirt_data_shadow);

   switch(event_idx)
   {
   case REP_ID_GET_FIRMWARE_VERSION:
   //   memcpy(&buffer[0],
    //         &FirmwareVersion[0],
     //        sizeof(FirmwareVersion));
      break;

   case REP_ID_CONTROL_PC_ENABLE:
    //  memcpy(&buffer[0],
     //        &hidirt_data_shadow.control_pc_enable,
     //        sizeof(hidirt_data_shadow.control_pc_enable));
      break;



   default: /* Report does not exist */
      return (USBD_FAIL);
      break;
   }




//   memcpy(&buffer[0], &f_report, 8);
//buffer[0] = 0x11;

   //*length = CustomHID_FeatureReportLength(event_idx);
 //  *length = 16;
   
   return (USBD_OK);
}

/**
 * @brief  CustomHID_FeatureReportLength
 *         Get length of a specific feature report (data without ReportID).
 * @param  event_idx: Requested Report Number
 * @retval length: Number of bytes of the according report.
 */
uint16_t CustomHID_FeatureReportLength(uint8_t event_idx)
{
   uint16_t length = 0;

   // Get length of corresponding data
   switch (event_idx)
   {
   case REP_ID_GET_FIRMWARE_VERSION:
     // length = sizeof(FirmwareVersion);
      break;

   case REP_ID_CONTROL_PC_ENABLE:
   case REP_ID_FORWARD_IR_ENABLE:
   case REP_ID_WATCHDOG_ENABLE:
   case REP_ID_WATCHDOG_RESET:
      //length = sizeof(bool);
      break;

   case REP_ID_POWER_ON_IR_CODE:
   case REP_ID_POWER_OFF_IR_CODE:
   case REP_ID_RESET_IR_CODE:
      //length = sizeof(IRMP_DATA);
      break;

   case REP_ID_MINIMUM_REPEATS:
   case REP_ID_WAKEUP_TIME_SPAN:
   case REP_ID_REQUEST_BOOTLOADER:
      //length = sizeof(uint8_t);
      break;

   case REP_ID_CURRENT_TIME:
      //length = sizeof(swrtc_time_t);
      break;

   case REP_ID_CLOCK_CORRECTION:
   case REP_ID_WAKEUP_TIME:
      //length = sizeof(hidirt_data_shadow.clock_correction);
      break;

   default:
      break;
   }


   return length;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

