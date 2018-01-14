#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define F_CPU 16000000UL

// data port
#define GLCD_DATA_PORT 	PORTA
#define GLCD_DATA_PIN	PINA
#define GLCD_DATA_DDR	DDRA
// control port
#define GLCD_CTRL_PORT	PORTC
#define GLCD_CTRL_PIN	PINC
#define GLCD_CTRL_DDR	DDRC
// control signals
#define GLCD_WR			PC0
#define GLCD_RD			PC1
#define GLCD_CE			PC2
#define GLCD_CD			PC3
#define GLCD_RESET		PC4
#define GLCD_FS			PC5
// display properties
#define GLCD_NUMBER_OF_LINES				128
#define GLCD_PIXELS_PER_LINE				240
#define GLCD_FONT_WIDTH						8
//
#define GLCD_GRAPHIC_AREA					(GLCD_PIXELS_PER_LINE / GLCD_FONT_WIDTH)
#define GLCD_TEXT_AREA						(GLCD_PIXELS_PER_LINE / GLCD_FONT_WIDTH)
#define GLCD_GRAPHIC_SIZE					(GLCD_GRAPHIC_AREA * GLCD_NUMBER_OF_LINES)
#define GLCD_TEXT_SIZE						(GLCD_TEXT_AREA * (GLCD_NUMBER_OF_LINES/8))


#define GLCD_TEXT_HOME						0
#define GLCD_GRAPHIC_HOME					(GLCD_TEXT_HOME + GLCD_TEXT_SIZE)
#define GLCD_OFFSET_REGISTER				2
#define GLCD_EXTERNAL_CG_HOME				(GLCD_OFFSET_REGISTER << 11)


#define T6963_SET_CURSOR_POINTER			0x21
#define T6963_SET_OFFSET_REGISTER			0x22
#define T6963_SET_ADDRESS_POINTER			0x24

#define T6963_SET_TEXT_HOME_ADDRESS			0x40
#define T6963_SET_TEXT_AREA					0x41
#define T6963_SET_GRAPHIC_HOME_ADDRESS		0x42
#define T6963_SET_GRAPHIC_AREA				0x43

#define T6963_MODE_SET						0x80


#define T6963_DISPLAY_MODE					0x90
#define T6963_CURSOR_BLINK_ON		    	0x01
#define T6963_CURSOR_DISPLAY_ON	    		0x02
#define T6963_TEXT_DISPLAY_ON		    	0x04
#define T6963_GRAPHIC_DISPLAY_ON	    	0x08


#define T6963_CURSOR_PATTERN_SELECT			0xA0
#define T6963_CURSOR_1_LINE			    	0x00
#define T6963_CURSOR_2_LINE			    	0x01
#define T6963_CURSOR_3_LINE			    	0x02
#define T6963_CURSOR_4_LINE			    	0x03
#define T6963_CURSOR_5_LINE			    	0x04
#define T6963_CURSOR_6_LINE			    	0x05
#define T6963_CURSOR_7_LINE		    		0x06
#define T6963_CURSOR_8_LINE				    0x07

#define T6963_SET_DATA_AUTO_WRITE			0xB0
#define T6963_SET_DATA_AUTO_READ			0xB1
#define T6963_AUTO_RESET					0xB2

#define T6963_DATA_WRITE_AND_INCREMENT		0xC0
#define T6963_DATA_READ_AND_INCREMENT		0xC1
#define T6963_DATA_WRITE_AND_DECREMENT		0xC2
#define T6963_DATA_READ_AND_DECREMENT		0xC3
#define T6963_DATA_WRITE_AND_NONVARIALBE	0xC4
#define T6963_DATA_READ_AND_NONVARIABLE		0xC5

#define T6963_SCREEN_PEEK					0xE0
#define T6963_SCREEN_COPY					0xE8


void GLCD_InitializeInterface(void);
unsigned char GLCD_CheckStatus(void);
void GLCD_WriteCommand(unsigned char);
void GLCD_WriteData(unsigned char);
unsigned char GLCD_ReadData(void);
void GLCD_ClearText(void);
void GLCD_ClearCG(void);
void GLCD_ClearGraphic(void);
void GLCD_WriteChar(char ch);
void GLCD_WriteString(char * str);
void GLCD_TextGoTo(unsigned char, unsigned char);
void GLCD_DefineCharacter(unsigned char, unsigned char *);
void GLCD_Initialize(void);
void GLCD_SetPixel(unsigned char, unsigned char, unsigned char);
void GLCD_WriteDisplayData(unsigned char);

void delay_ms(unsigned int ms);
void GLCD_Clear(void);

//-------------------------------------------------------------------------------------------------
//
// Delay function
//
//-------------------------------------------------------------------------------------------------
void delay(void)
{
	volatile unsigned char i;
	for (i = 0; i < (F_CPU / 1000000); i++)
	{
		asm("nop");
	}
}
//-------------------------------------------------------------------------------------------------
//
// Ports intalization
//
//-------------------------------------------------------------------------------------------------
void GLCD_InitializeInterface(void)
{
	GLCD_DATA_DDR = 0xFF;
	GLCD_CTRL_DDR = ((1 << GLCD_WR) | (1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD) | (1 << GLCD_RESET) | (1 << GLCD_FS));
	GLCD_CTRL_PORT |= ((1 << GLCD_WR) | (1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD) | (1 << GLCD_RESET) | (1 << GLCD_FS));
}
//-------------------------------------------------------------------------------------------------
//
// Reads dispay status
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_CheckStatus(void)
{
	uint8_t tmp;
	GLCD_DATA_DDR = 0x00;

	GLCD_CTRL_PORT &= ~((1 << GLCD_RD) | (1 << GLCD_CE));
	delay();
	tmp = GLCD_DATA_PIN;
	GLCD_DATA_DDR = 0xFF;
	GLCD_CTRL_PORT |= ((1 << GLCD_RD) | (1 << GLCD_CE));
	return tmp;
}
//-------------------------------------------------------------------------------------------------
//
// Writes instruction
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char command)
{
	while (!(GLCD_CheckStatus() & 0x03));
	GLCD_DATA_PORT = command;

	GLCD_CTRL_PORT &= ~((1 << GLCD_WR) | (1 << GLCD_CE));
	delay();
	GLCD_CTRL_PORT |= ((1 << GLCD_WR) | (1 << GLCD_CE));
}

//-------------------------------------------------------------------------------------------------
//
// Writes data
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char data)
{
	while (!(GLCD_CheckStatus() & 0x03));
	GLCD_DATA_PORT = data;

	GLCD_CTRL_PORT &= ~((1 << GLCD_WR) | (1 << GLCD_CE) | (1 << GLCD_CD));
	delay();
	GLCD_CTRL_PORT |= ((1 << GLCD_WR) | (1 << GLCD_CE) | (1 << GLCD_CD));
}
//-------------------------------------------------------------------------------------------------
//
// Reads data
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadData(void)
{
	uint8_t tmp;
	while (!(GLCD_CheckStatus() & 0x03));
	GLCD_DATA_DDR = 0x00;

	GLCD_CTRL_PORT &= ~((1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD));
	delay();
	tmp = GLCD_DATA_PIN;
	GLCD_CTRL_PORT |= ((1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD));
	GLCD_DATA_DDR = 0xFF;
	return tmp;
}
//-------------------------------------------------------------------------------------------------
//
// Sets address pointer for display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetAddressPointer(unsigned int address)
{
	GLCD_WriteData(address & 0xFF);
	GLCD_WriteData(address >> 8);
	GLCD_WriteCommand(T6963_SET_ADDRESS_POINTER);
}
//-------------------------------------------------------------------------------------------------
//
// Clears text area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearText(void)
{
	int i;
	GLCD_SetAddressPointer(GLCD_TEXT_HOME);

	for (i = 0; i < GLCD_TEXT_SIZE; i++)
	{
		GLCD_WriteDisplayData(0);
	}
}
//-------------------------------------------------------------------------------------------------
//
// Clears characters generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearCG(void)
{
	unsigned int i;
	GLCD_SetAddressPointer(GLCD_EXTERNAL_CG_HOME);

	for (i = 0; i < 256 * 8; i++)
	{
		GLCD_WriteDisplayData(0);
	}
}
//-------------------------------------------------------------------------------------------------
//
// Clears graphics area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearGraphic(void)
{
	int i;
	GLCD_SetAddressPointer(GLCD_GRAPHIC_HOME);
	for (i = 0; i < GLCD_GRAPHIC_SIZE; i++)
	{
		GLCD_WriteDisplayData(0x00);
	}
}
//-------------------------------------------------------------------------------------------------
//
// Writes a single character (ASCII code) to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(char charCode)
{
	GLCD_WriteDisplayData(charCode - 32);
}
//-------------------------------------------------------------------------------------------------
//
// Writes null-terminated string to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(char * string)
{
	while (*string)
	{
		GLCD_WriteChar(*string++);
	}
}

//-------------------------------------------------------------------------------------------------
//
// Sets display coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(unsigned char x, unsigned char y)
{
	unsigned int address;

	address = GLCD_TEXT_HOME + x + (GLCD_TEXT_AREA * y);

	GLCD_SetAddressPointer(address);
}
//-------------------------------------------------------------------------------------------------
//
// Writes single char pattern to character generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_DefineCharacter(unsigned char charCode, unsigned char * defChar)
{
	unsigned int address;
	unsigned char i;

	address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

	GLCD_SetAddressPointer(address);

	for (i = 0; i < 8; i++)
	{
		GLCD_WriteDisplayData(*(defChar + i));
	}
}

//-------------------------------------------------------------------------------------------------
//
// Set (if color==1) or clear (if color==0) pixel on screen
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
	unsigned char tmp;
	unsigned int address;

	address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

	GLCD_SetAddressPointer(address);

	GLCD_WriteCommand(T6963_DATA_READ_AND_NONVARIABLE);
	tmp = GLCD_ReadData();

	if (color==1)
		tmp |= (1 << (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
	else
		tmp &= ~(1 << (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));

	GLCD_WriteDisplayData(tmp);

}
//-------------------------------------------------------------------------------------------------
//
// Writes display data and increment address pointer
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteDisplayData(unsigned char x)
{
	GLCD_WriteData(x);
	GLCD_WriteCommand(T6963_DATA_WRITE_AND_INCREMENT);
}
//-------------------------------------------------------------------------------------------------
//
// Sets graphics coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(unsigned char x, unsigned char y)
{
	unsigned int address;
	address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
	GLCD_SetAddressPointer(address);
}

//-------------------------------------------------------------------------------------------------
//
// Display initalization
//
//-------------------------------------------------------------------------------------------------
void GLCD_Initialize(void)
{
	GLCD_InitializeInterface();

	GLCD_CTRL_PORT &= ~(1 << GLCD_RESET);
	delay_ms(1);
	GLCD_CTRL_PORT |= (1 << GLCD_RESET);

#if (GLCD_FONT_WIDTH == 8)
	GLCD_CTRL_PORT &= ~(1 << GLCD_FS);
#endif

	GLCD_WriteData(GLCD_GRAPHIC_HOME & 0xFF);
	GLCD_WriteData(GLCD_GRAPHIC_HOME >> 8);
	GLCD_WriteCommand(T6963_SET_GRAPHIC_HOME_ADDRESS);

	GLCD_WriteData(GLCD_GRAPHIC_AREA);
	GLCD_WriteData(0x00);
	GLCD_WriteCommand(T6963_SET_GRAPHIC_AREA);

	GLCD_WriteData(GLCD_TEXT_HOME);
	GLCD_WriteData(GLCD_TEXT_HOME >> 8);
	GLCD_WriteCommand(T6963_SET_TEXT_HOME_ADDRESS);

	GLCD_WriteData(GLCD_TEXT_AREA);
	GLCD_WriteData(0x00);
	GLCD_WriteCommand(T6963_SET_TEXT_AREA);

	GLCD_WriteData(GLCD_OFFSET_REGISTER);
	GLCD_WriteData(0x00);
	GLCD_WriteCommand(T6963_SET_OFFSET_REGISTER);

	GLCD_WriteCommand(T6963_DISPLAY_MODE | T6963_GRAPHIC_DISPLAY_ON | T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);

	GLCD_WriteCommand(T6963_MODE_SET | 0);

}

void delay_ms(unsigned int ms)
{
	int i, j;

	for (i = 0; i<ms; i++)
		for (j = 0; j<2100; j++);
}

void GLCD_Clear(void)
{
	GLCD_ClearCG();
	GLCD_ClearGraphic();
	GLCD_ClearText();
}
