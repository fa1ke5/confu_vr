#include "hardware.h"
#include "stm32f1xx.h"
//#include "cmsis_os.h"
#include "main.h"

#include "usbd_cdc_if.h"
//#include "stm32f1xx_hal_uart.h"
#include "ssd1306.h"
#include "i2cSoft.h"
#include <stdint.h>
#include <stdbool.h>
//#include "Toshiba_TC358870.h"
#include "Toshiba_TC358870_china.h"
//#include "Display_DualAUO_TC358870.c"
#include <stdio.h>
#include "SerialTrans.h"

uint8_t button_a_is_pressed = 0;
uint8_t button_b_is_pressed = 0;
uint8_t button_a_press_cntr = 0;
uint8_t button_b_press_cntr = 0;
bool HMDready = false;
bool DisplayStatus = true;
bool OledNeedUpdate = false;

void ModesSwitch()
{
  if(target_mode != mode && stage == 6)
  {
      DisplaySleep();
      PowerOffStage2();
      stage = 0;
         
  }
  return;
}
void EdidSwitch()
{
    if(target_edid != edid && stage == 6)
  {
      DisplaySleep();
      PowerOffStage2();
      stage = 0;
  }
  return;

}

void Buttons()
{
if(stage == 6)
{
  if(BUTTON_A_STATE == 0)
    {
      button_a_press_cntr++;
      if(button_a_press_cntr >= BUTTON_PRESS_MAX_CNTR )
      {

        button_a_press_cntr = 0;
        button_a_is_pressed = 1;
        }

      }

    else
    {
      if(button_a_is_pressed)
      {

        switch(target_mode)
        {
          case 0:
          target_mode = 1;
          break;
          
          case 1:
          target_mode = 0;
          break;
          
          }
          /*
        SerialPrint("\r\nMode changed to ");
        char a = 0;
        itoa(target_mode, &a, 2);     
        SerialPrint(&a);        
        SerialPrint("\r\n");
        */
       if(OLED_OK) SSD1306_DrawLine(0, 31, 127, 31, 0);
        ModesSwitch();
        button_a_is_pressed = 0;

      }
      button_a_press_cntr = 0;
    }
    if(BUTTON_B_STATE == 0)
    {
      button_b_press_cntr++;
      if(button_b_press_cntr >= BUTTON_PRESS_MAX_CNTR )
      {

        button_b_press_cntr = 0;
        button_b_is_pressed = 1;
        }

      }

    else
    {
      if(button_b_is_pressed)
      {
        switch(target_edid)
        {
          case 0:
          target_edid = 1;
          break;

          case 1:
          target_edid = 2;
          break;

          case 2:
          target_edid = 0;
          break;
          }
/*
        SerialPrint("\r\nEdid changed to ");
        char a = 0;
        itoa(target_edid, &a, 4);     
        SerialPrint(&a);        
        SerialPrint("\r\n");
        */
        if(OLED_OK) SSD1306_DrawLine(0, 31, 127, 31, 0);
        EdidSwitch();
        button_b_is_pressed = 0;

      }
      button_b_press_cntr = 0;
    }

}
    

}
void PrintScreen()
{
    if(status != old_status || OledNeedUpdate)
    {
      char hex_status[] = "na";

      switch(target_edid)
      {
        case 0:
        ssd1306_SetCursor(0, 5); 
        ssd1306_WriteString("OSVR", Font_11x18,White);
        ssd1306_SetCursor(46, 12); 
        ssd1306_WriteString("edid", Font_7x10,White);

        
        sprintf(hex_status, "%02X", mode);
        ssd1306_WriteString(hex_status, Font_7x10,White);

        break;
        
        case 1:
        ssd1306_SetCursor(0, 5); 
        ssd1306_WriteString("ORIG", Font_11x18,White);
        ssd1306_SetCursor(56, 12); 
        ssd1306_WriteString("edid", Font_7x10,White);

         
        sprintf(hex_status, "%02X", mode);
        ssd1306_WriteString(hex_status, Font_7x10,White);
        break;
        
        case 2:
        ssd1306_SetCursor(0, 5); 
        ssd1306_WriteString("CUST", Font_11x18,White);
        ssd1306_SetCursor(56, 12); 
        ssd1306_WriteString("edid", Font_7x10,White);

        
        sprintf(hex_status, "%02X", mode);
        ssd1306_WriteString(hex_status, Font_7x10,White);
        break;

      }
      if(DisplayStatus)
      {
        ssd1306_SetCursor(100, 0); 
        ssd1306_WriteString("sr", Font_7x10,White); 
        ssd1306_SetCursor(100, 10);
       
        sprintf(hex_status, "%02X", status);
        ssd1306_WriteString(hex_status, Font_11x18,White);
        }
        else
        {
        ssd1306_SetCursor(100, 0); 
        ssd1306_WriteString("  ", Font_7x10,White); 
        ssd1306_SetCursor(100, 10);
        ssd1306_WriteString("  ", Font_11x18,White);

        }


     
      ssd1306_UpdateScreen();
      old_status = status;
      if (OledNeedUpdate) OledNeedUpdate = false;
    }

    if(!HMDready)
    {
      //void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c);
      SSD1306_DrawLine(0, 31, stage * 21, 31, 1);
      ssd1306_UpdateScreen();
    }
}

void InitHardware()
{
      status = TC358870_i2c_Read(0x8520); 


    
//0 - initial, 1 - powerstage1, 2 = 5V is ok, 3 - toshiba init, 4 - powerstage2, 5 - display init

    switch (stage)
    {
    case 0:
    
    MainPowerOn();    
    Led_A_Blink(500, 2);
    stage = 1;
    
      break;
    case 1:
    if((status & 0x01) == 1 ) stage = 2;
  //  if(status == 1 ) stage = 2;
    break;

    case 2:

       Toshiba_Init(target_edid);
       Led_A_Blink(200, 2);
       stage = 3;
    break;

    case 25:
    if((status & 0x01) == 1 ) 
    {
      TOSHIBA_RESET_ON;
      HAL_Delay(40);
      stage = 1;
    }
    break;


    case 3:
    if (status == 0x9F)

    {
        HAL_Delay(10);
        stage = 4;
    }
    break;

    case 4:
      DiasplayPowerOn();    
      Led_A_Blink(50, 2);
      stage = 5;
    break;

    case 5:
    Toshiba_TC358870_OLED_SetupChina(target_mode); //Mode: 0 split, 1 - double
    HAL_Delay(240);
    stage = 6;
    mode = target_mode;
    edid = target_edid;
    Led_A_Blink(50, 1);
    break;

    case 6:
      lane_status = TC358870_i2c_Read(0x0290);
      if(!HMDready)
      {
        if(OLED_OK) SSD1306_DrawLine(0, 31, 127, 31, 1);
        OledNeedUpdate = true;
        HMDready = true;

      }
      

      
    break;

    default:
      break;
    }
    


}
void StatusControl()
{

  if(stage == 6)
    {
      //switch (status & 0x80)
      switch (status)
      {
      case 0: //Пропало 5в HDMI выдернули кабель 9F -> 1F -> 2F -> 3F -> 2F -> 3F -> 39 -> 28 (28 ~ 10ms)
      HMDready = false;
      stage = 1;
   //   SSD1306_DrawLine(0, 31, 127, 31, 0);
      Led_A_Blink(1000, 2);

        break;
      case 0x01: //Пропало 5в HDMI выдернули кабель 9F -> 1F -> 2F -> 3F -> 2F -> 3F -> 39 -> 28 (28 ~ 10ms)
      HMDready = false;      
      stage = 1;
   //   SSD1306_DrawLine(0, 31, 127, 31, 0);
      Led_A_Blink(500, 2);


        break;
      case 0x19: //Отключен программно
      DisplaySleep();
      HMDready = false;
      stage = 3;
   //   SSD1306_DrawLine(0, 31, 127, 31, 0);
      Led_A_Blink(100, 2);

        
        break;
        case 0x28: //выдернули кабель 9F -> 1F -> 2F -> 3F -> 2F -> 3F -> 39 -> 28 (28 ~ 10ms) делаем OLED_sleep и ждем status 0xAF тогда поднимаем
        DisplaySleep();
        HMDready = false;
        stage = 3;
    //    SSD1306_DrawLine(0, 31, 127, 31, 0);
        Led_A_Blink(100, 2);

        break;
                break;
        case 0x9F: //Нормальная работа
        if(!HMDready)
        HMDready = true;
        
        break;

        case 0x1F: 

        break;

        case 0x08: 
        HMDready = false;
    //    SSD1306_DrawLine(0, 31, 127, 31, 0);
        Led_A_Blink(100, 2);

        break;

        case 0x2F: 
        HMDready = false;
        Led_A_Blink(100, 2);
    //    SSD1306_DrawLine(0, 31, 127, 31, 0);

        break;

        case 0x3F: 
        HMDready = false;
        Led_A_Blink(100, 2);
    //    SSD1306_DrawLine(0, 31, 127, 31, 0);

        break;
        

        case 0x39: 
        HMDready = false;
        Led_A_Blink(100, 2);
    //    SSD1306_DrawLine(0, 31, 127, 31, 0);

        break;
      
      default:
      DisplaySleep();
      HMDready = false;
      stage = 1;
      Led_A_Blink(100, 2);
    //  SSD1306_DrawLine(0, 31, 127, 31, 0);

      break;
      }

    }
}

void Led_A_Blink(int cicle, int mode) //modes: 0 - off, 1 - on, 2 - blink
{
  switch(mode)
  {
    case 0:
   // osTimerStop(LedTimerHandle);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    break;

    case 1:
   // osTimerStop(LedTimerHandle);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    break;
    case 2:
   // osTimerStop(LedTimerHandle);
   // osTimerStart(LedTimerHandle, cicle);
    
    break;
  }
}

void MainPowerOn(void)
{

TOSHIBA_RESET_OFF;
MAIN_POWER_ON;
OLED_POWER_OFF;
OLED_RESET;
TOSHIBA_RESET_ON;
USB_ON;
HAL_Delay(150);
////LED_PWM_ON;
TOSHIBA_RESET_OFF;

}

void DiasplayPowerOn(void)
{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
OLED_POWER_ON;
HAL_Delay(200);
OLED_SET;
//HAL_Delay(20);
 
}
void DisplaySleep(void)
{
OLED_RESET;
Toshiba_TC358870_OLED_SleepChina();
OLED_POWER_OFF;
}

void PowerOffStage2(void)
{

OLED_RESET;
HAL_Delay(10);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_RESET);
HAL_Delay(10);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
HAL_Delay(100);
}

void MainPowerOff(void)
{
USB_OFF;
MAIN_POWER_OFF;
}

 void Toshiba_Init(int edid_var)
{
/*
  EDID_ORIGINAL 2
  EDID_OCULUS 1
  EDID_OSVR 3
  */

HAL_Delay(20);
TOSHIBA_RESET_ON;
HAL_Delay(40);
TOSHIBA_RESET_OFF;
HAL_Delay(4);
Toshiba_TC358870_HDMI_SetupChina();
EDID_Write(edid_var);
}

void Stage2InitSplit(void)
{

//LED1_OFF;
//Toshiba_TC358870_OLED_SetupChina();
HAL_Delay(120);
Toshiba_TC358870_OLED_SetupChina(0);
//HAL_Delay(40);
HAL_Delay(10);
}

