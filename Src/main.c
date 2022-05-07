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
#include "usb_device.h"

//#include "stm32f1xx_hal.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_hid.h"
#include <stdio.h>
#include "Handler.h"

#include "i2cSoft.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hardware.h"
#include "SerialTrans.h"
#include "Toshiba_TC358870_china.h"
#include "bno055.h"
#include "ssd1306.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
USBD_HandleTypeDef hUsbDeviceFS;

//blink LEDs var


uint8_t rxLed=0,txLed=0;

uint8_t uartBusy = 0;

/*CDC buffers declaration*/

char spi_tx[BUF_SIZE];
uint8_t spiCountTx=0;
uint8_t spiWritePointerTx=0, spiReadPointerTx=0;

char spi_rx[BUF_SIZE];
uint8_t spiCountRx=0;
uint8_t spiWritePointerRx=0, spiReadPointerRx=0;
/*HID payload vars***************************************************************************/
uint8_t hid_ver = 3; //Now 3 for OSVR
uint8_t video_detected = 0;
uint8_t packet_num = 0;

uint8_t dataToSend[16];

/*end HID payload vars***********************************************************************/
/*Hardware Vars******************************************************************************/
extern volatile uint8_t sMessage[16];

uint8_t mode = 0; //  0 - SBS mode, 1 - split mode, 
uint8_t target_mode = 0;
uint8_t stage = 0; //0 - initial, 1 - powerstage1, 2 = 5V is ok, 3 - toshiba init, 4 - powerstage2, 5 - display init
uint8_t new_stage = 2;
uint8_t edid = 0; //0 - SBS mode OSVR edid, 1 - SBS mode ORIG edid, 2 - SBS mode CUSTOM edid
uint8_t target_edid = 0;
uint8_t status = 0;
uint8_t old_status = 0;
//uint8_t state = 0; // 1 - DS mode, 2 - split mode, 3 - HDMI off, 4 - HDMI soft off
uint8_t lane_status = 0;
uint8_t edid_ver = 0;
/*End Hardware Vars******************************************************************************/

float PI = 3.14159265358979323846f;
float LINACC [3];
float LINACCOLD [3];
float Euler[3];
float Quat[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float QuatW;
float QuatX;
float QuatY;
float QuatZ;
float Gyro[3] = {0.0f, 0.0f, 0.0f};

int16_t quatCount[7]; 
int16_t QuatWbyte;
int16_t QuatXbyte;
int16_t QuatYbyte;
int16_t QuatZbyte;
int16_t Gxbyte;
int16_t Gybyte;
int16_t Gzbyte;
uint8_t send_report[16] = {0};


float test;

float sinr_cosp =0.0f;
float cosr_cosp =0.0f;
float sinp =0.0f;
float siny_cosp =0.0f;
float cosy_cosp =0.0f;

float prevVelocity[3];
float newVelocity[3];
float prevPosition[3];
float prevAccel[3];
float Position[3];
float nevalgo[3];
float Abs[3];
float AbsOld[3];
float EulerF[3];
float EulerF2[3];

float vzero[3];
float szero[3];

uint64_t lastRotationUpdate = 0.0f;
volatile uint64_t now = 0.0f;
float elapsedTime = 0.0f;


int optimer=0;
uint16_t Code;
uint8_t bufArd[30];
uint8_t cnt=0;
uint8_t status;
uint8_t test_result;
uint8_t error;

uint8_t sys=0;
uint8_t gyro=0;
uint8_t accel=0;
uint8_t mag=0;

uint8_t OLED_OK = 0;
int     HT_OK = 0;
void print(char string[]);

void floatToByteArray(float f, uint8_t *barr, uint8_t start) {

    unsigned int asInt = *((int*)&f);

    int i;
    for (i = 0; i < 4; i++) {
        barr[start+i] = (asInt >> 8 * i) & 0xFF;
    }
}



uint64_t GetMicros(void)
{
	register uint64_t systick, ticks;
	do {
	ticks = (uint64_t)HAL_GetTick();

	 systick = (uint64_t)SysTick->VAL;
	}
	 while (ticks != (uint64_t)HAL_GetTick());
	return (ticks * 1000ull) + (72ul*1000ull - systick )/ 72ul;
}
void processEuler()
{

	getEuler(VECTOR_EULER, Euler);

	/* sinr_cosp = 2.0f*(QuatY*QuatW-QuatX*QuatZ);
	 cosr_cosp = 1.0f-2.0f*(QuatY*QuatY+QuatZ*QuatZ);
	 sinp = 2.0f*(QuatX*QuatY + QuatZ*QuatW);
	 siny_cosp = 2.0f*(QuatX*QuatW-QuatY*QuatZ);
	 cosy_cosp = 1.f-2.0f*(QuatX*QuatX+QuatZ*QuatZ);

 Euler[0]=atan2(sinr_cosp, cosr_cosp);
 if(fabs(sinp)>=1){
 Euler[1]=copysign(PI/2,sinp);
 }
 else {
 		  Euler[1]=asin(sinp);
 	  }
 Euler[2]=atan2(siny_cosp, cosy_cosp);
 */
	  EulerF[0]=Euler[0]-180.0f;
	  EulerF[1]=Euler[1];
	  EulerF[2]=Euler[2];
} 

void CalibrateHeadTracker()
{
  
     while(sys != 3)
    {
    	char str[5];
    getCalibration(&sys, &gyro, &accel, &mag);
    
    LED1_TOGGLE;
    LED2_TOGGLE;    
    print("CALIBRATION: Sys=");
    sprintf(str, "0x%X",sys);
    HAL_Delay (500);
    SerialPrint(str);
    HAL_Delay (500);
    SerialPrint(" Gyro=");
    sprintf(str, "0x%X",gyro);
    HAL_Delay (500);
    SerialPrint(str);
    HAL_Delay (500);
    SerialPrint(" Accel=");
    sprintf(str, "0x%X",accel);
    HAL_Delay (500);
    SerialPrint(str);
    HAL_Delay (500);
    SerialPrint(" Mag=");
    sprintf(str, "0x%X",mag);
    HAL_Delay (500);
    SerialPrint(str);
    HAL_Delay (500);

    SerialPrint("\r\n");
    HAL_Delay (500);
  }
}

 
void processcalcAbs()
{
	lastRotationUpdate = now;
	now = GetMicros();
	elapsedTime = (now - lastRotationUpdate) / 1000000.0f; // set integration time by time elapsed since last filter update

    getLinear(VECTOR_LINEARACCEL, LINACC);

    LINACCOLD[0] = LINACCOLD[0]*0.995f+LINACC[0]*0.005f;
    LINACCOLD[1] = LINACCOLD[1]*0.995f+LINACC[1]*0.005f;
    LINACCOLD[2] = LINACCOLD[2]*0.995f+LINACC[2]*0.005f;

    newVelocity[0] = prevVelocity[0]+ LINACCOLD[0]* elapsedTime;
    newVelocity[1] = prevVelocity[1]+ LINACCOLD[1]* elapsedTime;
    newVelocity[2] = prevVelocity[2]+ LINACCOLD[2]* elapsedTime;

    nevalgo[0]+= LINACCOLD[0];
    nevalgo[1]+= LINACCOLD[1];
    nevalgo[2]+= LINACCOLD[2];


    Position[0] += (prevVelocity[0]* elapsedTime + (LINACCOLD[0]* elapsedTime* elapsedTime)/2);
    Position[1] += (prevVelocity[1]* elapsedTime + (LINACCOLD[1]* elapsedTime* elapsedTime)/2);
    Position[2] += (prevVelocity[2]* elapsedTime + (LINACCOLD[2]* elapsedTime* elapsedTime)/2);

    Abs[0] = Abs[0]*0.98f+Position[0]*0.02f;
    Abs[1] = Abs[1]*0.98f+Position[1]*0.02f;
    Abs[2] = Abs[2]*0.98f+Position[2]*0.02f;


    prevVelocity[0]= newVelocity[0];
    prevVelocity[1]= newVelocity[1];   
    prevVelocity[2]= newVelocity[2];
   



}

  void processQuat()
{

	getQuat(VECTOR_QUAT, quatCount);

}
void ProcessHeadTracking()
{
	processQuat();
	processEuler();
	processcalcAbs();
}

void print(char string[])
{
	CDC_Transmit_FS( (unsigned char*) string, strlen(string));
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 2 */
  MainPowerOn();
  HAL_Delay(50);
  i2cSoft_Init();
  HAL_Delay(150);
  for(int i = 0; i < 10; i++)
  {

    
    //ctl_OLED

  }
  //if(!OLED_OK && i2cSoft_DeviceExists(SSD1306_I2C_ADDR)) OLED_OK = 1;
  if(i2cSoft_DeviceExists(SSD1306_I2C_ADDR)) OLED_OK = 1;
  if(i2cSoft_DeviceExists(BNO055_ADDRESS_A)) HT_OK = 1;

  while(1){
  if(i2cSoft_DeviceExists(TOSHIBA_I2C_ADDR))
  break;
  }
  HAL_Delay(50);
  ssd1306_Init();
  HAL_Delay(50);

  if(OLED_OK)
  {
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
  }
  
  HT_OK = begin();
  //while(!VCPInitialized) {}   
    
    now = GetMicros();

  /* USER CODE END 2 */
  
  /* USER CODE BEGIN WHILE */
  //char byte;
UserRxBufferFS[0] = '\0';
  while (1)
  {   
    InitHardware();
    if(OLED_OK)
    {
          PrintScreen();
      }
    StatusControl();
    if(VCPInitialized)
    {
        Cmd_Interface();
        if(HT_OK == -1) SerialPrint("Bno is fail....");    
      }
    Buttons();
    processQuat();
		
		send_report[0]=hid_ver;
    if(status == 0x9F)
    send_report[0] = 0x13;
    send_report[1] = packet_num;
    
    packet_num++;
    for(int i = 0; i < 7; i++)
    {
      send_report[i*2+2] = (uint8_t)(quatCount[i] & 0xFF);
      send_report[i*2+3] = (uint8_t)(quatCount[i] >> 8);


    }
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, send_report, 16);
    if(packet_num == 0xFF)  packet_num = 0;
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_9|GPIO_PIN_0, GPIO_PIN_RESET);
 // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_9, GPIO_PIN_RESET);
  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_2 , GPIO_PIN_SET);
  

  /*Configure GPIO pin Output Level */
 // HAL_GPIO_WritePin(GPIOB, SoftI2C2_SDA_Pin|SoftI2C2_SCL_Pin|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_RESET); //OLED displayys power off
 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET); //Soft I2C pins to low

  /*Configure GPIO pin Output Level */
  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA8 */
// GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8;
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_10|GPIO_PIN_15|GPIO_PIN_9|GPIO_PIN_7|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /*Configure GPIO pins : SoftI2C2_SDA_Pin SoftI2C2_SCL_Pin PB10 PB11 
                           PB12 PB9 */
  GPIO_InitStruct.Pin = SoftI2C2_SDA_Pin|SoftI2C2_SCL_Pin 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB13 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_13|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    /*Configure GPIO pins : PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
