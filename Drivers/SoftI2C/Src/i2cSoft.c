/*
 * author:	Елисей Равнюшкин
 * date:	08.04.2014	19:00:00
 * file:	i2cSoft.c
 * Софтовая реализация I2C
 */

#include "i2cSoft.h"
//#include "stm32f1xx.h"
#include <stdbool.h>
#include <stdint.h>
#include "bno055.h"

//--------------------Прототипы локальных функций-----------------------------
static void i2cSoft_Delay ();
static bool i2cSoft_Start ();
static void i2cSoft_Stop ();
static void i2cSoft_Ack ();
static void i2cSoft_NoAck ();
static bool i2cSoft_WaitAck ();
static void SCLhigh ();
//static void SCLlow ();
//static void i2cSoft_SCLH ();
static void i2cSoft_PutByte ( uint8_t data );
static uint8_t i2cSoft_GetByte ();

//---------------------Глобальные функции--------------------------------------

/**
 *	@brief	Инициализация модуля i2c, а именно портов ввода/вывода
 *	@param	void
 *	@return	void
 */
static void i2cSoft_Delay ()
{
	volatile uint16_t i = I2C_DELAY;

	while ( i ) {
		i--;
	}
}



void i2cSoft_Init (void)
{
	SCLhigh();
	SDAH;
	 i2cSoft_Delay();

}
uint8_t i2cSoft_Read(uint8_t reg)
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( BNO055_ADDRESS_A << 1 );

	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( reg );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_Start();
	i2cSoft_PutByte( (BNO055_ADDRESS_A << 1) + 1 );
		if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
//	
	
	uint8_t data = i2cSoft_GetByte();

	i2cSoft_NoAck();
	i2cSoft_Stop();
	
		return data;
}

int TC358870_i2c_Write(uint16_t reg, uint32_t val, int len)
{
	uint8_t i2c_addr = TOSHIBA_I2C_ADDR << 1;
	switch(len)
	{
		
		
		case 1:
		i2cSoft_WriteByte ( i2c_addr,  reg, val & 0xFF );
		break;

		case 2:
		i2cSoft_WriteShort ( i2c_addr,  reg, val & 0xFFFF );
		break;

		case 4:
		i2cSoft_WriteLong ( i2c_addr,  reg, val );
		break;
	}

	return 1;
}
int TC358870_I2C_Write_BigIn(uint16_t reg, uint32_t val, int len)
{
	//Convert big endian from OSVR sources to Li for stm32
	uint32_t bi_len = 0;
	uint8_t i2c_addr = TOSHIBA_I2C_ADDR << 1;
	switch(len)
	{
		case 1:
		i2cSoft_WriteByte ( i2c_addr,  reg, val & 0xFF );
		break;

		case 2:
				 bi_len = ((val >> 8)&0xff) | // move byte 1 to byte 0
				 		  ((val << 8)&0xff00); // move byte 0 to byte 1
		i2cSoft_WriteShort ( i2c_addr,  reg, bi_len & 0xFFFF );
		break;

		case 4:
				 bi_len = ((val>>24)&0xff) | // move byte 3 to byte 0
				          ((val>>8)&0xff00) | // move byte 2 to byte 1
						  ((val<<8)&0xff0000) | // move byte 1 to byte 2
						  ((val<<24)&0xff000000); // byte 0 to byte 3
		i2cSoft_WriteLong ( i2c_addr,  reg, bi_len );
		break;
	}

	return 1;
}
uint8_t TC358870_i2c_Read(uint16_t addr)
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( TOSHIBA_I2C_ADDR << 1 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr >> 8 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr & 0xFF );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_Start();
	i2cSoft_PutByte( (TOSHIBA_I2C_ADDR << 1) + 1 );
		if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
//	
	
	uint8_t data = i2cSoft_GetByte();

	i2cSoft_NoAck();
	i2cSoft_Stop();
	
		return data;
}
uint8_t TC358870_i2c_Read8(uint16_t addr, uint8_t *data)
{
	


	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( TOSHIBA_I2C_ADDR << 1 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr >> 8 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr & 0xFF );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_Start();
	i2cSoft_PutByte( (TOSHIBA_I2C_ADDR << 1) + 1 );
		if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
//	
	
	*data = i2cSoft_GetByte();

	i2cSoft_NoAck();
	i2cSoft_Stop();
	
		return I2C_RESULT_SUCCESS;
}
uint8_t TC358870_i2c_Read16(uint16_t addr, uint16_t *data )
{
	
uint16_t ret = 0;

	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( TOSHIBA_I2C_ADDR << 1 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr >> 8 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr & 0xFF );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_Start();
	i2cSoft_PutByte( (TOSHIBA_I2C_ADDR << 1) + 1 );
		if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
//	
	ret  = i2cSoft_GetByte();

	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	ret = ret | (i2cSoft_GetByte() << 8);
	data += ret;
	i2cSoft_NoAck();
	i2cSoft_Stop();

		return I2C_RESULT_SUCCESS;
}
uint8_t TC358870_i2c_Read32(uint16_t addr, uint32_t *data )
{
	
uint32_t ret = 0;

	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( TOSHIBA_I2C_ADDR << 1 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr >> 8 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr & 0xFF );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_Start();
	i2cSoft_PutByte( (TOSHIBA_I2C_ADDR << 1) + 1 );
		if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
//	
	
	ret = i2cSoft_GetByte();
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
		ret =  ret | (i2cSoft_GetByte() << 8);
		
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	 ret = ret | (i2cSoft_GetByte() << 16);
	 data += ret;
	i2cSoft_NoAck();
	i2cSoft_Stop();

		return I2C_RESULT_SUCCESS;
}
int i2cSoft_Mem_Read (uint8_t chipAddress, uint8_t memaddr,  uint8_t *buffer, uint32_t sizeOfBuffer )
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( chipAddress << 1 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( memaddr );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
		if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;
	i2cSoft_PutByte( (chipAddress << 1) + 1 );
		if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}

	while ( sizeOfBuffer != 0 ) {
		*buffer = i2cSoft_GetByte();

		buffer++;
		sizeOfBuffer--;
		if ( sizeOfBuffer == 0 ) {
			i2cSoft_NoAck();
			break;
		}
		else
			i2cSoft_Ack();
	}
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;

}
/**
 *	@brief	Чтение данных из шины в буфер buffer, размером sizeOfBuffer
 *			у слейва с адресом chipAddress.
 *	@param	uint8_t chipAddress		- адрес подчиненного
 *			uint8_t *buffer			- указатель на буфер, куда класть
 *									  прочитанные данные
 *			uint32_t sizeOfBuffer	- количество байт для чтения
 *	@return	int - результат выполнения фунции:
 *			I2C_RESULT_SUCCESS	- в случае успеха
 *			I2C_RESULT_ERROR	- в случае ошибки
 */


int i2cSoft_ReadBuffer ( uint8_t chipAddress, uint8_t *buffer, uint32_t sizeOfBuffer )
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;
chipAddress = chipAddress << 1;
    i2cSoft_PutByte( chipAddress + 1 );
    if ( !i2cSoft_WaitAck() ) {
        i2cSoft_Stop();
        return I2C_RESULT_ERROR;
    }
 
    while ( sizeOfBuffer != 0 ) {
        *buffer = i2cSoft_GetByte();
 
        buffer++;
        sizeOfBuffer--;
        if ( sizeOfBuffer == 0 ) {
            i2cSoft_NoAck();
            break;
        }
        else
            i2cSoft_Ack();
    }
    i2cSoft_Stop();
    return I2C_RESULT_SUCCESS;
}

/**
 *	@brief	Запись данных в шину из буфера buffer, размером sizeOfBuffer
 *			в слейва с адресом chipAddress.
 *	@param	uint8_t chipAddress		- адрес подчиненного
 *			uint8_t *buffer			- указатель на буфер, откуда читать
 *									  записываемые данные
 *			uint32_t sizeOfBuffer	- количество байт для записи
 *	@return	int - результат выполнения фунции:
 *			I2C_RESULT_SUCCESS	- в случае успеха
 *			I2C_RESULT_ERROR	- в случае ошибки
 */
int i2cSoft_WriteBuffer ( uint8_t chipAddress, uint8_t *buffer, uint32_t sizeOfBuffer )
{
		if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( chipAddress << 1);
	if ( !i2cSoft_WaitAck() ) {
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}

	while ( sizeOfBuffer != 0 ) {
		i2cSoft_PutByte( *buffer );
		if ( !i2cSoft_WaitAck() ) {
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}

		buffer++;
		sizeOfBuffer--;
	}
	i2cSoft_Stop();
	i2cSoft_Delay();
	return I2C_RESULT_SUCCESS;

	/*
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( chipAddress << 1 );
	if ( !i2cSoft_WaitAck() ) {
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}

	while ( sizeOfBuffer != 0 ) {
		i2cSoft_PutByte( *buffer );
		if ( !i2cSoft_WaitAck() ) {
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}

		buffer++;
		sizeOfBuffer--;
	}
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;
	*/
}

int i2cSoft_WriteByteOled ( uint8_t chipAddress, uint8_t data )
{

	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;
i2cSoft_Delay();
i2cSoft_Delay();
	i2cSoft_PutByte( chipAddress << 1);
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}

	i2cSoft_PutByte( data );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}


	
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;	
}
int i2cSoft_DeviceReady ( uint8_t chipAddress )
{
	int x = 20;
	while(x)
	{
		i2cSoft_Delay();
			if ( !i2cSoft_Start() )
				{
					i2cSoft_Stop();
					i2cSoft_Delay();
					return 0;
				}
			i2cSoft_Delay();
			i2cSoft_PutByte( chipAddress << 1 );
			if ( i2cSoft_WaitAck() ) 
				{
					i2cSoft_Stop();
					return 1;
				}
		i2cSoft_Stop();
		x--;
	}
return 0;
}
int i2cSoft_DeviceExists ( uint8_t chipAddress )
{
	
	i2cSoft_Delay();
		if ( !i2cSoft_Start() )	
		{
			i2cSoft_Stop();
			i2cSoft_Delay();
			return 0;
		}
		i2cSoft_Delay();
		i2cSoft_PutByte( chipAddress << 1 );
			if ( i2cSoft_WaitAck() ) 
			{
			i2cSoft_Stop();
			return 1;
			}	
		i2cSoft_Stop();
return 0;
}


int i2cSoft_WriteOled ( uint8_t chipAddress, uint8_t reg, uint8_t data )
{

	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;
i2cSoft_Delay();
//i2cSoft_Delay();
	i2cSoft_PutByte( chipAddress << 1 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
		i2cSoft_PutByte( reg );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}

	i2cSoft_PutByte( data );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}


	
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;	
}
int i2cSoft_WriteByte ( uint8_t chipAddress,  uint16_t addr, uint8_t data )
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( chipAddress );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr >> 8 );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}
		i2cSoft_PutByte( addr & 0xFF );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}
	i2cSoft_PutByte( data );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}


	
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;
}
int i2cSoft_WriteShort ( uint8_t chipAddress, uint16_t addr, uint16_t data )
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( chipAddress );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}
	i2cSoft_PutByte( addr >> 8 );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}
	i2cSoft_PutByte( addr & 0xFF );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}
	i2cSoft_PutByte( (uint8_t)(data >> 8) );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}
	i2cSoft_PutByte( (uint8_t )(data & 0xFF)  );
		if ( !i2cSoft_WaitAck() ) 
		{
			i2cSoft_Stop();
			return I2C_RESULT_ERROR;
		}


	
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;
}


int i2cSoft_WriteLong ( uint8_t chipAddress,  uint16_t addr, uint32_t data )
{
	if ( !i2cSoft_Start() )
		return I2C_RESULT_ERROR;

	i2cSoft_PutByte( chipAddress );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr >> 8 );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( addr & 0xFF );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( (uint8_t )(data >> 24) );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( (uint8_t )(data >> 16) );
	if ( !i2cSoft_WaitAck() ) 
	{
	i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}
	i2cSoft_PutByte( (uint8_t )(data >> 8) );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}


	i2cSoft_PutByte( (uint8_t ) data & 0xFF );
	if ( !i2cSoft_WaitAck() ) 
	{
		i2cSoft_Stop();
		return I2C_RESULT_ERROR;
	}



	
	i2cSoft_Stop();
	return I2C_RESULT_SUCCESS;
}

//---------------------------ЛОКАЛЬНЫЕ ФУНКЦИИ-------------------------------

/**
 *	@brief	Реализация простой задержки
 *	@param	void
 *	@return	void
 */


/**
 *	@brief	Отправка последовательности СТАРТ в шину
 *	@param	void
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static bool i2cSoft_Start ()
{
		/*
	while(!(SDAread))
	{
		SCLL;
		i2cSoft_Delay();
		SCLhigh();
		i2cSoft_Delay();
	}
	SDAL;
	i2cSoft_Delay();
	SCLL;
	i2cSoft_Delay();
	*/

	SDAH;						// отпустить обе линии, на случай
	SCLhigh();						// на случай, если они были прижаты


	i2cSoft_Delay();
	if ( !(SDAread) )			// если линия SDA прижата слейвом,
		return 0;			// то сформировать старт невозможно, выход с ошибкой
	SDAL;						// прижимаем SDA к земле
	i2cSoft_Delay();
	if ( SDAread )				// если не прижалась, то шина неисправна
		return 0;			// выход с ошибкой
	SCLL;	
	i2cSoft_Delay();
	return 1;				// старт успешно сформирован
}

/**
 *	@brief	Отправка последовательности СТОП в шину
 *	@param	void
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static void i2cSoft_Stop ()
{

	SCLL;						// последовательность для формирования Стопа
	i2cSoft_Delay();
	SDAL;
	i2cSoft_Delay();
	SCLhigh();
	i2cSoft_Delay();
	SDAH;
	i2cSoft_Delay();
}

/**
 *	@brief	Отправка последовательности ACK в шину
 *	@param	void
 *	@return	void
 */

static void i2cSoft_Ack ()
{

	SCLL;
	i2cSoft_Delay();
	SDAL;						// прижимаем линию SDA к земле
	i2cSoft_Delay();
	SCLhigh();						// и делаем один клик линием SCL
	i2cSoft_Delay();
	SCLL;
	i2cSoft_Delay();
}

/**
 *	@brief	Отправка последовательности NO ACK в шину
 *	@param	void
 *	@return	void
 */
static void i2cSoft_NoAck ()	//
{

	SCLL;
	i2cSoft_Delay();
	SDAH;						// отпускаем линию SDA
	i2cSoft_Delay();
	SCLhigh();						// и делаем один клик линием SCL
	i2cSoft_Delay();
	i2cSoft_Delay();
	SCLL;
//	i2cSoft_Delay();
}

/**
 *	@brief	Проверка шины на наличие ACK от слейва
 *	@param	void
 *	@return	bool - результат выполнения функции:
 *			true  - если ACK получен
 *			false - если ACK НЕ получен
 */
static bool i2cSoft_WaitAck ()
{

	SCLL;
	i2cSoft_Delay();
	SDAH;
	i2cSoft_Delay();
	SCLhigh();						// делаем половину клика линией SCL
	i2cSoft_Delay();
	i2cSoft_Delay();
	if ( SDAread ) {			// и проверяем, прижал ли слейв линию SDA
		SCLL;
		return 0;
	}
	SCLL;						// завершаем клик линией SCL
	return 1;
}

/**
 *	@brief	Отправка одного байта data в шину
 *	@param	uint8_t data - байт данных для отправки
 *	@return	void
 */
static void i2cSoft_PutByte ( uint8_t data )
{

	
	uint8_t i = 8;				// нужно отправить 8 бит данных
	while ( i-- ) {				// пока не отправили все биты
		SCLL;					// прижимаем линию SCL к земле
		i2cSoft_Delay();
		if ( data & 0x80 )		// и выставляем на линии SDA нужный уровень
			SDAH;
		else
			SDAL;
		data <<= 1;
		i2cSoft_Delay();
		SCLH;
		//SCLhigh();					// отпускаем линию SCL
		i2cSoft_Delay();		// после этого слейв сразу же прочитает значение на линии SDA
		i2cSoft_Delay();
	}
	SCLL;
}
/**
 *	@brief	Чтение одного байта data из шины
 *	@param	void
 *	@return	uint8_t - прочитанный байт
 */
static uint8_t i2cSoft_GetByte ()
{
	volatile uint8_t i = 8;		// нужно отправить 8 бит данных
	uint8_t data = 0;

	SDAH;						// отпускаем линию SDA. управлять ей будет слейв

	while ( i-- ) {				// пока не получили все биты
		data <<= 1;
		SCLL;					// делаем клик линией SCL
		i2cSoft_Delay();
		//SCLH;
		SCLhigh();
		i2cSoft_Delay();
		if ( SDAread ) {		// читаем значение на линии SDA
			data |= 0x01;
		}
	}
	SCLL;
	return data;				// возвращаем прочитанное значение
}

static void SCLhigh ()
{
	while(~SCLread)
	SCLH;
}