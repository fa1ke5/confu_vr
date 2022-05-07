/*
 * author:	Елисей Равнюшкин
 * date:	08.04.2014	19:00:00
 * file:	i2cSoft.h
 * Софтовая реализация I2C
 */
#ifndef I2CSOFT_H_
#define I2CSOFT_H_
#include <stdint.h>
#include "stm32f1xx.h"
#include "hardware.h"
//I2C



#define SCLH						I2C_GPIO->BSRR = GPIO_Pin_SCL
#define SCLL						I2C_GPIO->BRR = GPIO_Pin_SCL

#define SDAH						I2C_GPIO->BSRR = GPIO_Pin_SDA
#define SDAL						I2C_GPIO->BRR = GPIO_Pin_SDA
#define SDAread					I2C_GPIO->IDR & GPIO_Pin_SDA
#define SCLread					I2C_GPIO->IDR & GPIO_Pin_SCL

#define I2C_DELAY 1

#define I2C_RESULT_SUCCESS			0
#define I2C_RESULT_ERROR			(-1)



void i2cSoft_Init (void);
int i2cSoft_DeviceReady ( uint8_t chipAddress );
int i2cSoft_ReadBuffer (uint8_t chipAddress, uint8_t *buffer, uint32_t sizeOfBuffer);
int i2cSoft_WriteBuffer (uint8_t chipAddress, uint8_t *buffer, uint32_t sizeOfBuffer);
int i2cSoft_WriteOled ( uint8_t chipAddress, uint8_t reg, uint8_t data );
int i2cSoft_WriteByteOled  ( uint8_t chipAddress,  uint8_t data );
int i2cSoft_WriteByte  ( uint8_t chipAddress,  uint16_t addr, uint8_t data );
int i2cSoft_WriteShort ( uint8_t chipAddress, uint16_t addr, uint16_t data );
int i2cSoft_WriteLong ( uint8_t chipAddress,  uint16_t addr, uint32_t data );
int i2cSoft_DeviceExists ( uint8_t chipAddress );
int TC358870_i2c_Write(uint16_t reg, uint32_t val, int len);
uint8_t i2cSoft_Read(uint8_t reg);
int TC358870_I2C_Write_BigIn(uint16_t reg, uint32_t val, int len);
int i2cSoft_Mem_Read (uint8_t chipAddress, uint8_t memaddr,  uint8_t *buffer, uint32_t sizeOfBuffer );

uint8_t TC358870_i2c_Read8(uint16_t addr, uint8_t *data);
uint8_t TC358870_i2c_Read16(uint16_t addr, uint16_t *data );
uint8_t TC358870_i2c_Read32(uint16_t addr, uint32_t *data );

//uint16_t TC358870_i2c_Read16(uint16_t addr);
uint8_t TC358870_i2c_Read(uint16_t addr);

#endif
