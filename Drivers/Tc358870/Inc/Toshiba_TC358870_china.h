#ifndef TOSHIBA_TC358870_CHINA_H_
#define TOSHIBA_TC358870_CHINA_H_
#include  "stm32f1xx_hal.h"
#include "stm32f1xx.h"

extern uint8_t target_mode;
extern uint8_t target_edid;


//void Toshiba_TC358870_SW_Reset();
void Toshiba_TC358870_Prepare_TX();
void Toshiba_TC358870_Configure_Splitter();
void Toshiba_TC358870_HDMI_Setup(void);
void Toshiba_TC358870_HDMI2MIPI(void);
void Toshiba_TC358870_HDMI_SetupChina(void);
void Toshiba_TC358870_EDID_Write(void);
void Toshiba_TC358870_OLED_SetupChina(int mode);
void Toshiba_TC358870_OLED_SleepChina(void);
void EDID_Write(int edid_ver);
void EDID_Write_OSVR(int edid_ver);
void Setup_DSITX (int offset, int currmode);
void SendDSIcfg(void);
#endif