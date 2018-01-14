
void GLCD_screen(void)
{
    int i;
    for(i=0;i<9;i++)
    {
        GLCD_Line(8+13*i,12,8+13*i,116);
        GLCD_Line(8,12+13*i,112,12+13*i);
        GLCD_Line(127+13*i,12,127+13*i,116);
        GLCD_Line(127,12+13*i,231,12+13*i);
    }
}
void begin_cursoron(unsigned char x, unsigned char y)
{
    GLCD_Line(x+2,y+9,x+2,y+12);
    GLCD_Line(x+3,y+9,x+3,y+15);
    GLCD_Line(x+4,y+9,x+4,y+15);
    GLCD_Line(x+5,y+9,x+5,y+18);
    GLCD_Line(x+6,y+11,x+6,y+21);
    GLCD_Line(x+7,y+1,x+7,y+24);
    GLCD_Line(x+8,y,x+8,y+25);
    GLCD_Line(x+9,y,x+9,y+25);
    GLCD_Line(x+10,y+1,x+10,y+25);
    GLCD_Line(x+11,y+7,x+11,y+25);
    GLCD_Line(x+12,y+7,x+12,y+25);
    GLCD_Line(x+13,y+8,x+13,y+25);
    GLCD_Line(x+14,y+8,x+14,y+25);
    GLCD_Line(x+15,y+9,x+15,y+25);
    GLCD_Line(x+16,y+9,x+16,y+25);
    GLCD_Line(x+17,y+10,x+17,y+23);
    GLCD_Line(x+18,y+10,x+18,y+20);
    GLCD_Line(x+19,y+12,x+19,y+16);
}
void begin_cursoroff(unsigned char x, unsigned char y)
{
    color=0;
    GLCD_Line(x+2,y+9,x+2,y+12);
    GLCD_Line(x+3,y+9,x+3,y+15);
    GLCD_Line(x+4,y+9,x+4,y+15);
    GLCD_Line(x+5,y+9,x+5,y+18);
    GLCD_Line(x+6,y+11,x+6,y+21);
    GLCD_Line(x+7,y+1,x+7,y+24);
    GLCD_Line(x+8,y,x+8,y+25);
    GLCD_Line(x+9,y,x+9,y+25);
    GLCD_Line(x+10,y+1,x+10,y+25);
    GLCD_Line(x+11,y+7,x+11,y+25);
    GLCD_Line(x+12,y+7,x+12,y+25);
    GLCD_Line(x+13,y+8,x+13,y+25);
    GLCD_Line(x+14,y+8,x+14,y+25);
    GLCD_Line(x+15,y+9,x+15,y+25);
    GLCD_Line(x+16,y+9,x+16,y+25);
    GLCD_Line(x+17,y+10,x+17,y+23);
    GLCD_Line(x+18,y+10,x+18,y+20);
    GLCD_Line(x+19,y+12,x+19,y+16);
    color=1;
}
void GLCD_TextGoTo1(unsigned char x, unsigned char y)
{
unsigned int address;
    int i;
address = GLCD_TEXT_HOME +  x + (GLCD_TEXT_AREA * y);

GLCD_SetAddressPointer(address);

	for (i = 0; i < 12; i++)
	{
		GLCD_WriteDisplayData(0);
	}
}
void lGLCD_success(int i, int j)
{
    unsigned char x;
    unsigned char y;
    unsigned char color=1;
    x=14+13*i;
    y=18+13*j;
    GLCD_Circle(x,y,6);
    GLCD_Circle(x,y,5);
    GLCD_Circle(x,y,4);
}
void lGLCD_cursor(int i, int j)
{
    unsigned char x=14+13*i;
    unsigned char y=18+13*j;
    GLCD_Line(x-4,y-6,x-4,y+5);
    GLCD_Line(x-3,y-5,x-3,y+4);
    GLCD_Line(x-2,y-5,x-2,y+3);
    GLCD_Line(x-1,y-4,x-1,y+2);
    GLCD_Line(x,y-4,x,y+3);
    GLCD_Line(x+1,y-3,x+1,y+5);
    GLCD_Line(x+2,y-3,x+2,y);
    GLCD_Line(x+3,y-2,x+3,y);
    GLCD_Line(x+4,y-1,x+4,y);
    GLCD_Line(x+5,y-1,x+5,y);
    GLCD_SetPixel(x+6,y,color);
    GLCD_Line(x+2,y+2,x+2,y+6);
    GLCD_Line(x+3,y+3,x+3,y+6);
    GLCD_Line(x+4,y+4,x+4,y+5);
}
void lGLCD_mine(int i, int j)
{
    unsigned char x=14+13*i;
    unsigned char y=18+13*j;
    GLCD_Line(x-4,y-6,x+4,y-6);
    GLCD_SetPixel(x-5,y-5,color);
    GLCD_SetPixel(x+5,y-5,color);
    GLCD_Line(x-6,y-4,x-6,y);
    GLCD_Line(x+6,y-4,x+6,y);
    GLCD_SetPixel(x-3,y-4,color);
    GLCD_Line(x-2,y-4,x-2,y-2);
    GLCD_SetPixel(x-3,y-1,color);
    GLCD_SetPixel(x-3,y,color);
    GLCD_SetPixel(x-4,y-3,color);
    GLCD_SetPixel(x-4,y-2,color);
    GLCD_SetPixel(x+3,y-4,color);
    GLCD_Line(x+2,y-4,x+2,y-2);
    GLCD_SetPixel(x+3,y-1,color);
    GLCD_SetPixel(x+3,y,color);
    GLCD_SetPixel(x+4,y-3,color);
    GLCD_SetPixel(x+4,y-2,color);
    GLCD_SetPixel(x-5,y+1,color);
    GLCD_SetPixel(x+5,y+1,color);
    GLCD_Line(x-4,y+2,x-4,y+5);
    GLCD_Line(x-4,y+6,x-4,y+6);
    GLCD_Line(x+4,y+2,x+4,y+5);
    GLCD_Rectangle(x-2,y+2,5,3);

}

void lGLCD_SemiCircle(unsigned char cx, unsigned char cy, unsigned char radius)
{
	int ax, ay, xchange, ychange, radiusError;
	ax = radius;
	ay = 0;
	xchange = 1 - 2 * radius;
	ychange = 1;
	radiusError = 0;
	while (ax >= ay)
	{
		GLCD_SetPixel(cx - ax, cy - ay, color);
		GLCD_SetPixel(cx + ax, cy - ay, color);
		GLCD_SetPixel(cx - ay, cy - ax, color);
		GLCD_SetPixel(cx + ay, cy - ax, color);
		ay++;
		radiusError += ychange;
		ychange += 2;
		if (2 * radiusError + xchange > 0)
		{
			ax--;
			radiusError += xchange;
			xchange += 2;
		}
	}
}
void lGLCD_item(int i, int j)
{
    unsigned char x=14+13*i;
    unsigned char y=18+13*j;
    lGLCD_SemiCircle(x-3,y-3,3);
    lGLCD_SemiCircle(x+3,y-3,3);
    GLCD_Line(x-6,y-3,x-6,y);
    GLCD_Line(x+6,y-3,x+6,y);
    GLCD_Line(x-6,y,x,y+6);
    GLCD_Line(x+6,y,x,y+6);

}
void lGLCD_rock(int i,int j)
{
    unsigned char x=14+13*i;
    unsigned char y=18+13*j;
    GLCD_Line(x-4,y-4,x-4,y-3);
    GLCD_Line(x-2,y-4,x-2,y-3);
    GLCD_Line(x+2,y-4,x+2,y-3);
    GLCD_Line(x+4,y-4,x+4,y-3);
    GLCD_Line(x-4,y,x+4,y);
    GLCD_Line(x-3,y+1,x-3,y+3);
    GLCD_Line(x+3,y+1,x+3,y+3);
    GLCD_Line(x,y+1,x,y+2);
    GLCD_Line(x-2,y+4,x-1,y+4);
    GLCD_Line(x+1,y+3,x+2,y+4);
    GLCD_Line(x-1,y+5,x+1,y+5);
    GLCD_SetPixel(x-3,y-5,1);
    GLCD_SetPixel(x-3,y-2,1);
    GLCD_SetPixel(x+3,y-5,1);
    GLCD_SetPixel(x+3,y-2,1);
}
void lGLCD_fail(int i,int j)
{
    unsigned char x=14+13*i;
    unsigned char y=18+13*j;
    GLCD_Line(x-6,y-6,x+6,y+6);
    GLCD_Line(x+6,y-6,x-6,y+6);
    GLCD_Line(x-5,y-6,x+6,y+5);
    GLCD_Line(x-6,y-5,x+5,y+6);
    GLCD_Line(x-6,y+5,x+5,y-6);
    GLCD_Line(x-5,y+6,x+6,y-5);

}

void GLCD_cursor_off(int i,int j)
{
    unsigned char color=0;
    int x=14+13*i,y=18+13*j;
    int a,b;
    color=0;
    for(a=-5;a<7;a++)
        for(b=-5;b<7;b++)
            GLCD_SetPixel(x+a,y+b,color);
    color=1;
}
