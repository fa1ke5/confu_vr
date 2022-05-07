#include "SerialTrans.h"
#include "usb_device.h"
//#include "usbd_cdc_hid.h"
#include <stdint.h>
#include "hardware.h"
#include "i2cSoft.h"
#include "Handler.h"
#include "ssd1306.h"
#include <stdbool.h>
#include "usbd_cdc_if.h"


/*CDC buffers declaration*/
extern uint8_t status;

char uart_tx[BUF_SIZE];
uint8_t countTx=0;
uint8_t writePointerTx=0, readPointerTx=0;

char uart_rx[BUF_SIZE];
uint8_t countRx=0;
uint8_t writePointerRx=0, readPointerRx=0;

char tmp[BUF_SIZE];



char cmd[APP_RX_DATA_SIZE + 1];
/*
void cdcTransmit(uint8_t size){
	blinkRX();
	for(int i=0;i<size;i++){
		tmp[i]=uart_rx[readPointerRx];
		readPointerRx=(readPointerRx+1)%BUF_SIZE;
		if(i==49){
			CDC_Transmit_FS((uint8_t*)tmp, 50);
			countRx-=50;
			size-=50;
			i=-1;
		}
	}
	if(size!=0){
		CDC_Transmit_FS((uint8_t*)tmp, size);
		countRx-=size;
	}
}
*/
/*
void VCP_write(uint8_t *buf, uint16_t size)
{
  CDC_Transmit_FS((uint8_t*)buf, size);


}
*/
/*
void uartTransmit(uint8_t size){

	for(int i=0;i<size;i++){
		tmp[i]=uart_tx[readPointerTx];
		readPointerTx=(readPointerTx+1)%BUF_SIZE;
		if(i==31){

			countTx-=32;
			size-=32;
			i=-1;
		}
	}
	if(size!=0){
		countTx-=size;

	}
}
*/
void SerialPrint(char* str)
{
  if(VCPInitialized)
  {
        uint8_t len = 0;
    while(1)
    {
        if (*(str + len)!=0 )
        {
            len++;
        }
        else
        {
            break;
        }
       
    }
    VCP_write((uint8_t *) str, len);
  }


}
static void Display_firmware_details(void)
{
	SerialPrint("Firmware Variant: HDK_OLED");
  SerialPrint("\r\n");
	SerialPrint("Revision: ");
	SerialPrint("DEV 0.0.0.1");
  SerialPrint("\r\n");
	SerialPrint("Build info: ");
	SerialPrint("Special Config Defines:  [none]");

}


static void Display_software_version(void)
{
	char OutString[20];

	SerialPrint("Version ");
	sprintf(OutString, "%d.%2.2d%s", 2, 00, " (DEV-IDE)");
	SerialPrint(OutString);
	SerialPrint("  ");
	SerialPrint(__DATE__);
  SerialPrint("\r\n");
}
void ProcessInfoCommands(void)

// process commands that start with '?'

{
	char OutString[12];

	switch (cmd[1])
	{
	case 'V':  // version
	case 'v':
	
		Display_software_version();
		break;
	
	case 'F':  // firmware variant/config
	case 'f':
	
		Display_firmware_details();
		break;
	
	case 'C':  // clock
	case 'c':
	
		// todo: add back clock
		// sprintf(OutString, "%2.2d:%2.2d:%2.2d:%2.2d", day,hour,minute,second);
		SerialPrint(OutString);
    SerialPrint("\r\n");
	  break;

	
	case 'h':  // hardware
	case 'H':
	//	PrintHardwareInfoCommand();
		break;
	}
}

void Cmd_Interface ()
{

   
    uint32_t size = VCP_read(cmd, APP_RX_DATA_SIZE);
   
   if(size)
   {

    uint8_t buf[] = {0, 0, 0};



    switch(cmd[0])    
    {
      case '?':		
			    ProcessInfoCommands();
			break;
		
      case 'H':
      case 'h': 
       // SerialPrint(&buff[1]);
        SerialPrint("\r\n\r\nCommands list:");
        SerialPrint("\r\nH - help");
        SerialPrint("\r\nS - status register");
        SerialPrint("\r\nD - Diasplay status register on OLED");
        SerialPrint("\r\nI - get chip ID");
        SerialPrint("\r\nF - find I2C devices");        
        SerialPrint("\r\n");
        SerialPrint("\r\nInsert command");

        //ssd1306_UpdateScreen();

      return;

      case 'I': // cmd is "I" 
      case 'i':
            buf[0]=0x00;
            buf[1]=0x00;
            if(i2cSoft_WriteBuffer ( TOSHIBA_I2C_ADDR, (uint8_t *)buf, 2 ) != 0)
                  {
                          SerialPrint("\r\nI2C error - device not found");
                                return;
                  }
                   else if(i2cSoft_ReadBuffer ( TOSHIBA_I2C_ADDR, (uint8_t *)buf, 2 ) != 0)
                         {
                                   SerialPrint("\r\nI2C error - device not responding");
                                           return;
                          }

            char a[2];
            SerialPrint("Toshiba chip ID = 0x");


            itoa(buf[0], a, 16);
            SerialPrint(a);
            if(buf[0] == 0)SerialPrint(a);
            itoa(buf[1], a, 16);
            SerialPrint(a);
            if(buf[1] == 0)SerialPrint(a);
            SerialPrint("\r\n\r\nInsert command");
                  return;
      case 'D': //Cmd is "D"
      case 'd':
      switch (DisplayStatus)
      {
        case 0:
        SerialPrint("\r\nStatus register display ON");
        DisplayStatus = true;
        OledNeedUpdate = true;
        break;

        case 1:
        SerialPrint("\r\nStatus register display Off");
        DisplayStatus = false;
        OledNeedUpdate = true;
        break;

      }

      return;
  
      case 'S': //Cmd is "S"
      case 's':
      
      sprintf((char *)buf, "%02X", (const char )status);
      
      SerialPrint("\r\nStatus register: 0x");
      SerialPrint((char *)buf);
                  return;

      case 'F': //cmd is "F"
      case 'f':
            for (uint8_t i = 8; i < 127; i++)
                  {
                       if(i2cSoft_DeviceExists(i))
                             {
                                SerialPrint("\r\nFound device from address - 0x");         
                                //char myValue = i;
                                char a[3];
                                itoa((char)i, a, 16);
                                VCP_write(&a, 3);
                                HAL_Delay(10);
                              }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                    }
                          return;      
      
            case 'O': //cmd is "O"
            case 'o':

            switch(cmd[1])
            {
              case '\r':
              case '\n':
              SerialPrint("Test print 1234567890");

              break;
              default:
              SerialPrint("Default");
              break;
            }
              case 'V': //SxS video mode toggle
              case 'v':
              /*
              if(mode)
              {
                new_stage = 2;
              target_mode = !mode;
              SerialPrint("Target node = 0");

              }

              else
              {
                new_stage = 2;
                target_mode = !mode;
                SerialPrint("Target node = 1");
              }
              */

              break;

      default:

    SerialPrint("\r\n");
    SerialPrint(" - Unknown command, send - H for help.");

      break;

    }
   // return;


   }
}