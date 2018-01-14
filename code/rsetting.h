int i1[5]={0,0,1,0}, j1[5]={-1,1,0,0};
int i2[5]={0,-1,1,2,0}, j2[5]={-1,0,0,0,0};
int i3[5]={1,0}, j3[2]={0,0};
int i4[5]={0,0,1,1}, j4[5]={0,-1,-1,0}, tmp=0;
void lGLCD_SemiCircle(unsigned char cx, unsigned char cy, unsigned char radius);
void rGLCD_1block(int i, int j)
{
    int a, b;
    int x=133+13*i;
    int y=19+13*j;

    for(a=-4;a<6;a++)
        for(b=-5;b<5;b++)
            GLCD_SetPixel(x+a,y+b,color);
}

void rship1(int i,int j)
{
    int n;
    for(n=0;n<4;n++)
        rGLCD_1block(i+i1[n],j+j1[n]);
}

void rship2(int i,int j)
{
    int n;
    for(n=0;n<5;n++)
        rGLCD_1block(i+i2[n],j+j2[n]);
}

void rship3(int i,int j)
{
    int n;
    for(n=0;n<2;n++)
        rGLCD_1block(i+i3[n],j+j3[n]);
}

void switch_turn(char *ship)
{
    int n;

   if (ship == "rship1") {
      for (n = 0; n < 4; n++) {
         tmp = i1[n];
         i1[n] = j1[n];
         j1[n] = tmp;
         if (i1[n] < 0 && j1[n] > 0 || i1[n]<0 && j1[n]<0) i1[n] *= -1;
         else j1[n] *= -1;
      }
   }
   else if (ship == "rship2") {
      for (n = 0; n < 5; n++) {
         tmp = i2[n];
         i2[n] = j2[n];
         j2[n] = tmp;
         if (i2[n] < 0 && j2[n] > 0 || i2[n]<0 && j2[n]<0) i2[n] *= -1;
         else j2[n] *= -1;
      }
   }
   else{
        for(n=0;n<2;n++){
            tmp = i3[n];
            i3[n] = j3[n];
            j3[n] = tmp;
            if(i3[n] < 0 && j3[n]>0 || i3[n]<0 && j3[n]<0) i3[n] *= -1;
            else j3[n] *=-1;
        }
}
}

void rship4(int i,int j)
{
    int n;
    for(n=0;n<4;n++)
        rGLCD_1block(i+i4[n],j+j4[n]);
}

void rGLCD_cursor_off(int i,int j)
{
    int a,b;
    int x=133+13*i;
    int y=19+13*j;

    color=0;
    for(a=-5;a<7;a++)
        for(b=-6;b<6;b++)
            GLCD_SetPixel(x+a,y+b,color);
    color=1;
}
static int do_rand(unsigned long *ctx)
{
    long hi, lo, x;

    x = *ctx;
    if (x == 0)
        x = 123459876L;
    hi = x / 127773L;
    lo = x % 127773L;
    x = 16807L * lo - 2836L * hi;
    if (x < 0)
        x += 0x7fffffffL;
    return ((*ctx = x) % ((unsigned long)8 + 1));
}
static unsigned long next = 1;

int rand(void)
{
    return do_rand(&next);
}
void srand(unsigned int seed)
{
    next = seed;
}


void rGLCD_mine(int i,int j)
{
    unsigned char x=133+13*i;
    unsigned char y=19+13*j;
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

void rGLCD_item(int i,int j)//
{
    unsigned char x=133+13*i;
    unsigned char y=19+13*j;
    lGLCD_SemiCircle(x-3,y-3,3);
    lGLCD_SemiCircle(x+3,y-3,3);
    GLCD_Line(x-6,y-3,x-6,y);
    GLCD_Line(x+6,y-3,x+6,y);
    GLCD_Line(x-6,y,x,y+6);
    GLCD_Line(x+6,y,x,y+6);
}
void rGLCD_fail(int i,int j)
{
    unsigned char x=133+13*i;
    unsigned char y=19+13*j;
    GLCD_Line(x-6,y-6,x+6,y+6);
    GLCD_Line(x+6,y-6,x-6,y+6);
    GLCD_Line(x-5,y-6,x+6,y+5);
    GLCD_Line(x-6,y-5,x+5,y+6);
    GLCD_Line(x-6,y+5,x+5,y-6);
    GLCD_Line(x-5,y+6,x+6,y-5);
}

void rGLCD_rock(int i, int j)
{
    unsigned char x=133+13*i;
    unsigned char y=19+13*j;
    GLCD_Line(x-2,y-5,x-5,y-2);
    GLCD_Line(x-2,y-4,x-5,y-1);
    GLCD_SetPixel(x-3,y-5,color);
    GLCD_Line(x-5,y-1,x-2,y+2);
    GLCD_Line(x-5,y-2,x-2,y+1);
    GLCD_Line(x-2,y+1,x-5,y+4);
    GLCD_Line(x-2,y+2,x-5,y+5);
    GLCD_SetPixel(x-4,y+5,color);
    GLCD_SetPixel(x+1,y-5,color);
    GLCD_Line(x+2,y-5,x-1,y+2);
    GLCD_Line(x+2,y-4,x-1,y+1);
    GLCD_Line(x-1,y+2,x+2,y+1);
    GLCD_Line(x-1,y+1,x+2,y+2);
    GLCD_Line(x+2,y+1,x-1,y+4);
    GLCD_Line(x+2,y+2,x-1,y+5);
    GLCD_SetPixel(x,y+5,color);
    GLCD_SetPixel(x+5,y-5,color);
    GLCD_Line(x+6,y-5,x+3,y-2);
    GLCD_Line(x+6,y-4,x+3,y-1);
    GLCD_Line(x+3,y-2,x+6,y+1);
    GLCD_Line(x+3,y-1,x+6,y+2);
    GLCD_Line(x+6,y+1,x+3,y+4);
    GLCD_Line(x+6,y+2,x+3,y+5);
    GLCD_SetPixel(x+4,y+5,color);
}
