
#include "ssd1306.h"
#include "hardware.h"
#include "i2cSoft.h"
//#include "cmsis_os.h"

// Screenbuffer
//static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static uint8_t SSD1306_Buffer_all[SSD1306_WIDTH * SSD1306_HEIGHT / 8 + 1], *SSD1306_Buffer = SSD1306_Buffer_all + 1;

// Screen object
static SSD1306_t SSD1306;
/* Write command for soft I2C*/
#define SSD1306_WRITECOMMAND(command)      i2cSoft_WriteOled(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data for soft I2C*/
#define SSD1306_WRITEDATA(data)            i2cSoft_WriteOled(SSD1306_I2C_ADDR, 0x40, (data))

//
//  Send a byte to the command register
//
/*
static void ssd1306_WriteCommand(uint8_t command) //HAL i2c
{
	HAL_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR,0x00,1,&command,1,10);
}
*/


//
//	Initialize the oled screen
//
uint8_t ssd1306_Init(void)
{	

	if (!i2cSoft_DeviceReady(SSD1306_I2C_ADDR) ) {
		/* Return false */
	//	elog("SSD1306 I2C device not found");
		return 0;
	}
	// Wait for the screen to boot
	HAL_Delay(100);
	
	/* Init LCD */

SSD1306_WRITECOMMAND(0xAE); //display off
	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); //---set low column address
	SSD1306_WRITECOMMAND(0x10); //---set high column address
	SSD1306_WRITECOMMAND(0x40); //--set start line address
	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
	if(SSD1306_HEIGHT == 32)
	SSD1306_WRITECOMMAND(0x8F);
	else
	SSD1306_WRITECOMMAND(0xFF);
	
	
	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); //--set normal display
	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	if(SSD1306_HEIGHT == 32)
	SSD1306_WRITECOMMAND(0x1F);
	else
	SSD1306_WRITECOMMAND(0x3F);
		
	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); //-set display offset
	SSD1306_WRITECOMMAND(0x00); //-not offset
	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); //
	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	if(SSD1306_HEIGHT == 32)
	SSD1306_WRITECOMMAND(0x02);
	else
	SSD1306_WRITECOMMAND(0x12);
		
	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); //
	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
	
	// Clear screen
	ssd1306_Fill(Black);
	
	// Flush buffer to screen
	ssd1306_UpdateScreen();
	
	// Set default values for screen object
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	SSD1306.Initialized = 1;
	
	return 1;
}

//
//  Fill the whole screen with the given color
//
void ssd1306_Fill(SSD1306_COLOR color) 
{
	/* Set memory */
	uint32_t i;

	for(i = 0; i < sizeof(SSD1306_Buffer); i++)
	{
		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}

//
//  Write the screenbuffer with changed to the screen
//
void ssd1306_UpdateScreen(void) 
{
	
	SSD1306_Buffer_all[0] = 0x40;

	i2cSoft_WriteBuffer( SSD1306_I2C_ADDR, SSD1306_Buffer_all, SSD1306_WIDTH * SSD1306_HEIGHT / 8 + 1 );
	
	HAL_Delay(5);

}

//
//	Draw one pixel in the screenbuffer
//	X => X Coordinate
//	Y => Y Coordinate
//	color => Pixel color
//
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		// Don't write outside the buffer
		return;
	}
	
	// Check if pixel should be inverted
	if (SSD1306.Inverted) 
	{
		color = (SSD1306_COLOR)!color;
	}
	
	// Draw in the right color
	if (color == White)
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} 
	else 
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

//
//  Draw 1 char to the screen buffer
//	ch 		=> char om weg te schrijven
//	Font 	=> Font waarmee we gaan schrijven
//	color 	=> Black or White
//
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
	uint32_t i, b, j;
	
	// Check remaining space on current line
	if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}
	
	// Use the font to write
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000) 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
			} 
			else 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
			}
		}
	}
	
	// The current space is now taken
	SSD1306.CurrentX += Font.FontWidth;
	
	// Return written char for validation
	return ch;
}

//
//  Write full string to screenbuffer
//
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
	// Write until null-byte
	while (*str) 
	{
		if (ssd1306_WriteChar(*str, Font, color) != *str)
		{
			// Char could not be written
			return *str;
		}
		
		// Next char
		str++;
	}
	
	// Everything ok
	return *str;
}

//
//	Position the cursor
//
void ssd1306_SetCursor(uint8_t x, uint8_t y) 
{
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

//addon functions

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= SSD1306_WIDTH) {
		x0 = SSD1306_WIDTH - 1;
	}
	if (x1 >= SSD1306_WIDTH) {
		x1 = SSD1306_WIDTH - 1;
	}
	if (y0 >= SSD1306_HEIGHT) {
		y0 = SSD1306_HEIGHT - 1;
	}
	if (y1 >= SSD1306_HEIGHT) {
		y1 = SSD1306_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			ssd1306_DrawPixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			ssd1306_DrawPixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		ssd1306_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t c) {
	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
	SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
	SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void ssd1306_image(uint8_t *img, uint8_t frame, uint8_t x, uint8_t y)
{
	uint32_t i, b, j;
	
	b = 0;
	if(frame >= img[2])
		return;
	uint32_t start = (frame * (img[3] + (img[4] << 8)));
	
	/* Go through font */
	for (i = 0; i < img[1]; i++) {
		for (j = 0; j < img[0]; j++) {

			ssd1306_DrawPixel(x + j, (y + i), (uint8_t) (img[b/8 + 5 + start] >> (b%8)) & 1);
			b++;
		}
	}
}