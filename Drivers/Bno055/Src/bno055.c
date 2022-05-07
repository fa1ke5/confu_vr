/*
 * BNO055.c
 *
 *  Created on: 23 Oca 2018
 *      Author: bugra
 */
#include "BNO055.h"
#include "i2cSoft.h"
#include "main.h"
#include <string.h>
//#include "cmsis_os.h"
//extern I2C_HandleTypeDef hi2c2;
uint8_t bno_mode = OPERATION_MODE_NDOF;
uint8_t buffer[6];
uint8_t bufferquat[8];



uint8_t const scd[] = {
//#include "bno-hostif/SCD-Bosch-BNO070-8G-lowGyroNoise.c"
//#include "bno-hostif/SCD-Bosch-BNO070-8G.c"
#include "SCD-Bosch-BNO070-sqtsNoise.c"
};

int8_t begin( void ){
	/*
	 * IMU Begin function
	 */
	uint8_t id = I2C_read8(BNO055_ADDRESS_A, BNO055_CHIP_ID_ADDR);
	if(id != BNO055_ID){
		HAL_Delay(20); // hold on for boot
		id = I2C_read8(BNO055_ADDRESS_A, BNO055_CHIP_ID_ADDR);
		if(id != BNO055_ID) {
			return -1;  // still not? ok bail
		}
	}
	setMode(OPERATION_MODE_CONFIG);
	I2C_write8(BNO055_ADDRESS_A, BNO055_SYS_TRIGGER_ADDR, 0x20);
	while (I2C_read8(BNO055_ADDRESS_A, BNO055_CHIP_ID_ADDR) != (int8_t)BNO055_ID){
		HAL_Delay(20);
	}
	HAL_Delay(20);
	/* Set to normal power mode */
	I2C_write8(BNO055_ADDRESS_A, BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL); 
	HAL_Delay(10);
	uint8_t unit = I2C_read8(BNO055_ADDRESS_A, BNO055_UNIT_SEL_ADDR);
	HAL_Delay(10);
	I2C_write8(BNO055_ADDRESS_A, BNO055_UNIT_SEL_ADDR, unit | 0x02); 
	HAL_Delay(10);
	I2C_write8(BNO055_ADDRESS_A, BNO055_PAGE_ID_ADDR, 0);
	HAL_Delay(20);
	I2C_write8(BNO055_ADDRESS_A, BNO055_SYS_TRIGGER_ADDR, 0x0);
	HAL_Delay(20);
	/* Set the requested operating mode (see section 3.3) */
	setMode(bno_mode);
	HAL_Delay(20);
	return 0;
}

void setMode(uint8_t mode){
	/*
	 * To change operation mode
	 *
	 * OPERATION MODES:
	 *
	 * OPERATION_MODE_CONFIG
	 * OPERATION_MODE_ACCONLY
	 * OPERATION_MODE_MAGONLY
	 * OPERATION_MODE_GYRONLY
	 * OPERATION_MODE_ACCMAG
	 * OPERATION_MODE_ACCGYRO
	 * OPERATION_MODE_MAGGYRO
	 * OPERATION_MODE_AMG
	 * OPERATION_MODE_IMUPLUS
	 * OPERATION_MODE_COMPASS
	 * OPERATION_MODE_M4G
	 * OPERATION_MODE_NDOF_FMC_OFF
	 * OPERATION_MODE_NDOF
	 */

	I2C_write8(BNO055_ADDRESS_A, BNO055_OPR_MODE_ADDR, mode);
	HAL_Delay(20);
}

void setExtCrystalUse(uint8_t usextal){
	/*
	 * !!!UNAPPROVED!!!
	 */
	setMode(OPERATION_MODE_CONFIG);
	HAL_Delay(25);
	I2C_write8(BNO055_ADDRESS_A, BNO055_PAGE_ID_ADDR, 0);
	if (usextal == 1) {
		I2C_write8(BNO055_ADDRESS_A, BNO055_SYS_TRIGGER_ADDR, 0x80);
	}
	else {
		I2C_write8(BNO055_ADDRESS_A, BNO055_SYS_TRIGGER_ADDR, 0x00);
	}
	HAL_Delay(10);
	/* Set the requested operating mode (see section 3.3) */
	setMode(bno_mode);
	HAL_Delay(20);
}

int8_t getTemp(void){
	/*
	 * return uint8_t temp data
	 */
	int8_t temp = (int8_t)(I2C_read8(BNO055_ADDRESS_A, BNO055_TEMP_ADDR));
	return temp;
}



void getCalibration(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag){
	uint8_t calData = I2C_read8(BNO055_ADDRESS_A, BNO055_CALIB_STAT_ADDR);
	if (sys != 0) {
		*sys = (calData >> 6) & 0x03;
	}
	if (gyro != 0) {
		*gyro = (calData >> 4) & 0x03;
	}
	if (accel != 0) {
		*accel = (calData >> 2) & 0x03;
	}
	if (mag != 0) {
		*mag = calData & 0x03;
	}
}

void getSystemStatus(uint8_t *system_status, uint8_t *self_test_result, uint8_t *system_error){
	I2C_write8(BNO055_ADDRESS_A, BNO055_PAGE_ID_ADDR, 0);
	HAL_Delay(10);
	/* System Status (see section 4.3.58)
     ---------------------------------
     0 = Idle
     1 = System Error
     2 = Initializing Peripherals
     3 = System Iniitalization
     4 = Executing Self-Test
     5 = Sensor fusio algorithm running
     6 = System running without fusion algorithms */

	if (system_status != 0){
		*system_status    = I2C_read8(BNO055_ADDRESS_A, BNO055_SYS_STAT_ADDR);
		HAL_Delay(20);
	}

	/* Self Test Results (see section )
     --------------------------------
     1 = test passed, 0 = test failed

     Bit 0 = Accelerometer self test
     Bit 1 = Magnetometer self test
     Bit 2 = Gyroscope self test
     Bit 3 = MCU self test

     0x0F = all good! */

	if (self_test_result != 0){
		*self_test_result = I2C_read8(BNO055_ADDRESS_A, BNO055_SELFTEST_RESULT_ADDR);
		HAL_Delay(20);
	}

	/* System Error (see section 4.3.59)
     ---------------------------------
     0 = No error
     1 = Peripheral initialization error
     2 = System initialization error
     3 = Self test result failed
     4 = Register map value out of range
     5 = Register map address out of range
     6 = Register map write error
     7 = BNO low power mode not available for selected operat ion mode
     8 = Accelerometer power mode not available
     9 = Fusion algorithm configuration error
     A = Sensor configuration error */

	if (system_error != 0){
		*system_error     = I2C_read8(BNO055_ADDRESS_A, BNO055_SYS_ERR_ADDR);
	}
	HAL_Delay(200);
}

void getVector(uint8_t vector_type, float* xyz){
	/*
	 * xyz = The address of xyz data array ( double xyz[3] )
	 * vector types:
	 * VECTOR_ACCELEROMETER
	 * VECTOR_MAGNETOMETER
	 * VECTOR_GYROSCOPE
	 * VECTOR_EULER
	 * VECTOR_LINEARACCEL
	 * VECTOR_GRAVITY
	 */
	int16_t x, y, z;
	x = y = z = 0;

	/* Read vector data (6 bytes) */
	x = I2C_read16(BNO055_ADDRESS_A, vector_type);
	y = I2C_read16(BNO055_ADDRESS_A, vector_type+2);
	z = I2C_read16(BNO055_ADDRESS_A, vector_type+4);

	switch(vector_type){
    	case VECTOR_MAGNETOMETER:
    		/* 1uT = 16 LSB */
    		xyz[0] = ((float)x)/16.0;
    		xyz[1] = ((float)y)/16.0;
    		xyz[2] = ((float)z)/16.0;
    		break;
    	case VECTOR_GYROSCOPE:
    		/* 1dps = 16 LSB */
    		xyz[0] = ((float)x)/16.0;
    		xyz[1] = ((float)y)/16.0;
    		xyz[2] = ((float)z)/16.0;
    		break;
    	case VECTOR_EULER:
    		/* 1 degree = 16 LSB */
    		xyz[0] = ((float)x)/16.0;
    		xyz[1] = ((float)y)/16.0;
    		xyz[2] = ((float)z)/16.0;
    		break;
		case VECTOR_ACCELEROMETER:
			/* 1m/s^2 = 100 LSB */
			xyz[0] = ((float)x)/100.0;
			xyz[1] = ((float)y)/100.0;
			xyz[2] = ((float)z)/100.0;
			break;
		case VECTOR_LINEARACCEL:
			/* 1m/s^2 = 100 LSB */
			xyz[0] = ((float)x)/100.0;
			xyz[1] = ((float)y)/100.0;
			xyz[2] = ((float)z)/100.0;
			break;
		case VECTOR_GRAVITY:
			/* 1m/s^2 = 100 LSB */
			xyz[0] = ((float)x)/100.0;
			xyz[1] = ((float)y)/100.0;
			xyz[2] = ((float)z)/100.0;
			break;
	}
}
/*
 * UNDER CONSTRUCTION!!!
 * getRevInfo
 * getQuat
 * getSensor
 * getEvent
 * getSensorOffsets
 * getSensorOffsets
 * setSensorOffsets
 * setSensorOffsets
 * isFullyCalibrated
 */
void getQuat(uint8_t vector_type, int16_t* xyzw){
	/*
	 * wxyz = The address of wxyz data array ( double wxyz[4] )

	 */
	//HAL_I2C_Mem_Read(&hi2c2,BNO055_ADDRESS_A<<1,vector_type, 1, bufferquat, 8, 100);
	i2cSoft_Mem_Read ( BNO055_ADDRESS_A, vector_type,  bufferquat, 8 );

	xyzw[3] = (uint16_t)bufferquat[0] | (uint16_t)bufferquat[1] << 8;
	xyzw[0] = (uint16_t)bufferquat[2] | (uint16_t)bufferquat[3] << 8;
	xyzw[1] = (uint16_t)bufferquat[4] | (uint16_t)bufferquat[5] << 8;
	xyzw[2] = (uint16_t)bufferquat[6] | (uint16_t)bufferquat[7] << 8;

	 // 	char str0[100];
	  	//sprintf(str0,"%08d, %08d, %08d, %08d\r\n", (int16_t *) xyzw[0] , (int16_t *) xyzw[1] , (int16_t *)xyzw[2], (int16_t *)xyzw[3]  );
	//	sprintf(str0,"%d, %d, %d, %d\r\n",  xyzw[0] , xyzw[1] , xyzw[2], xyzw[3]  );
		//  sprintf(str0,"%d\r\n", (uint16_t *) (xyzw[0] >> 1 ));
	//  CDC_Transmit_FS((unsigned char*)str0, strlen(str0));
/*
	for(int i = 2; i < 8; i++)
	send_report[i+2] = bufferquat[i] ;

	send_report[0] = bufferquat[0] ;
	send_report[1] = bufferquat[1] ;

*/	
	i2cSoft_Mem_Read ( BNO055_ADDRESS_A, 0x14,  bufferquat, 6 );
	/*
		for(int i = 0; i < 6; i++)
	send_report[i+10] = bufferquat[i] * 56;
	*/
xyzw[4] = (uint16_t)bufferquat[0] | (uint16_t)bufferquat[1] << 8;
xyzw[5] = (uint16_t)bufferquat[2] | (uint16_t)bufferquat[3] << 8;
xyzw[6] = (uint16_t)bufferquat[4] | (uint16_t)bufferquat[5] << 8;

	  //	char str0[100];
	 // 	sprintf(str0,"%08d, %08d, %08d\r\n", (int16_t *) xyzw[4] , (int16_t *) xyzw[5] , (int16_t *)xyzw[6]);
	//	sprintf(str0,"%d, %d, %d, %d\r\n",  xyzw[0] , xyzw[1] , xyzw[2], xyzw[3]  );
		//  sprintf(str0,"%d\r\n", (uint16_t *) (xyzw[0] >> 1 ));
	//  CDC_Transmit_FS((unsigned char*)str0, strlen(str0));

	
}

void getEuler(uint8_t vector_type, float* xyz){
	/*
	 * wxyz = The address of wxyz data array ( double wxyz[4] )

	 */


	//HAL_I2C_Mem_Read(&hi2c2,BNO055_ADDRESS_A<<1,vector_type, 1, buffer, 6, 100);
	i2cSoft_Mem_Read ( BNO055_ADDRESS_A, vector_type,  buffer, 6 );

	int16_t temp[3];


	switch(vector_type){

	case VECTOR_EULER:
	    		/* 1 degree = 16 LSB */
		temp[0] = (((int16_t)buffer[0]) | (buffer[1]) << 8);
		temp[1] = (((int16_t)buffer[2]) | (buffer[3]) << 8);
		temp[2] = (((int16_t)buffer[4]) | (buffer[5]) << 8);

	    xyz[0] = ((float)(temp[0])/16.0f);
	    xyz[1] = ((float)(temp[1])/16.0f);
	    xyz[2] = ((float)(temp[2])/16.0f);
	    break;

	}
}

void getLinear(uint8_t vector_type, float* xyz){
	/*
	 * xyz = The address of xyz data array ( double xyz[4] )

	 */


//	HAL_I2C_Mem_Read(&hi2c2,BNO055_ADDRESS_A<<1,vector_type, 1, buffer, 6, 100);
    i2cSoft_Mem_Read ( BNO055_ADDRESS_A, vector_type,  buffer, 6 );
	int16_t temp[3];


	switch(vector_type){

	case VECTOR_LINEARACCEL:
		/* 1m/s^2 = 100 LSB */
		temp[0] = (((int16_t)buffer[0]) | (buffer[1]) << 8);
		temp[1] = (((int16_t)buffer[2]) | (buffer[3]) << 8);
		temp[2] = (((int16_t)buffer[4]) | (buffer[5]) << 8);

	    xyz[0] = ((float)(temp[0])/100);
	    xyz[1] = ((float)(temp[1])/100);
	    xyz[2] = ((float)(temp[2])/100);
	    break;

	}
}


int8_t I2C_read8(uint8_t address, uint8_t reg){
	uint8_t val = i2cSoft_Read(reg);
	return val;
}


void I2C_write8(uint8_t address, uint8_t reg, uint8_t data){
	static uint8_t buffer[2];
	buffer[0]=reg;
	buffer[1]=data;
	i2cSoft_WriteBuffer( address, buffer, 2);
    HAL_Delay(20);
}

int16_t I2C_read16(uint8_t address, uint8_t reg){
	int16_t val=0;
	i2cSoft_WriteByteOled ( address, reg );
	//HAL_I2C_Master_Transmit(&hi2c2, slave, &address, 1, 100 );
	//HAL_Delay(20);
	i2cSoft_ReadBuffer(address, (uint8_t *)&val, 2 );
	///HAL_I2C_Master_Receive(&hi2c2,slave, (uint8_t *)&val, 2, 100);
	return val;
}

