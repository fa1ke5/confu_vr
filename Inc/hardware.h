#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <stdbool.h>
#include <stdint.h>
//#define MAX(x, y) (((x) > (y)) ? (x) : (y))
//#define MIN(x, y) (((x) < (y)) ? (x) : (y))

//I2C
#define SoftI2C2_SDA_Pin GPIO_PIN_0
#define SoftI2C2_SDA_GPIO_Port GPIOB
#define SoftI2C2_SCL_Pin GPIO_PIN_1
#define SoftI2C2_SCL_GPIO_Port GPIOB
#define PWM_Pin GPIO_PIN_7
#define PWM_GPIO_Port GPIOB

#define GPIO_Pin_SDA				GPIO_PIN_4
#define GPIO_Pin_SCL				GPIO_PIN_5
#define I2C_GPIO					GPIOB
#define I2C_RCC_APB2Periph_GPIO	RCC_APB2Periph_GPIOB
#define TOSHIBA_I2C_ADDR 0x0F

//OLED
//#define OLED_128x32 true
//#define SSD1306_I2C_ADDR        0x78
#define SSD1306_I2C_ADDR        0x3C
// SSD1306 width in pixels
#define SSD1306_WIDTH           128
// SSD1306 LCD height in pixels
#define SSD1306_HEIGHT          32

//OSVR defines
#define SVR_HAVE_TOSHIBA_TC358870
#define SVR_IS_HDK_20





//USB defines
//%ATMEL_CDC_ASF_COMPOSITE_EXAMPLE2%=DriverInstall.NTamd64, USB\VID_03EB&PID_2421&MI_00
    /// HDK2 family
    #define SVR_HDK_VARIANT_STRING "20"
    #define  USB_DEVICE_VENDOR_ID             0x1532 // Razer VID
	/// @todo this should be updated: HDK2 should not have same PID as 1.x!
    #define  USB_DEVICE_PRODUCT_ID            0x0B00
    #define  USB_DEVICE_MANUFACTURE_NAME      "Sensics"

    #define  USB_DEVICE_MAJOR_VERSION         1
    #define  USB_DEVICE_MINOR_VERSION         0
    #define  USB_DEVICE_POWER                 100 // Consumption on VBUS line (mA)
 /*   #define  USB_DEVICE_ATTR                  \
    		(USB_CONFIG_ATTR_SELF_POWERED) */

    #define  USB_DEVICE_PRODUCT_NAME          "OSVR HDK 2.0"
	#define  SVR_USB_DEVICE_PRODUCT_NAME_LENGTH 12
    #define  USB_DEVICE_SERIAL_NAME           "HDK-" SVR_HDK_VARIANT_STRING "-"

    //#define  USB_DEVICE_SERIAL_NAME            
	#define  USB_DEVICE_GET_SERIAL_NAME_POINTER "OSVR HDK 2.0"
	#define  USB_DEVICE_GET_SERIAL_NAME_LENGTH  12
	//extern uint8_t serial_number[];

    #define USB_INTERFACE_STRING "OSVR_HMD_CDC"



//! Control endpoint size
#define  USB_DEVICE_EP_CTRL_SIZE       64

//! Number of interfaces for this device
////#define  USB_DEVICE_NB_INTERFACE       3 // 2 for composite, 1 for generic HID
#define  USB_DEVICE_NB_INTERFACE       2 // 2 for composite, 1 for generic HID

//! Total endpoint used by all interfaces
//! Note:
//! It is possible to define an IN and OUT endpoints with the same number on XMEGA product only
//! E.g. MSC class can be have IN endpoint 0x81 and OUT endpoint 0x01
#define  USB_DEVICE_MAX_EP             5 // 2 for generic HID, 3 for composite
//@}
//Video mode defines
//#define OSVR_90_HZ


//EDID defines
//#define OVERRIDE_EDID 
//#ifdef OVERRIDE_EDID
#define VID_EDID 0x1017
#define PID_EDID 0x0007
//#endif
#define BUTTON_PRESS_MAX_CNTR 10

//TOSHIBA regs
#define TOSHIBA_SYS_STATUS 0x8520

//GPIOs
    #define USB_ON                  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    #define USB_OFF                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    #define TOSHIBA_RESET_OFF       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
    #define TOSHIBA_RESET_ON        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
    #define OLED_RESET              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET)
    #define OLED_SET                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_RESET)
    #define MAIN_POWER_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_7 | GPIO_PIN_3, GPIO_PIN_SET)
    #define MAIN_POWER_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_7 | GPIO_PIN_3, GPIO_PIN_RESET)
    #define LED_PWM_ON              HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2)
    //#define BUTTONS_ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);
    #define BUTTON_A_STATE          HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
    #define BUTTON_B_STATE          HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)

    #define TOSHIBA_12_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
    #define TOSHIBA_12_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
    #define TOSHIBA_18_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET)
    #define TOSHIBA_18_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET)

    #define LED1_ON                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
    #define LED1_OFF                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
    #define LED2_ON                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
    #define LED2_OFF                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
    #define LED3_ON                 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
    #define LED3_OFF                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
    #define LED1_TOGGLE             HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    #define LED2_TOGGLE             HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    #define OLED_POWER_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_SET) 
    #define OLED_POWER_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_RESET);
#endif
void MainPowerOn(void);
void DiasplayPowerOn(void);
//void PowerStage2to1(void);
void PowerOffStage2(void);
void MainPowerOff(void);
void ButtonsOn(void);
void Toshiba_Init(int edid_var);
void Stage2InitSplit(void);
void Led_A_Blink(int cicle, int mode);
void InitHardware(void);
void StatusControl(void);
void Buttons(void);
void PrintScreen(void);
void DisplaySleep(void);
extern bool DisplayStatus;
extern bool OledNeedUpdate;
extern uint8_t OLED_OK;
/*
        static DirectModeVendors vendors = DirectModeVendors{
            Vendor{"OVR", "Oculus"},
            Vendor{"SEN", "Sensics", "Some Sensics professional displays"},
            Vendor{"SVR", "Sensics", "Some Sensics professional displays"},
            Vendor{"SEN", "OSVR", "Some OSVR HDK units with early firmware/EDID data"},
            Vendor{"SVR", "OSVR", "Most OSVR HDK units"},
            Vendor{"AUO", "OSVR", "Some OSVR HDK2 firmware versions"},
            Vendor{"VVR"},
            Vendor{"IWR", "Vuzix"},
            Vendor{"HVR", "HTC"},
            Vendor{"AVR"},
            Vendor{"VRG", "VRGate"},
            Vendor{"TSB", "VRGate"},
            Vendor{"VRV", "Vrvana"},
            Vendor{"TVL", "TotalVision"},
            Vendor{"FOV", "FOVE"},
            Vendor{"LZT", "HomeVR"},
        
        };

        */