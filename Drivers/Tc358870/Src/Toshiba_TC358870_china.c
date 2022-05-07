#include "Toshiba_TC358870_china.h"
#include "i2cSoft.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int offset = 200;
int off = 0;

//CV1 serial WMVR1033300A4X
//DK1 serial 3EC0946BFFFF //0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4E, 0xD2, 0x19, 0x10,



char edid_double[]={ 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4e, 0xd2, 0x19, 0x10, 0x00, 0x00, 0x00, 0x00, 
/*1080x1200*/		 0x0C, 0x1B, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x07, 0xF5, 0x9A, 0x56, 0x4E, 0x86, 0x26, 
					 0x1E, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 0x05, 
					 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x59, 0x6F, 0x6E, 
					 0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x30, 
					 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
					 0x00, 0x17, 0x4B, 0x0F, 0xF0, 0x22, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x15, 
					 0x02, 0x03, 0x17, 0x74, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x09, 0x7F, 0x07, 
					 0x66, 0x03, 0x0C, 0x00, 0x30, 0x00, 0x80, 0xBC, 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 
					 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 
					 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 0x39, 0x38, 0x13, 0x41, 
					 0xB0, 0x0C, 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 0x39, 0x38, 
					 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 
					 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 
					 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B };
					 
/*
char edid_double_orig[]={ 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x33, 0x54, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x0C, 0x1B, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x07, 0xF5, 0x9A, 0x56, 0x4E, 0x86, 0x26, 
					 0x1E, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 0x05, 
					 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x59, 0x6F, 0x6E, 
					 0x67, 0x58, 0x69, 0x6E, 0x67, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x30, 
					 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
					 0x00, 0x17, 0x4B, 0x0F, 0xF0, 0x22, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x15, 
					 0x02, 0x03, 0x17, 0x74, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x09, 0x7F, 0x07, 
					 0x66, 0x03, 0x0C, 0x00, 0x30, 0x00, 0x80, 0xBC, 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 
					 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 
					 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 0x39, 0x38, 0x13, 0x41, 
					 0xB0, 0x0C, 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 0x39, 0x38, 
					 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0xBC, 
					 0x39, 0x38, 0x13, 0x41, 0xB0, 0x0C, 0x40, 0x64, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 
					 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B };
*/
 char edid_SBS[256] = {	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4e, 0xd2, 0x19, 0x10, 0x00, 0x00, 0x00, 0x00, 
/*2160x1200*/ 			0x0C, 0x1B, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x07, 0xF5, 0x9A, 0x56, 0x4E, 0x86, 0x26, 
						0x1E, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 
						0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x53, 0x65, 0x6E, 
						0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4F, 
						0x53, 0x56, 0x52, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
						0x00, 0x17, 0x4B, 0x0F, 0xF0, 0x22, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x51, 
						0x02, 0x03, 0x17, 0x74, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x09, 0x7F, 0x07, 
						0x66, 0x03, 0x0C, 0x00, 0x30, 0x00, 0x80, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 
						0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 
						0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 
						0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 
						0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 
						0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 
						0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7 };

					


//char edid_SBS_old[256] ={ 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x33, 0x54, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
/*2160x1200*/     /*        0x0C, 0x1B, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x07, 0xF5, 0x9A, 0x56, 0x4E, 0x86, 0x26, 
						  0x1E, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 
						  0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x59, 0x6F, 0x6E, 
						  0x67, 0x58, 0x69, 0x6E, 0x67, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x30, 
						  0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
						  0x00, 0x17, 0x4B, 0x0F, 0xF0, 0x22, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x51, 
						  0x02, 0x03, 0x17, 0x74, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x09, 0x7F, 0x07, 
						  0x66, 0x03, 0x0C, 0x00, 0x30, 0x00, 0x80, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 
						  0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 
						  0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 
						  0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 
						  0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 
						  0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 
						  0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7 };
*/
/*
char edid_osvr[256] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4E, 0xD2, 0x19, 0x10, 0x7A, 0x01, 0x01, 0x01, 
  						0xFF, 0x1A, 0x01, 0x04, 0xA0, 0x49, 0x00, 0x78, 0x22, 0xA8, 0x7D, 0xAB, 0x54, 0x3C, 0xB6, 0x21, 
						0x10, 0x4C, 0x4F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
						0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x8E, 0x73, 0x70, 0x28, 0x82, 0xB0, 0x0C, 0x40, 0xA4, 0x20, 
						0x63, 0x40, 0x04, 0x96, 0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x53, 0x65, 0x6E, 
						0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4F, 
						0x53, 0x56, 0x52, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
						0x00, 0x58, 0x5C, 0x0F, 0xB4, 0x1E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x2D, 
						0x02, 0x03, 0x12, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xE2, 0x13, 0x81, 0xE4, 0x32, 0x00, 
						0x01, 0x02, 0x8E, 0x73, 0x70, 0x28, 0x82, 0xB0, 0x0C, 0x40, 0xA4, 0x20, 0x63, 0x40, 0x04, 0x96, 
						0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96 };
*/

/*
char edid_rift[] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x3E, 0xD2, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1B, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x07, 0xF5, 0x9A, 0x56, 0x4E, 0x86, 0x26, 
					 0x1E, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 
					 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x52, 0x69, 0x66, 0x74, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x57, 
					 0x4D, 0x48, 0x44, 0x33, 0x31, 0x34, 0x4D, 0x31, 0x30, 0x30, 0x46, 0x46, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x17, 0x4B, 0x0F, 0xF0, 0x22, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xE7, 
					 0x02, 0x03, 0x17, 0x74, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x09, 0x7F, 0x07, 0x66, 0x03, 0x0C, 0x00, 0x30, 0x00, 0x80, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 
					 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 
					 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x9C, 
					 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x5A, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7 };

char edid_htc[] =  { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x22, 0xD2, 0x01, 0xAA, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x1A, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x02, 0x60, 0xBC, 0xAA, 0x55, 0x40, 0xB6, 0x23, 
					 0x0C, 0x4C, 0x52, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 
					 0xC2, 0x04, 0x7A, 0x44, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 0x04, 0x7A, 0x44, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48, 
					 0x54, 0x43, 0x2D, 0x56, 0x49, 0x56, 0x45, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x14, 0x78, 0x01, 0xFF, 0x1D, 0x01, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xDE, 
					 0x02, 0x03, 0x15, 0x40, 0x42, 0x01, 0x02, 0x23, 0x09, 0x07, 0x01, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 
					 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 
					 0x41, 0x28, 0x14, 0xC2, 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB1 };
 
*/ /* 0x06, 0xAF, 0x11, 0x11*/
char edid_osvr[] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4E, 0xD2, 0x19, 0x10, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x01, 0x03, 0x80, 0x0D, 0x0F, 0x78, 0xEA, 0xA8, 0x7D, 0xAB, 0x54, 0x3C, 0xB6, 0x21,
					 0x10, 0x4C, 0x4F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
					 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xA4, 0x6A, 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05,
					 0x63, 0x00, 0x82, 0x96, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x53, 0x65, 0x6E,
					 0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4F,
					 0x53, 0x56, 0x52, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
					 0x00, 0x14, 0x78, 0x01, 0xFF, 0x1D, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xBC,
					 0x02, 0x03, 0x0C, 0x30, 0x41, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xA4, 0x6A, 0x70, 0x56,
					 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x82, 0x96, 0x00, 0x00, 0x00, 0x18, 0xA4, 0x6A,
					 0x70, 0x56, 0x81, 0xB0, 0x0C, 0x40, 0x5C, 0x05, 0x63, 0x00, 0x82, 0x96, 0x00, 0x00, 0x00, 0x18,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70 };

char edid_osvr1[] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4E, 0xD2, 0x19, 0x10, 0x7A, 0x01, 0x01, 0x01,
                      0xFF, 0x1A, 0x01, 0x04, 0xA0, 0x49, 0x00, 0x78, 0x22, 0xA8, 0x7D, 0xAB, 0x54, 0x3C, 0xB6, 0x21,
					  0x10, 0x4C, 0x4F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
					  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x8E, 0x73, 0x70, 0x28, 0x82, 0xB0, 0x0C, 0x40, 0xA4, 0x20,
					  0x63, 0x40, 0x04, 0x96, 0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x53, 0x65, 0x6E,
					  0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4F,
					  0x53, 0x56, 0x52, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
					  0x00, 0x58, 0x5C, 0x0F, 0xB4, 0x1E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x2D,
					  0x02, 0x03, 0x12, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xE2, 0x13, 0x81, 0xE4, 0x32, 0x00,
					  0x01, 0x02, 0x8E, 0x73, 0x70, 0x28, 0x82, 0xB0, 0x0C, 0x40, 0xA4, 0x20, 0x63, 0x40, 0x04, 0x96,
					  0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96 };


/*
char edid_osvr[] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4E, 0xD2, 0x19, 0x10, 0x7A, 0x01, 0x01, 0x01, 0xFF, 0x1A, 0x01, 0x04, 0xA0, 0x49, 0x00, 0x78, 0x22, 0xA8, 0x7D, 0xAB, 0x54, 0x3C, 0xB6, 0x21, 
                     0x10, 0x4C, 0x4F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x8E, 0x73, 0x70, 0x28, 0x82, 0xB0, 0x0C, 0x40, 0xA4, 0x20, 
					 0x63, 0x40, 0x04, 0x96, 0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x53, 0x65, 0x6E, 0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4F, 
					 0x53, 0x56, 0x52, 0x20, 0x48, 0x44, 0x4B, 0x32, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x58, 0x5C, 0x0F, 0xB4, 0x1E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x2D, 
					 0x02, 0x03, 0x12, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xE2, 0x13, 0x81, 0xE4, 0x32, 0x00, 0x01, 0x02, 0x8E, 0x73, 0x70, 0x28, 0x82, 0xB0, 0x0C, 0x40, 0xA4, 0x20, 0x63, 0x40, 0x04, 0x96, 
					 0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96 };
*/

char edid_rift[] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x3E, 0xD2, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x19, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0xE2, 0x60, 0xB1, 0xAA, 0x55, 0x40, 0xB6, 0x23,
					 0x0C, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0x73, 0x70, 0x50, 0x80, 0xB0, 0x11, 0x41, 0x08, 0x20, 
					 0x22, 0x0C, 0x77, 0x42, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x52, 0x69, 0x66, 0x74, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x57, 
					 0x4D, 0x48, 0x44, 0x33, 0x31, 0x34, 0x4D, 0x31, 0x30, 0x30, 0x46, 0x46, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x38, 0x78, 0x1E, 0x96, 0x20, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x13, 
					 0x02, 0x03, 0x0D, 0x00, 0x68, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2B };


char edid_htc[] =  { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x22, 0xD2, 0x01, 0xAA, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x1A, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x02, 0x60, 0xBC, 0xAA, 0x55, 0x40, 0xB6, 0x23, 
					 0x0C, 0x4C, 0x52, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 
					 0xC2, 0x04, 0x7A, 0x44, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 0x04, 0x7A, 0x44, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48, 
					 0x54, 0x43, 0x2D, 0x56, 0x49, 0x56, 0x45, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x14, 0x78, 0x01, 0xFF, 0x1D, 0x01, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xDE, 
					 0x02, 0x03, 0x15, 0x30, 0x42, 0x01, 0x02, 0x23, 0x09, 0x07, 0x01, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 
					 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 
					 0x41, 0x28, 0x14, 0xC2, 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x04, 0x74, 0x70, 0x6A, 0x80, 0xB0, 0x08, 0x41, 0x28, 0x14, 0xC2, 0x04, 0xC0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC1 };

  char edid_new[] =  { 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x4e, 0xd2, 0x19, 0x10, 0x7a, 0x01, 0x01, 0x01, 
  					   0xff, 0x1a, 0x01, 0x04, 0xa0, 0x49, 0x00, 0x78, 0x22, 0xa8, 0x7d, 0xab, 0x54, 0x3c, 0xb6, 0x21, 
					   0x10, 0x4c, 0x4f, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
					   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x8e, 0x73, 0x70, 0x28, 0x82, 0xb0, 0x0c, 0x40, 0xa4, 0x20,
					   0x63, 0x40, 0x04, 0x96, 0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xff, 0x00, 0x53, 0x65, 0x6e,
					   0x73, 0x69, 0x63, 0x73, 0x20, 0x48, 0x44, 0x4b, 0x32, 0x0a, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x4f,
					   0x53, 0x56, 0x52, 0x20, 0x48, 0x44, 0x4b, 0x32, 0x0a, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfd,
					   0x00, 0x58, 0x5c, 0x0f, 0xb4, 0x1e, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x2d,
					   0x02, 0x03, 0x12, 0x00, 0x65, 0x03, 0x0c, 0x00, 0x10, 0x00, 0xe2, 0x13, 0x81, 0xe4, 0x32, 0x00,
					   0x01, 0x02, 0x8e, 0x73, 0x70, 0x28, 0x82, 0xb0, 0x0c, 0x40, 0xa4, 0x20, 0x63, 0x40, 0x04, 0x96,
					   0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96 };
			 					 

/*
void Toshiba_TC358870_SW_Reset()
{
	// Software Reset

	// disable everything except auto-increment
	TC358870_i2c_Write(0x0004, 0x0400, 2);
	TC358870_i2c_Write(0x0002, 0x013F, 2);
	TC358870_i2c_Write(0x0002, 0x0000, 2);
	TC358870_i2c_Write(0x0006, 0x0800, 2);
	
	//TC358870_i2c_Write(0x0004, 0x0004, 2);  // ConfCtl0
	//TC358870_i2c_Write(0x0002, 0x3F01, 2);  // SysCtl
	//TC358870_i2c_Write(0x0002, 0x0000, 2);  // SysCtl
	//TC358870_i2c_Write(0x0006, 0x0008, 2);  // ConfCtl1
}
*/
void Toshiba_TC358870_HDMI2MIPI(void)
{
	TC358870_i2c_Write(0x85D1, 0x01, 1);
	TC358870_i2c_Write(0x8560, 0x24, 1);
	TC358870_i2c_Write(0x8563, 0x11, 1);
	TC358870_i2c_Write(0x8564, 0x0F, 1);
	TC358870_i2c_Write(0x85D1, 0x01, 1);
		// Video Color Format Setting
	TC358870_i2c_Write(0x8A02, 0x42, 1);  // VOUT_SYNC0
		// DDC active only while hotplug is being output, no delay in DDC5V rising detection.
	TC358870_i2c_Write(0x8543, 0x02, 1);  // DDC_CTL
		// DDC5v detection interlock - HPDO will be asserted when INIT_END asserted
	TC358870_i2c_Write(0x8544, 0x11, 1);  // HPD_CTL

		// HDMI Audio Setting
	TC358870_i2c_Write(0x8600, 0x00, 1);        // AUD_Auto_Mute
	TC358870_i2c_Write(0x8602, 0xF3, 1);        // Auto_CMD0
	TC358870_i2c_Write(0x8603, 0x02, 1);        // Auto_CMD1
	TC358870_i2c_Write(0x8604, 0x0C, 1);        // Auto_CMD2
	TC358870_i2c_Write(0x8606, 0x05, 1);        // BUFINIT_START
	TC358870_i2c_Write(0x8607, 0x00, 1);        // FS_MUTE
	TC358870_i2c_Write(0x8652, 0x02, 1);        // SDO_MODE1
	TC358870_i2c_Write(0x8671, 0x020C49BA, 4);  // NCO_48F0A_D
	TC358870_i2c_Write(0x8675, 0x01E1B089, 4);  // NCO_44F0A_D

//	TC358870_i2c_Write(0x8680, 0x00, 1);        // AUD_MODE
	TC358870_i2c_Write(0x854A, 0x01, 1);  // INIT_END



}
//#endif  // INCLUDED_Toshiba_TC358870_Setup_Impl_h_GUID_098B0B47_6470_4AA6_589E_4B32CCC542C8
void Toshiba_TC358870_HDMI_SetupChina(void)
{
//	#ifdef OSVR_90_HZ
	/*
TC358870_i2c_Write(0x8520, 0x9f, 2);
TC358870_i2c_Write(0x8593, 0x33, 2);
TC358870_i2c_Write(0x8405, 0x71, 2);
TC358870_i2c_Write(0x8406, 0x0e, 2);
TC358870_i2c_Write(0x858e, 0x98, 2);
TC358870_i2c_Write(0x858f, 0x0a, 2);
TC358870_i2c_Write(0x8582, 0x70, 2);
TC358870_i2c_Write(0x8583, 0x08, 2);
TC358870_i2c_Write(0x8590, 0x78, 2);
TC358870_i2c_Write(0x8591, 0x09, 2);
TC358870_i2c_Write(0x858c, 0xb0, 2);
TC358870_i2c_Write(0x858d, 0x04, 2);
*/
//#else

TC358870_i2c_Write(0x0004, 0x0400, 2);
TC358870_i2c_Write(0x0002, 0x013F, 2);
HAL_Delay(1);
TC358870_i2c_Write(0x0002, 0x0000, 2);
TC358870_i2c_Write(0x0006, 0x0800, 2);
HAL_Delay(1);

TC358870_i2c_Write(0x8410, 0x03, 1);
TC358870_i2c_Write(0x8413, 0x3F, 1);
#ifdef OSVR_90_HZ
TC358870_i2c_Write(0x8420, 0x06, 1); // EQ_BYPS
#endif
TC358870_i2c_Write(0x84F0, 0x31, 1); // APLL_CTL
TC358870_i2c_Write(0x84F4, 0x01, 1); // DDCIO_CTL

TC358870_i2c_Write(0x8540, 0xC012, 2);
TC358870_i2c_Write(0x8630, 0x00, 1);
TC358870_i2c_Write(0x8631, 0x5307, 2);
TC358870_i2c_Write(0x8670, 0x02, 1);
TC358870_i2c_Write(0x8A0C, 0xC012, 2);


#ifdef OSVR_90_HZ
TC358870_i2c_Write(0x851B, 0xFD, 1);    // MISC_INTM
#endif
TC358870_i2c_Write(0x8502, 0xFF, 1);
TC358870_i2c_Write(0x8512, 0xFE, 1);
TC358870_i2c_Write(0x8514, 0x00, 1);
TC358870_i2c_Write(0x8515, 0x00, 1);
TC358870_i2c_Write(0x8516, 0x00, 1);
//test
#ifdef OSVR_90_HZ
TC358870_i2c_Write(0x851B, 0xFD, 1);    // MISC_INTM
#endif
//
////#endif
#ifdef OSVR_90_HZ
TC358870_i2c_Write(0x0014, 0xBF0F, 2);
TC358870_i2c_Write(0x0016, 0xBF0D, 2);
//TC358870_i2c_Write(0x0016, 0xFF0F, 2);
#else
TC358870_i2c_Write(0x0014, 0xBF0F, 2);
TC358870_i2c_Write(0x0016, 0xFF0F, 2);
#endif
TC358870_i2c_Write(0x85E0, 0x01, 1);
TC358870_i2c_Write(0x85E3, 0x0001, 2);
//#endif


}
void Toshiba_TC358870_EDID_Write(void)
{
	//


}
void Toshiba_TC358870_OLED_SleepChina()
{
	TC358870_i2c_Write(0x0504, 0x0500, 2);
	TC358870_i2c_Write(0x0504, 0x1700, 2);
	HAL_Delay(100);
	TC358870_i2c_Write(0x0504, 0x0500, 2);
	TC358870_i2c_Write(0x0504, 0x1C00, 2);
	HAL_Delay(50);
	TC358870_i2c_Write(0x0504, 0x0500, 2);
	TC358870_i2c_Write(0x0504, 0x0A00, 2);
	HAL_Delay(372);
}
void Toshiba_TC358870_OLED_SetupChina(int mode)
{
TC358870_i2c_Write(0x0004, 0x0400, 2);
#ifdef OSVR_90_HZ
//TC358870_i2c_Write(0x0002, 0x013F, 2);
TC358870_i2c_Write(0x0002, 0x0012, 2);
#else
TC358870_i2c_Write(0x0002, 0x0012, 2);
#endif
HAL_Delay(1);
TC358870_i2c_Write(0x0002, 0x0000, 2);
TC358870_i2c_Write(0x0006, 0x0800, 2);

Setup_DSITX (0x0, mode);
Setup_DSITX (0x200, mode);

TC358870_i2c_Write(0x0110, 0x16000000, 4); // MODE_CONFIG - high speed mode DSI commands, hsync-, vsync-
TC358870_i2c_Write(0x0310, 0x16000000, 4); // MODE_CONFIG - high speed mode DSI commands, hsync-, vsync-
HAL_Delay(1);

SendDSIcfg();
//TC358870_i2c_Write(0x0504, 0x5120, 2); //яркость!!!

TC358870_i2c_Write(0x0504, 0x0500, 2);
TC358870_i2c_Write(0x0504, 0x1100, 2); //Sleep OUT
HAL_Delay(167);
TC358870_i2c_Write(0x0504, 0x1500, 2);
TC358870_i2c_Write(0x0504, 0x2900, 2); //Display ON
TC358870_i2c_Write(0x5000, 0x0000, 2);
#ifndef  OSVR_90_HZ
TC358870_i2c_Write(0x5008, 0xA80C, 2);
#endif

if(!mode) 
{
	#ifdef  OSVR_90_HZ
TC358870_i2c_Write(0x500C, 0xE084, 2);

#else
TC358870_i2c_Write(0x500C, 0x0080, 2);
#endif
}
else
{
TC358870_i2c_Write(0x500C, 0x0000, 2);
TC358870_i2c_Write(0x500E, 0x3704, 2);
}
//#ifdef  OSVR_90_HZ
TC358870_i2c_Write(0x5080, 0x0000, 2);
//#endif

#ifndef  OSVR_90_HZ

TC358870_i2c_Write(0x5088, 0xA80C, 2);
#endif

if(mode) 
{
	TC358870_i2c_Write(0x508C, 0x0000, 2);
	TC358870_i2c_Write(0x508E, 0x3704, 2);
}
//Enable Video TX start
TC358870_i2c_Write(0x0004, 0x370C, 2); // Enable Video TX0 and TX1
TC358870_i2c_Write(0x0006, 0x0000, 2); // Switch clock source to HDMI pixel clock
/*
#ifdef OSVR_90_HZ
TC358870_i2c_Write(0x0110, 0x16000000, 4);
TC358870_i2c_Write(0x0310, 0x16000000, 4);
TC358870_i2c_Write(0x0500, 0x0400, 2);
#else
TC358870_i2c_Write(0x0110, 0x06000000, 4);
TC358870_i2c_Write(0x0310, 0x06000000, 4);
#endif
*/
#ifdef  OSVR_90_HZ
//TC358870_i2c_Write(0x0110, 0x16000000, 4);
//TC358870_i2c_Write(0x0310, 0x16000000, 4);
TC358870_i2c_Write(0x0110, 0x06000000, 4);
TC358870_i2c_Write(0x0310, 0x06000000, 4);
//TC358870_i2c_Write(0x0500, 0x0400, 2);
#else
TC358870_i2c_Write(0x0110, 0x16000000, 4);
TC358870_i2c_Write(0x0310, 0x16000000, 4);
#endif

//Enable Video TX end

HAL_Delay(22*12);

//после чего преходим в режим монитора статуса регистров 0x0290 (80)8F, 8520 (9F)

}

void SendDSIcfg(void)
{
	TC358870_i2c_Write(0x0500, 0x0400, 2); // CMD_SEL - send dcs cmds to both tx
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xFE07, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x00EC, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x0BEC, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x16EC, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x21EC, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x2DEC, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xA9BA, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xAB06, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xBB84, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xBC1C, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xFE08, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x071A, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xFE0A, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x2A1B, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xFE0D, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x0265, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x4D41, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x4B0F, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x53FE, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xFE00, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0xC203, 2); //
	TC358870_i2c_Write(0x0504, 0x1500, 2);
	TC358870_i2c_Write(0x0504, 0x51FF, 2); //яркость!!! //
}

void Setup_DSITX (int offset, int currmode)
{
TC358870_i2c_Write(offset + 0x0108, 0x01000000, 4);  // DSI_TX_CLKEN
TC358870_i2c_Write(offset + 0x010C, 0x01000000, 4); // DSI_TX_CLKSEL
TC358870_i2c_Write(offset + 0x02A0, 0x01000000, 4); // MIPI_PLL_CONTROL

if(!currmode) 
{
	#ifdef OSVR_90_HZ
	TC358870_i2c_Write(offset + 0x02AC, 0xBD900000, 4); //MIPI_PLL_CNF 90hz_2160_1200
	//TC358870_i2c_Write(offset + 0x02AC, 0x95900000, 4);  // MIPI_PLL_CNF 60hz_2160_1200
	#else
	TC358870_i2c_Write(offset + 0x02AC, 0xB5900000, 4); // MIPI_PLL_CNF
	#endif
}
else
{
	TC358870_i2c_Write(offset + 0x02AC, 0xBC900000, 4); // MIPI_PLL_CNF SPLIT
} 
HAL_Delay(2);
TC358870_i2c_Write(offset + 0x02A0, 0x03000000, 4); // MIPI_PLL_CONTROL
TC358870_i2c_Write(offset + 0x0118, 0x14000000, 4);  // LANE_ENABLE
HAL_Delay(1);
if(!currmode) 
{
	#ifdef OSVR_90_HZ
	TC358870_i2c_Write(offset + 0x0120, 0x70170000, 4); // LINE_INIT_COUNT
	#else
	TC358870_i2c_Write(offset + 0x0120, 0x8D170000, 4);  // LINE_INIT_COUNT
	#endif
}
else 
{
	TC358870_i2c_Write(offset + 0x0120, 0x70180000, 4); // LINE_INIT_COUNT
}

TC358870_i2c_Write(offset + 0x0124, 0x00000000, 4); // HSTX_TO_COUNT
TC358870_i2c_Write(offset + 0x0128, 0x01010000, 4); // FUNC_ENABLE
TC358870_i2c_Write(offset + 0x0130, 0x00000100, 4); // DSI_TATO_COUNT
TC358870_i2c_Write(offset + 0x0134, 0x00500000, 4); // DSI_PRESP_BTA_COUNT
TC358870_i2c_Write(offset + 0x0138, 0x00000100, 4); // DSI_PRESP_LPR_COUNT
TC358870_i2c_Write(offset + 0x013C, 0x00000100, 4); // DSI_PRESP_LPW_COUNT
TC358870_i2c_Write(offset + 0x0140, 0x00000100, 4); // DSI_PRESP_HSR_COUNT
TC358870_i2c_Write(offset + 0x0144, 0x00000100, 4); // DSI_PRESP_HSW_COUNT
TC358870_i2c_Write(offset + 0x0148, 0x00100000, 4); // DSI_PR_TO_COUNT
TC358870_i2c_Write(offset + 0x014C, 0x00000100, 4); // DSI_LRX-H_TO_COUNT

//
#ifdef OSVR_90_HZ
TC358870_i2c_Write(offset + 0x0150, 0x60010000, 4); // FUNC_MODE 90hz_2160_1200
#else
TC358870_i2c_Write(offset + 0x0150, 0x61010000, 4); // FUNC_MODE CHINA SBS
#endif
//TC358870_i2c_Write(offset + 0x0150, 0x41010000, 4); // FUNC_MODE 60hz_2160_1200
TC358870_i2c_Write(offset + 0x0154, 0x01000000, 4); // DSI_RX_VC_ENABLE
TC358870_i2c_Write(offset + 0x0158, 0xC8000000, 4); // IND_TO_COUNT
TC358870_i2c_Write(offset + 0x0168, 0x2A000000, 4); // DSI_HSYNC_STOP_COUNT
if(!currmode) 
{
	#ifdef OSVR_90_HZ
	TC358870_i2c_Write(offset + 0x0170, 0xC4030000, 4); //APF_VDELAYCNT 90hz_2160_1200
	//TC358870_i2c_Write(offset + 0x0170, 0xBF040000, 4); // APF_VDELAYCNT 60hz_2160_1200
	#else
	TC358870_i2c_Write(offset + 0x0170, 0xDA030000, 4); // APF_VDELAYCNT
	#endif
} 
else TC358870_i2c_Write(offset + 0x0170, 0xEC030000, 4); // APF_VDELAYCNT

TC358870_i2c_Write(offset + 0x017C, 0x81000000, 4); // DSI_TX_MODE
if(!currmode)
{
	#ifdef OSVR_90_HZ
    TC358870_i2c_Write(offset + 0x018C, 0x01000000, 4);
	//TC358870_i2c_Write(0x0390, 0x90010000, 4);
	//TC358870_i2c_Write(offset + 0x0190, 0x22010000, 4); //DSI_HBPR 60hz_2160_1200
	TC358870_i2c_Write(offset + 0x0190, 0x90010000, 4); //DSI_HBPR 90hz_2160_1200
#else
TC358870_i2c_Write(offset + 0x018C, 0x09000000, 4); // DSI_HSYNC_WIDTH
	TC358870_i2c_Write(offset + 0x0190, 0x92010000, 4); // DSI_HBPR
#endif
}
else
{
	TC358870_i2c_Write(offset + 0x018C, 0x10000000, 4); // DSI_HSYNC_WIDTH
	TC358870_i2c_Write(offset + 0x0190, 0x1B020000, 4); // DSI_HBPR
}
TC358870_i2c_Write(offset + 0x01A4, 0x00000000, 4); // DSI_RX_STATE_INT_MASK
TC358870_i2c_Write(offset + 0x01C0, 0x15000000, 4); // DSI_LPRX_THRESH_COUNT
TC358870_i2c_Write(offset + 0x0214, 0x00000000, 4); // APP_SIDE_ERR_INT_MASK
TC358870_i2c_Write(offset + 0x021C, 0x80000000, 4); // DSI_RX_ERR_INT_MASK
TC358870_i2c_Write(offset + 0x0224, 0x00000000, 4); // DSI_LPTX_INT_MASK
TC358870_i2c_Write(offset + 0x0254, 0x06000000, 4); // LPTXTIMECNT
if(!currmode) 
{
#ifdef OSVR_90_HZ
//TC358870_i2c_Write(offset + 0x0258, 0x04022400, 4);

TC358870_i2c_Write(offset + 0x0258, 0x04022400, 4); // TCLK_HEADERCNT 90hz_2160_1200
	TC358870_i2c_Write(offset + 0x025C, 0x08000D00, 4); //TCLK_TRAILCNT 90hz_2160_1200
	//TC358870_i2c_Write(offset + 0x025C, 0x09000D00, 4); // TCLK_TRAILCNT 60hz_2160_1200
	TC358870_i2c_Write(offset + 0x0260, 0x06001400, 4); //THS_HEADERCNT 90hz_2160_1200
	//TC358870_i2c_Write(offset + 0x0260, 0x07001400, 4); // THS_HEADERCNT 60hz_2160_1200
	TC358870_i2c_Write(offset + 0x0264, 0x68420000, 4); // TWAKEUPCNT
#else
TC358870_i2c_Write(offset + 0x0258, 0x05032700, 4); // TCLK_HEADERCNT
	TC358870_i2c_Write(offset + 0x025C, 0x0A000D00, 4); // TCLK_TRAILCNT
	TC358870_i2c_Write(offset + 0x0260, 0x08001200, 4); // THS_HEADERCNT
	TC358870_i2c_Write(offset + 0x0264, 0x4B430000, 4); // TWAKEUPCNT
#endif
}
else
{ 
	TC358870_i2c_Write(offset + 0x0258, 0x05032800, 4); //TCLK_HEADERCNT
	TC358870_i2c_Write(offset + 0x025C, 0x0B000D00, 4); // TCLK_TRAILCNT
	TC358870_i2c_Write(offset + 0x0260, 0x08001300, 4); // THS_HEADERCNT
	TC358870_i2c_Write(offset + 0x0264, 0xD4450000, 4); // TWAKEUPCNT
}
#ifdef OSVR_90_HZ
TC358870_i2c_Write(offset + 0x0268, 0x0F000000, 4); // TCLK_POSTCNT 90hz_2160_1200
TC358870_i2c_Write(offset + 0x026C, 0x08000D00, 4); // THS_TRAILCNT 90hz_2160_1200
//TC358870_i2c_Write(offset + 0x026C, 0x09000D00, 4); // THS_TRAILCNT 60hz_2160_1200
#else
TC358870_i2c_Write(offset + 0x0268, 0x0E000000, 4); // TCLK_POSTCNT
TC358870_i2c_Write(offset + 0x026C, 0x09000D00, 4); // THS_TRAILCNT
#endif
TC358870_i2c_Write(offset + 0x0270, 0x20000000, 4); // HSTXVREGCNT
TC358870_i2c_Write(offset + 0x0274, 0x1F000000, 4); // HSTXVREGEN
#ifdef OSVR_90_HZ
TC358870_i2c_Write(offset + 0x0278, 0x05000600, 4); // BTA_COUNT 90hz_2160_1200
#else
TC358870_i2c_Write(offset + 0x0278, 0x08000600, 4); // BTA_COUNT
#endif
TC358870_i2c_Write(offset + 0x027C, 0x02000000, 4); // DPHY_TX ADJUST
//#ifndef OSVR_90_HZ
TC358870_i2c_Write(offset + 0x0288, 0xAA020000, 4);
//#endif
TC358870_i2c_Write(offset + 0x011C, 0x01000000, 4); // DSITX_START

}

void EDID_Write(int edid_ver)
{
/*
#define MODE_SBS 0
#define MODE_SPLIT 1

#define EDID_OSVR 0
#define EDID_CUSTOM 1
#define EDID_ORIGINAL 2
*/
	int i = 0;
	char tmp[256] = {0};

	switch(target_edid)
	{
		case 0:
			//if(target_mode == 0) memcpy( &tmp, &edid_SBS, 0x100);
			if(target_mode == 0)memcpy( &tmp, &edid_osvr , 0x100);
			else memcpy( &tmp, &edid_double, 0x100);			
			
		break;
		
		case 1:
			if(target_mode == 0) memcpy( &tmp, &edid_rift, 0x100);
			else memcpy( &tmp, &edid_double, 0x100);			
		break;

		case 2:
			if(target_mode == 0)memcpy( &tmp, &edid_htc, 0x100);
			else memcpy( &tmp, &edid_double, 0x100);
		break;
	}


tmp[127] = 0;
tmp[255] = 0;

#ifdef OVERRIDE_EDID
tmp[8] = VID_EDID & 0xFF;
tmp[9] = VID_EDID >> 8;
tmp[10] = PID_EDID & 0xFF;
tmp[11] = PID_EDID >> 8;

#endif

	while ( i < 256 )
	{

	
		if(i < 127)
				tmp[127] += tmp[i];
		if(i == 127)
				tmp[127] = 0 - tmp[127];
	
		if(i > 0x127)
				tmp[255] += tmp[i];	
		if(i == 255)
				tmp[255] = 0 - tmp[255];
				
				
	TC358870_i2c_Write(0x8C00 + i, tmp[i], 1);
		i++;
	}
	

	//TC358870_i2c_Write(0x8CFF, cs, 1); //write CS byte
	



TC358870_i2c_Write(0x850b, 0xFF, 1);
//	TC358870_i2c_Write(0x85D1, 0x01, 1);
//	TC358870_i2c_Write(0x8560, 0x24, 1);
//	TC358870_i2c_Write(0x8563, 0x11, 1);
	TC358870_i2c_Write(0x8564, 0x0F, 1);
	TC358870_i2c_Write(0x8A02, 0x42, 1);
	TC358870_i2c_Write(0x8543, 0x02, 1);
	TC358870_i2c_Write(0x8544, 0x11, 1);
	TC358870_i2c_Write(0x8600, 0x00, 1);
	TC358870_i2c_Write(0x8602, 0xF3, 1);
	TC358870_i2c_Write(0x8603, 0x02, 1);
	TC358870_i2c_Write(0x8604, 0x0C, 1);
	TC358870_i2c_Write(0x8606, 0x05, 1);
	TC358870_i2c_Write(0x8607, 0x00, 1);
	TC358870_i2c_Write(0x8652, 0x02, 1);
	TC358870_i2c_Write(0x8671, 0xBA490C02, 4);
	TC358870_i2c_Write(0x8675, 0x89B0E101, 4);
	TC358870_i2c_Write(0x854A, 0x01, 1);
}
