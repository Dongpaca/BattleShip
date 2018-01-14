#include <iom128v.h>
#include <stdlib.h>
#include "graphic.h"
#include "t6963c.h"
#include "rsetting.h"
#include "lsetting.h"

void is_found_rock(void);

int i=0, j=1;
int game_set[8][8];
int counter_set[8][8]={0};
int counter_detect[8][8]={0};
int my_detect[8][8]={0};
char flag=0;
unsigned char sleep=0;
int destroy[4]={-1,-1,-1,-1};
char ship_cnt=0, item_cnt=0, mine_cnt=0;
unsigned char outdata;
unsigned int timercounter=0;
unsigned char timer_sec=16;
unsigned char time_flag=1;
unsigned char mode=0;
void timeover_select(void);
unsigned char difficult=7; //난이도-높을수록 쉬움
unsigned char select=0;
int imax, imin, jmax, jmin;
int ship1x, ship1y, ship2x, ship2y, ship3x, ship3y, ship4x, ship4y, item1x, item1y,
    item2x, item2y, mine1x, mine1y, mine2x, mine2y, rock1x, rock1y, rock2x, rock2y;
int cship1x[4],cship1y[4],cship2x[5],cship2y[5],cship3x[2],cship3y[2],cship4x[4],cship4y[4],
    citemx[2],citemy[2],cminex[2],cminey[2],crockx[2],crocky[2];
signed char n, state=0;
char cnt=0;
int p_i=0, p_j=0;
char error=0;
int val_x, val_y;
int rand_i,rand_j,rand_k,rand_l;
char rest_ship;
int AtoD_cnt=0;
int rate;
int com_rest_ship=15;
int cnt_x,cnt_y;

void rGLCD_success(int i, int j)
{
    unsigned char x;
    unsigned char y;
    unsigned char color=1;
    x=133+13*i;
    y=19+13*j;
    GLCD_Circle(x,y,6);
    GLCD_Circle(x,y,5);
    GLCD_Circle(x,y,4);
}

//nothing ship item mine rock foundrock
#pragma interrupt_handler po_down_on:iv_EXT_INT1
void po_down_on(void)
{
    if(sleep==0){
        MCUCR=0x30;
        delay_ms(500);
        SREG=0x80;
        sleep++;
        asm("SLEEP");
    }
    else{
        MCUCR=0x00;
        delay_ms(500);
        sleep--;
    }
}

#pragma interrupt_handler timer2_ovf:iv_TIMER2_OVF
void timer2_ovf(void)
{
    timercounter++;
    if(timercounter==125)
    {
        timer_sec--;
        if(timer_sec>=10){
            GLCD_TextGoTo(14,0);
            GLCD_WriteChar(timer_sec/10+'0');
            GLCD_WriteChar(timer_sec%10+'0');
            GLCD_TextGoTo(16,0);
            GLCD_WriteDisplayData(0);
        }
        else
        {
            GLCD_TextGoTo(14,0);
            GLCD_WriteDisplayData(0);
            GLCD_TextGoTo(15,0);
            GLCD_WriteChar(timer_sec+'0');
            GLCD_TextGoTo(16,0);
            GLCD_WriteDisplayData(0);
        }
            if(!timer_sec){
            GLCD_TextGoTo(15,0);
            GLCD_WriteDisplayData(0);
            GLCD_WriteDisplayData(0);
            GLCD_WriteDisplayData(0);
                //flag--;
                timer_sec=16;
                time_flag=0;
            timeover_select();

            }
            if(!flag) {
                    TIMSK=0x00;
        }
        timercounter=0;
    }
    TCNT2=131;
}


void USART0_Transmit( char data )
{
    while(!(UCSR0A&0x20));
    UDR0 = data;
}

void AtoD(void)
{
    ADMUX =0x00;
    ADCSRA |= 0x40;
    while((ADCSRA&0x10)==0);
    val_x=(int)ADCL;
    val_x+=(int)ADCH*256;
    ADMUX =0x01;
    ADCSRA |= 0x40;
    while((ADCSRA&0x10)==0);
    val_y=(int)ADCL;
    val_y+=(int)ADCH*256;
    AtoD_cnt++;
    AtoD_cnt=AtoD_cnt%300;
}

void vibrating(void)
{
    PORTD|=0x10;
    delay_ms(500);
    PORTD&=0xef;
}

void mode_select(void)
{
     select=0;
     GLCD_TextGoTo(8,3);
     GLCD_WriteString("BATTLESHIP GAME");
     GLCD_TextGoTo(2,8);
     GLCD_WriteString("SINGLE MODE");
     GLCD_TextGoTo(17,8);
     GLCD_WriteString("MULTI MODE");
    while(select==0)
    {
        while(select==0&&mode==0){
            GLCD_TextGoTo(17,8);
            GLCD_WriteString("MULTI MODE");
            begin_cursoron(50,80);
            begin_cursoroff(170,80);
            AtoD();
            if(val_x>900) { mode=1; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo1(2,8);
            AtoD();
            if(val_x>900) { mode=1; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo(2,8);
            GLCD_WriteString("SINGLE MODE");
            AtoD();
            if(val_x>900) { mode=1; break;}
            if((PINF&0x08)==0x00) {select++; break;}
        }
        while(select==0&&mode==1){
            GLCD_TextGoTo(2,8);
            GLCD_WriteString("SINGLE MODE");
            begin_cursoron(170,80);
            begin_cursoroff(50,80);
            AtoD();
            if(val_x<100) { mode=0; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo1(17,8);
            AtoD();
            if(val_x<100) { mode=0; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo(17,8);
            GLCD_WriteString("MULTI MODE");
            AtoD();
            if(val_x<100) { mode=0; break;}
            if((PINF&0x08)==0x00) {select++; break;}
        }
    }
    if(mode==1) USART0_Transmit('a');
}

void difficulty_select(void)
{
     select=0;
     GLCD_TextGoTo(7,3);
     GLCD_WriteString("CHOOSE DIFFICULY");
     GLCD_TextGoTo(2,8);
     GLCD_WriteString("EASY MODE");
     GLCD_TextGoTo(17,8);
     GLCD_WriteString("HARD MODE");
    while(select==0)
    {
        while(select==0&&difficult==7){
            GLCD_TextGoTo(17,8);
            GLCD_WriteString("HARD MODE");
            begin_cursoron(50,80);
            begin_cursoroff(170,80);
            AtoD();
            if(val_x>900) { difficult=5; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo1(2,8);
            AtoD();
            if(val_x>900) { difficult=5; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo(2,8);
            GLCD_WriteString("EASY MODE");
            AtoD();
            if(val_x>900) { difficult=5; break;}
            if((PINF&0x08)==0x00) {select++; break;}
        }
        while(select==0&&difficult==5){
            GLCD_TextGoTo(2,8);
            GLCD_WriteString("EASY MODE");
            begin_cursoron(170,80);
            begin_cursoroff(50,80);
            AtoD();
            if(val_x<100) { difficult=7; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo1(17,8);
            AtoD();
            if(val_x<100) { difficult=7; break;}
            if((PINF&0x08)==0x00) {select++; break;}
            delay_ms(300);
            GLCD_TextGoTo(17,8);
            GLCD_WriteString("HARD MODE");
            AtoD();
            if(val_x<100) { difficult=7; break;}
            if((PINF&0x08)==0x00) {select++; break;}
        }
    }
}


void joystick_setting(void)
{
    while(1){
        AtoD();
        if(val_x>900)
        {
            i++;
            if(i+imax>7) i--;
            break;
        }
        else if(val_x<100)
        {
            i--;
            if(i+imin<0) i++;
            break;
        }
        else if(val_y>900)
        {
            j++;
            if(j+jmax>7) j--;
            break;
        }
        else if(val_y<100)
        {
            j--;
            if(j+jmin<0) j++;
            break;
        }
    }
}

void joystick_playing(void)
{
    AtoD();
    if(val_x>900){
        GLCD_cursor_off(i,j);
        if(counter_detect[i][j]){
            if(counter_set[i][j]==0) lGLCD_fail(i,j);
            else if(counter_set[i][j]==1||counter_set[i][j]==4) lGLCD_success(i,j);
            else if(counter_set[i][j]==2) lGLCD_item(i,j);
            else if(counter_set[i][j]==3) lGLCD_mine(i,j);
            else lGLCD_rock(i,j);
        }
        i++;
        if(i>7) i=7;
        lGLCD_cursor(i,j);
        delay_ms(200);
    }
    else if(val_x<100){
        GLCD_cursor_off(i,j);
        if(counter_detect[i][j]){
            if(counter_set[i][j]==0) lGLCD_fail(i,j);
            else if(counter_set[i][j]==1||counter_set[i][j]==4) lGLCD_success(i,j);
            else if(counter_set[i][j]==2) lGLCD_item(i,j);
            else if(counter_set[i][j]==3) lGLCD_mine(i,j);
            else lGLCD_rock(i,j);
        }
        i--;
        if(i<0) i=0;
        lGLCD_cursor(i,j);
        delay_ms(200);
    }
    else if(val_y>900){
        GLCD_cursor_off(i,j);
        if(counter_detect[i][j]){
            if(counter_set[i][j]==0) lGLCD_fail(i,j);
            else if(counter_set[i][j]==1||counter_set[i][j]==4) lGLCD_success(i,j);
            else if(counter_set[i][j]==2) lGLCD_item(i,j);
            else if(counter_set[i][j]==3) lGLCD_mine(i,j);
            else lGLCD_rock(i,j);
        }
        j++;
        if(j>7) j=7;
        lGLCD_cursor(i,j);
        delay_ms(200);
    }
    else if(val_y<100){
        GLCD_cursor_off(i,j);
        if(counter_detect[i][j]){
            if(counter_set[i][j]==0) lGLCD_fail(i,j);
            else if(counter_set[i][j]==1||counter_set[i][j]==4) lGLCD_success(i,j);
            else if(counter_set[i][j]==2) lGLCD_item(i,j);
            else if(counter_set[i][j]==3) lGLCD_mine(i,j);
            else lGLCD_rock(i,j);
        }
        j--;
        if(j<0) j=0;
        lGLCD_cursor(i,j);
        delay_ms(200);
    }
}
void timeover_select(void)
{
    rand_i=rand()%8;
    rand_j=rand()%8;
    if(!counter_set[rand_i][rand_j]){ //blank
        flag--;
        GLCD_cursor_off(i,j);
        lGLCD_fail(rand_i,rand_j);
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[rand_i][rand_j]>=1&&counter_set[rand_i][rand_j]<=4||counter_set[rand_i][rand_j]==7){//ship or rock
        GLCD_cursor_off(i,j);
        lGLCD_success(rand_i,rand_j);
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[rand_i][rand_j]==5){ //item
        flag++;
        GLCD_cursor_off(i,j);
        lGLCD_item(rand_i,rand_j);
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[rand_i][rand_j]==6){ //mine
        flag--;
        GLCD_cursor_off(i,j);
        lGLCD_mine(rand_i,rand_j);
        rand_k=rand()%8;
        rand_l=rand()%8;
        while(counter_detect[rand_k][rand_l]||game_set[rand_k][rand_l]!=1) rand_k=rand()%8, rand_l=rand()%8;
        rGLCD_cursor_off(rand_k,rand_l);
        rGLCD_success(rand_k,rand_l), my_detect[rand_k][rand_l]=1;
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[rand_i][rand_j]==8){
        GLCD_cursor_off(i,j);
        lGLCD_rock(rand_i,rand_j);
        vibrating();
        flag--;
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    counter_detect[rand_i][rand_j]=1;
    delay_ms(100);

}
void switch_select(void)
{
    is_found_rock();
    if(!counter_set[i][j]){ //blank
        flag--;
        GLCD_cursor_off(i,j);
        lGLCD_fail(i,j);
                time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[i][j]>=1&&counter_set[i][j]<=4||counter_set[i][j]==7){//ship or rock
        GLCD_cursor_off(i,j);
        lGLCD_success(i,j);
                time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[i][j]==5){ //item
        flag++;
        GLCD_cursor_off(i,j);
        lGLCD_item(i,j);
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[i][j]==6){ //mine
        flag--;
        GLCD_cursor_off(i,j);
        lGLCD_mine(i,j);
        rand_i=rand()%8;
        rand_j=rand()%8;
        while(counter_detect[rand_i][rand_j]||game_set[rand_i][rand_j]!=1) rand_i=rand()%8, rand_j=rand()%8;
        rGLCD_cursor_off(rand_i,rand_j);
        rGLCD_success(rand_i,rand_j), my_detect[rand_i][rand_j]=1;
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_set[i][j]==8){
        GLCD_cursor_off(i,j);
        lGLCD_rock(i,j);
        vibrating();
        flag--;
        time_flag++;
        TIMSK=0X00;
        timer_sec=16;
    }
    if(counter_detect[crockx[0]][crocky[0]]&&counter_set[crockx[0]][crocky[0]]==8){
        GLCD_cursor_off(crockx[0],crocky[0]);
        lGLCD_rock(crockx[0],crocky[0]);
        vibrating();
    }
    if(counter_detect[crockx[1]][crocky[1]]&&counter_set[crockx[1]][crocky[1]]==8){
        GLCD_cursor_off(crockx[1],crocky[1]);
        lGLCD_rock(crockx[1],crocky[1]);
        vibrating();
    }
    counter_detect[i][j]=1;
    delay_ms(100);
    USART0_Transmit('s');
    USART0_Transmit(i),USART0_Transmit(j);
}

void coordinate(void)
{
    int a,b;
    int l=0,m=0,n=0,o=0,p=0,q=0,r=0;

    for(a=0;a<8;a++)
        for(b=0;b<8;b++){
            if(counter_set[a][b]==1) cship1x[l]=a,cship1y[l]=b,l++;
            else if(counter_set[a][b]==2) cship2x[m]=a,cship2y[m]=b,m++;
            else if(counter_set[a][b]==3) cship3x[n]=a,cship3y[n]=b,n++;
            else if(counter_set[a][b]==4) cship4x[o]=a,cship4y[o]=b,o++;
            else if(counter_set[a][b]==5) citemx[p]=a,citemy[p]=b,p++;
            else if(counter_set[a][b]==6) cminex[q]=a,cminey[q]=b,q++;
            else if(counter_set[a][b]==7) crockx[r]=a,crocky[r]=b,r++;
        }
}

void is_found_rock(void)
{
    if(counter_detect[crockx[0]-1][crocky[0]]&&counter_detect[crockx[0]+1][crocky[0]]&&
       counter_detect[crockx[0]][crocky[0]-1]&&counter_detect[crockx[0]][crocky[0]+1])
        counter_set[crockx[0]][crocky[0]]=8;
    if(counter_detect[crockx[1]-1][crocky[1]]&&counter_detect[crockx[1]+1][crocky[1]]&&
       counter_detect[crockx[1]][crocky[1]-1]&&counter_detect[crockx[1]][crocky[1]+1])
        counter_set[crockx[1]][crocky[1]]=8;
}

void led_score(void)
{
    if(ship_cnt==1) PORTE|=0x10;
    else if(ship_cnt==2) PORTE|=0x20;
    else if(ship_cnt==3) PORTE|=0x40;
    else if(ship_cnt==4) PORTE|=0x80;
    else if(item_cnt==1) PORTE|=0x08;
    else if(item_cnt==2) PORTE|=0x04;
    else if(mine_cnt==1) PORTE|=0x02;
    else if(mine_cnt==2) PORTE|=0x01;
}

void register_timer(void)
{
    TCCR2=0x05;
    TCNT2=131;
    TIMSK=0X40;
    SREG=0x80;
}

void off_bounded(char *ship)
{
    int n;
    imax=0, imin=0, jmax=0, jmin=0;
    if(ship=="rship1"){
        for(n=0;n<4;n++)
            rGLCD_cursor_off(i+i1[n],j+j1[n]);
        for(n=0;n<4;n++) {
            if(imax<i1[n]) imax=i1[n];
            if(imin>i1[n]) imin=i1[n];
            if(jmax<j1[n]) jmax=j1[n];
            if(jmin>j1[n]) jmin=j1[n];
        }
    }
    else if (ship=="rship2"){
        for(n=0;n<5;n++)
            rGLCD_cursor_off(i+i2[n],j+j2[n]);
        for(n=0;n<5;n++) {
            if(imax<i2[n]) imax=i2[n];
            if(imin>i2[n]) imin=i2[n];
            if(jmax<j2[n]) jmax=j2[n];
            if(jmin>j2[n]) jmin=j2[n];
        }
    }
    else {
        for(n=0;n<2;n++)
            rGLCD_cursor_off(i+i3[n],j+j3[n]);
        for(n=0;n<2;n++) {
            if(imax<i3[n]) imax=i3[n];
            if(imin>i3[n]) imin=i3[n];
            if(jmax<j3[n]) jmax=j3[n];
            if(jmin>j3[n]) jmin=j3[n];
        }
    }
    if(i+imax>7) i--;
    else if(i+imin<0) i++;
    else if(j+jmax>7) j--;
    else if(j+jmin<0) j++;
}

int getchar(void)
{
     while ((UCSR0A & 0x80) == 0);
     return UDR0;
}


void set_tx_uart(void)
{
    int i,j;
    for(i=0;i<8;i++){
    for(j=0;j<8;j++){
    USART0_Transmit(game_set[i][j]);}}
}

void set_rx_uart(void)
{
    int i,j;
    for(i=0;i<8;i++){
    for(j=0;j<8;j++){
    counter_set[i][j]=getchar();}}
}

void main(void)
{
    DDRA=0xff; //GLCD databus
    DDRB=0x00; //UARTdata
    DDRC=0x1f; //0001 1111 GLCD setting
    DDRD=0x1e; //0001 1110 vibrating motor PD4, power down/on switch PD0 blue key cap
    DDRE=0xff; //1111 1111 ship PE7:4 white led, item PE3:2 red led, mine PE1:0 yellow led
    DDRF=0x80; //1000 0000 select switch PF3 yellow, turn switch PF2 red, joystick PF1:0



    EICRA=0x02; //0000 0010 PD0 falling edge. sleep mode
    EIMSK=0x01; //0000 0001 INT0 enable
    SREG=0x80;  //interrupt enable

    UCSR0A=0x00; //don't use flag register
    UCSR0B=0x18; //tx enable, rx enable, transmission bit = 8 bit
    UCSR0C=0x06; //asynchronous communication2
    UBRR0H=0;
    UBRR0L=103; //9600bps

    ADCSRA = 0x87; // ADCsinglemode, 128

    //register_timer();
    GLCD_Initialize();
    GLCD_Clear();

    mode_select();
    if(mode==0) {
            GLCD_Clear();
            delay_ms(100);
            difficulty_select();
            if(difficult==5){
                GLCD_Clear();
                GLCD_TextGoTo(7,7);
                GLCD_WriteString("Oh, seriously?");
                delay_ms(1000);
                GLCD_Clear();
                GLCD_TextGoTo(2,7);
                GLCD_WriteString("You're gonna regret");
                GLCD_TextGoTo(2,8);
                GLCD_WriteString("choosing the hard mode");
                delay_ms(2500);
            }
            }

    while(1) { //setting
        GLCD_Initialize();
        GLCD_Clear();

        GLCD_screen();

        imax=1, imin=0, jmax=1, jmin=-1;

        while(1){
            while(cnt==0){ //ship1. voiliers
                AtoD();
                rship1(i,j);
                if(val_x>900||val_x<100||val_y>900||val_y<100) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if((PINF&0x04)==0x00) state=2;
                else if((PINF&0x08)==0x00) state=3;
                if (state==1){
                    for(n=0;n<4;n++)
                        rGLCD_cursor_off(p_i+i1[n],p_j+j1[n]);
                    if(i+imax>7||i+imin<0||j+jmax>7||j+jmin<0) i=p_i, j=p_j;
                    state=0;
                }
                else if(state==2){
                    for(n=0;n<4;n++)
                        rGLCD_cursor_off(i+i1[n],j+j1[n]);
                    switch_turn("rship1");
                    off_bounded("rship1");
                    state=0;
                }
                else if(state==3){
                    for(n=0;n<4;n++)
                        game_set[i+i1[n]][j+j1[n]]=1;
                    ship1x=i, ship1y=j;
                    i=1, j=1;
                    imax=2, imin=-1, jmax=0, jmin=-1;
                    cnt++;
                    delay_ms(300);
                    state=-1;
                }
            }
            while(cnt==1){ //ship2. biggest ship
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x04)==0x00) state=2;
                else if(!state&&(PINF&0x08)==0x00) state=3;
                if(state==-1){
                    rship1(ship1x,ship1y);
                    for(n=0;n<5;n++){
                        if(game_set[i+i2[n]][j+j2[n]]) {
                            rGLCD_cursor_off(i+i2[n],j+j2[n]);
                            rGLCD_fail(i+i2[n],j+j2[n]);
                        }
                        else rGLCD_1block(i+i2[n],j+j2[n]);
                        state=0;
                    }
                }
                if(state==1){
                    for(n=0;n<5;n++)
                        rGLCD_cursor_off(p_i+i2[n],p_j+j2[n]);
                    state=-1;
                }
                else if(state==2){
                    for(n=0;n<5;n++)
                        rGLCD_cursor_off(i+i2[n],j+j2[n]);
                    switch_turn("rship2");
                    off_bounded("rship2");
                    state=-1;
                }
                else if(state==3){
                    for(n=0;n<5;n++)
                        if(game_set[i+i2[n]][j+j2[n]]) error=1;
                    if(!error){
                        for(n=0;n<5;n++) game_set[i+i2[n]][j+j2[n]]=2;
                        ship2x=i, ship2y=j;
                        i=0, j=0;
                        imax=1, imin=0, jmax=0, jmin=0;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==2){ //ship3. 2 blocks rectangular
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x04)==0x00) state=2;
                else if(!state&&(PINF&0x08)==0x00) state=3;
                if(state==-1){
                    rship1(ship1x,ship1y),rship2(ship2x,ship2y);
                    for(n=0;n<2;n++){
                        if(game_set[i+i3[n]][j+j3[n]]) {
                            rGLCD_cursor_off(i+i3[n],j+j3[n]);
                            rGLCD_fail(i+i3[n],j+j3[n]);
                        }
                        else rGLCD_1block(i+i3[n],j+j3[n]);
                        state=0;
                    }
                }
                if(state==1){
                    for(n=0;n<2;n++)
                        rGLCD_cursor_off(p_i+i3[n],p_j+j3[n]);
                    state=-1;
                }
                else if(state==2){
                    for(n=0;n<2;n++)
                        rGLCD_cursor_off(i+i3[n],j+j3[n]);
                    switch_turn("rship3");
                    off_bounded("rship3");
                    state=-1;
                }
                else if(state==3){
                    for(n=0;n<2;n++)
                        if(game_set[i+i3[n]][j+j3[n]]) error=1;
                    if(!error){
                        for(n=0;n<2;n++) game_set[i+i3[n]][j+j3[n]]=3;
                        ship3x=i, ship3y=j;
                        i=0, j=1;
                        imax=1, imin=0, jmax=0, jmin=-1;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==3){ //ship4. 4 blocks square
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;//joystick select
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rship1(ship1x,ship1y),rship2(ship2x,ship2y),rship3(ship3x,ship3y);
                    for(n=0;n<4;n++){
                        if(game_set[i+i4[n]][j+j4[n]]) {
                            rGLCD_cursor_off(i+i4[n],j+j4[n]);
                            rGLCD_fail(i+i4[n],j+j4[n]);
                        }
                        else rGLCD_1block(i+i4[n],j+j4[n]);
                        state=0;
                    }
                }
                else if(state==1){
                    for(n=0;n<4;n++)
                        rGLCD_cursor_off(p_i+i4[n],p_j+j4[n]);
                    if(i+imax>7||i+imin<0||j+jmax>7||j+jmin<0) i=p_i, j=p_j;
                    state=-1;
                }
                else if(state==2){
                    for(n=0;n<4;n++)
                        if(game_set[i+i4[n]][j+j4[n]]) error=1;
                    if(!error){
                        for(n=0;n<4;n++) game_set[i+i4[n]][j+j4[n]]=4;
                        ship4x=i, ship4y=j;
                        i=0, j=0;
                        imax=0, imin=0, jmax=0, jmin=0;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==4){ //item1
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rship1(ship1x,ship1y), rship2(ship2x,ship2y), rship3(ship3x,ship3y), rship4(ship4x,ship4y);
                    if(game_set[i][j]){
                        rGLCD_cursor_off(i,j);
                        rGLCD_fail(i,j);
                    }
                    else rGLCD_item(i,j);
                    state=0;
                    delay_ms(300);
                }
                else if(state==1){
                    rGLCD_cursor_off(p_i,p_j);
                    state=-1;
                }
                else if(state==2){
                    if(!game_set[i][j]) {
                        game_set[i][j]=5;
                        item1x=i,item1y=j;
                        i=0,j=0;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==5){ //item2
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rGLCD_item(item1x,item1y),
                    rship1(ship1x,ship1y), rship2(ship2x,ship2y), rship3(ship3x,ship3y), rship4(ship4x,ship4y);
                    if(game_set[i][j]){
                        rGLCD_cursor_off(i,j);
                        rGLCD_fail(i,j);
                    }
                    else rGLCD_item(i,j);
                    state=0;
                    delay_ms(300);
                }
                else if(state==1){
                    rGLCD_cursor_off(p_i,p_j);
                    state=-1;
                }
                else if(state==2){
                    if(!game_set[i][j]) {
                        game_set[i][j]=5;
                        item2x=i,item2y=j;
                        i=0,j=0;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==6){ //mine1
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rGLCD_item(item1x,item1y),rGLCD_item(item2x,item2y),
                    rship1(ship1x,ship1y), rship2(ship2x,ship2y), rship3(ship3x,ship3y), rship4(ship4x,ship4y);
                    if(game_set[i][j]){
                        rGLCD_cursor_off(i,j);
                        rGLCD_fail(i,j);
                    }
                    else rGLCD_mine(i,j);
                    state=0;
                    delay_ms(300);
                }
                else if(state==1){
                    rGLCD_cursor_off(p_i,p_j);
                    state=-1;
                }
                else if(state==2){
                    if(!game_set[i][j]) {
                        game_set[i][j]=6;
                        mine1x=i,mine1y=j;
                        i=0,j=0;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==7){ //mine2
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rGLCD_item(item1x,item1y),rGLCD_item(item2x,item2y),rGLCD_mine(mine1x,mine1y),
                    rship1(ship1x,ship1y), rship2(ship2x,ship2y), rship3(ship3x,ship3y), rship4(ship4x,ship4y);
                    if(game_set[i][j]){
                        rGLCD_cursor_off(i,j);
                        rGLCD_fail(i,j);
                    }
                    else rGLCD_mine(i,j);
                    state=0;
                    delay_ms(300);
                }
                else if(state==1){
                    rGLCD_cursor_off(p_i,p_j);
                    state=-1;
                }
                else if(state==2){
                    if(!game_set[i][j]) {
                        game_set[i][j]=6;
                        mine2x=i,mine2y=j;
                        i=0,j=0;
                        cnt++;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==8){ //rock1
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rGLCD_item(item1x,item1y),rGLCD_item(item2x,item2y),rGLCD_mine(mine1x,mine1y),rGLCD_mine(mine2x,mine2y),
                    rship1(ship1x,ship1y), rship2(ship2x,ship2y), rship3(ship3x,ship3y), rship4(ship4x,ship4y);
                    if(game_set[i][j]){
                        rGLCD_cursor_off(i,j);
                        rGLCD_fail(i,j);
                    }
                    else rGLCD_rock(i,j);
                    state=0;
                    delay_ms(300);
                }
                else if(state==1){
                    rGLCD_cursor_off(p_i,p_j);
                    state=-1;
                }
                else if(state==2){
                    if(!game_set[i][j]) {
                        game_set[i][j]=7;
                        rock1x=i,rock1y=j;
                        i=0,j=0;
                        cnt=9;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                    state=-1;
                }
            }
            while(cnt==9){ //rock2
                AtoD();
                if(!state&&(val_x>900||val_x<100||val_y>900||val_y<100)) {
                    p_i=i, p_j=j;
                    joystick_setting();
                    state=1;
                }
                else if(!state&&(PINF&0x08)==0x00) state=2;
                if(state==-1){
                    rGLCD_item(item1x,item1y),rGLCD_item(item2x,item2y),rGLCD_mine(mine1x,mine1y),rGLCD_mine(mine2x,mine2y),rGLCD_rock(rock1x,rock1y),
                    rship1(ship1x,ship1y), rship2(ship2x,ship2y), rship3(ship3x,ship3y), rship4(ship4x,ship4y);
                    if(game_set[i][j]){
                        rGLCD_cursor_off(i,j);
                        rGLCD_fail(i,j);
                    }
                    else rGLCD_rock(i,j);
                    state=0;
                    delay_ms
                    (300);
                }
                else if(state==1){
                    rGLCD_cursor_off(p_i,p_j);
                    state=-1;
                }
                else if(state==2){
                    if(!game_set[i][j]) {
                        game_set[i][j]=7;
                        rock2x=i,rock2y=j;
                        i=0,j=0;
                        cnt=10;
                        delay_ms(300);
                    }
                    else {
                        vibrating();
                        error=0;
                    }
                }
            }
            break;
        }
        break;
    }

    GLCD_Initialize();
    GLCD_Clear();
    srand(AtoD_cnt);
    GLCD_screen();

    for(i=0;i<8;i++)
        for(j=0;j<8;j++){
            if(game_set[i][j]>=1&&game_set[i][j]<=4) rGLCD_1block(i,j);
            else if(game_set[i][j]==5) rGLCD_item(i,j);
            else if(game_set[i][j]==6) rGLCD_mine(i,j);
            else if(game_set[i][j]==7) rGLCD_rock(i,j);
        }

    if(mode==0){//1인용
        char overlap=0;
        for(i=0;i<8;i++)
        for(j=0;j<8;j++)
        counter_set[i][j]=0;

        cnt=0;
        while(1){//컴퓨터가 세팅
            while(cnt==0){//ship1
            overlap=0;
            i=rand()%8;
            j=rand()%8;
            if(counter_set[i][j]==0){
                if(i>-1&&i<7&&j>0&&j<7){
                    for(n=0;n<4;n++)
                        if(counter_set[i+i1[n]][j+j1[n]]!=0) //주변 중복검사
                            overlap++;
                    if(overlap==0){
                        for(n=0;n<4;n++)
                            counter_set[i+i1[n]][j+j1[n]]=1;
                        cnt++;
                    }
                }
            }
            }
            while(cnt==1){//ship2
            overlap=0;
            i=rand()%8;
            j=rand()%8;
            if(counter_set[i][j]==0){
                if(i>0&&i<6&&j>0&&j<8){
                    for(n=0;n<5;n++)
                        if(counter_set[i+i2[n]][j+j2[n]]!=0) //주변 중복검사
                            overlap++;
                    if(overlap==0){
                        for(n=0;n<5;n++)
                            counter_set[i+i2[n]][j+j2[n]]=2;
                        cnt++;
                    }
                }
            }
            }
            while(cnt==2){//ship3
            overlap=0;
            i=rand()%8;
            j=rand()%8;
           if(counter_set[i][j]==0){
                if(i>-1&&i<7&&j>-1&&j<8){
                    for(n=0;n<2;n++)
                        if(counter_set[i+i3[n]][j+j3[n]]!=0) //주변 중복검사
                            overlap++;
                    if(overlap==0){
                        for(n=0;n<2;n++)
                            counter_set[i+i3[n]][j+j3[n]]=3;
                        cnt++;
                    }
                }
            }
            }
            while(cnt==3){//ship4
            overlap=0;
            i=rand()%8;
            j=rand()%8;
           if(counter_set[i][j]==0){
                if(i>-1&&i<7&&j>-1&&j<7){
                    for(n=0;n<4;n++)
                        if(counter_set[i+i4[n]][j+j4[n]]!=0) //주변 중복검사
                            overlap++;
                    if(overlap==0){
                        for(n=0;n<4;n++)
                            counter_set[i+i4[n]][j+j4[n]]=4;
                        cnt++;
                    }
                }
            }
            }
            while(cnt==4){//item1
            i=rand()%8;
            j=rand()%8;
            if(counter_set[i][j]==0){
                counter_set[i][j]=5;
                cnt++;

                }
            }
            while(cnt==5){//item2
            i=rand()%8;
            j=rand()%8;
            if(counter_set[i][j]==0){
                counter_set[i][j]=5;
                cnt++;

                }
            }
            while(cnt==6){//mine1
            i=rand()%8;
            j=rand()%8;
           if(counter_set[i][j]==0){
                counter_set[i][j]=6;
                cnt++;
                lGLCD_fail(1,6);

                }
            }
            while(cnt==7){//mine2
            i=rand()%8;
            j=rand()%8;
           if(counter_set[i][j]==0){
                counter_set[i][j]=6;
                cnt++;

                }
            }
            while(cnt==8){//rock1
            i=rand()%8;
            j=rand()%8;
            if(counter_set[i][j]==0){
                counter_set[i][j]=7;
                cnt++;

                }
            }
            while(cnt==9){//rock2
            i=rand()%8;
            j=rand()%8;
           if(counter_set[i][j]==0){
                counter_set[i][j]=7;
                break;

                }
                break;
            }
        break;
        }

        GLCD_Clear();
        GLCD_screen();

        for(i=0;i<8;i++)
            for(j=0;j<8;j++){
            if(game_set[i][j]>=1&&game_set[i][j]<=4) rGLCD_1block(i,j);
            else if(game_set[i][j]==5) rGLCD_item(i,j);
            else if(game_set[i][j]==6) rGLCD_mine(i,j);
            else if(game_set[i][j]==7) rGLCD_rock(i,j);
        }/*
//컴 세팅 보이게 하기
       for(i=0;i<8;i++)
            for(j=0;j<8;j++){
            if(counter_set[i][j]>=1&&counter_set[i][j]<=4) lGLCD_success(i,j);
            else if(counter_set[i][j]==5) lGLCD_item(i,j);
            else if(counter_set[i][j]==6) lGLCD_mine(i,j);
            else if(counter_set[i][j]==7) lGLCD_rock(i,j);
        }
*/
       while(1){ //playing
        flag=1;
        i=0,j=0;
        lGLCD_cursor(i,j);
        while(flag>0){ //my turnn
         if(time_flag>=1)
        {
            register_timer();
            time_flag=0;
        }
            joystick_playing();
            if((PINF&0x08)==0x00){ //select
                if(!counter_detect[i][j]){
                    switch_select();
                    if(counter_set[i][j]==1){
                        for(n=0;n<5;n++){
                            if(destroy[n]==-1&&!counter_detect[ship1x+i1[n]][ship1y+j1[n]]) destroy[0]=0;
                            else if(destroy[n]==-1&&!counter_detect[ship2x+i2[n]][ship2y+j2[n]]) destroy[1]=0;
                            else if(destroy[n]==-1&&!counter_detect[ship3x+i3[n]][ship3y+j3[n]]) destroy[2]=0;
                            else if(destroy[n]==-1&&!counter_detect[ship4x+i4[n]][ship4y+j4[n]]) destroy[3]=0;
                        }
                        for(n=0;n<4;n++){
                            if(destroy[n]==-1){
                                ship_cnt++, destroy[n]=1, led_score();
                                vibrating();
                            }
                            else if(!destroy[n]) destroy[n]=-1;
                        }
                        com_rest_ship--;
                        if(com_rest_ship==0){
                            GLCD_Clear();
                            GLCD_TextGoTo(10,7);
                            GLCD_WriteString("VICTOTY!!");
                            GLCD_cursor_off(0,0);
                            break;
                        }
                    }
                    else if(counter_set[i][j]==2){
                        item_cnt++, led_score();
                        vibrating();
                    }
                    else if(counter_set[i][j]==3){
                        mine_cnt++, led_score();
                        vibrating();
                    }
                    if(com_rest_ship==0) break;
                }
                if(com_rest_ship==0) break;
            }
            if(com_rest_ship==0) break;
        }
        if(com_rest_ship==0) break;

        flag=1;
        delay_ms(800);
        while(flag>0){ //com turn
            rand_i=rand()%8;
            rand_j=rand()%8;
            rate=rand()%10;
            if(rate>difficult){//easy: 20%의 확률 공격 hard: 40%확률 공격
                    cnt=0;
                    for(i=0;i<8;i++)
                    for(j=0;j<8;j++){
                        if(game_set[i][j]==1&&cnt==0) {
                            delay_ms(500);
                            rGLCD_cursor_off(i,j);
                            rGLCD_success(i,j);
                            game_set[i][j]=5;
                            cnt++;
                            }
                        }
                }
            else if(game_set[rand_i][rand_j]==0){//비적중
                rGLCD_cursor_off(rand_i,rand_j);
                rGLCD_fail(rand_i,rand_j);
                flag--;
                game_set[rand_i][rand_j]=5;
                }
            else if(game_set[rand_i][rand_j]==1||game_set[rand_i][rand_j]==4){//전함,암초적중
                rGLCD_cursor_off(rand_i,rand_j);
                rGLCD_success(rand_i,rand_j);
                game_set[rand_i][rand_j]=5;
            }
            else if(game_set[rand_i][rand_j]==2){//아이템
                rGLCD_fail(rand_i,rand_j);
                flag++;
                game_set[rand_i][rand_j]=5;
            }
            else if(game_set[rand_i][rand_j]==3){//지뢰
                rGLCD_cursor_off(rand_i,rand_j);
                rGLCD_fail(rand_i,rand_j);
                flag--;
                game_set[rand_i][rand_j]=5;
                i=rand()%8; j=rand()%8;
                while(counter_set[i][j]!=1) i=rand()%8, j=rand()%8;
                lGLCD_fail(i,j); game_set[i][j]=5;
            }
        }
        rest_ship=0;
        for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            if(game_set[i][j]==1)
                rest_ship++;

        if(rest_ship==0){
            GLCD_Clear();
            GLCD_TextGoTo(7,7);
            GLCD_WriteString("YOU LOSE haha");
            GLCD_cursor_off(0,0);
            break;
        }
        else if(rest_ship<5&&difficult==5){
            GLCD_TextGoTo(2,15);
            GLCD_WriteString("Why don't you give up?");
        }


    }
    }
    else{//2인용
        while(1){ //playing
            i=0,j=0;
            while(flag>0){ //my turn
                lGLCD_cursor(i,j);
                joystick_playing();
                if((PINF&0x08)==0x00){ //select
                    if(!counter_detect[i][j]){
                        switch_select();
                        if(counter_set[i][j]>=1&&counter_set[i][j]<=4){
                            for(n=0;n<5;n++){
                                if(destroy[0]==-1&&!counter_detect[cship1x[n]][cship1y[n]]) destroy[0]=0;
                                else if(destroy[1]==-1&&!counter_detect[cship2x[n]][cship2y[n]]) destroy[1]=0;
                                else if(destroy[2]==-1&&!counter_detect[cship3x[n]][cship3y[n]]) destroy[2]=0;
                                else if(destroy[3]==-1&&!counter_detect[cship4x[n]][cship4y[n]]) destroy[3]=0;
                            }
                            for(n=0;n<4;n++){
                                if(destroy[n]==-1){
                                    ship_cnt++, destroy[n]=1, led_score();
                                    vibrating();
                                }
                                else if(!destroy[n]) destroy[n]=-1;
                            }
                        }
                        else if(counter_set[i][j]==5){
                            item_cnt++, led_score();
                            vibrating();
                        }
                        else if(counter_set[i][j]==6){
                            mine_cnt++, led_score();
                            vibrating();
                        }
                    }
    //                  if(!mode) counter_detect[i][j] data transmit;
    //                  else
                    else vibrating();
                    register_timer();
                }
    //      flag=1 transmit
           // i=0, j=0;
    //      if(ship_cnt==4) win; break; tx=lose;
            }
    //        whlie(flag==0){
    //            if(mode==0){

    //            }
    //            else
    //        }
        }
        USART0_Transmit('t');
        while(1){//counter turn
            switch(getchar()){
            case 'm':
                mode=1;
                break;
            case 't':
                flag++;
                break;
            case 'c':
                GLCD_Clear();
                GLCD_TextGoTo(7,7);
                GLCD_WriteString("YOU LOSE haha");
                GLCD_cursor_off(0,0);
                break;
            case 's':
                cnt_x=getchar();
                cnt_y=getchar();
                my_detect[cnt_x][cnt_y]=1;
                if(game_set[cnt_x][cnt_y]==5) rGLCD_fail(cnt_x,cnt_y);
                else if(game_set[cnt_x][cnt_y]>=1&&game_set[cnt_x][cnt_y]<=4||game_set[cnt_x][cnt_y]==7) {
                    rGLCD_cursor_off(cnt_x,cnt_y);
                    rGLCD_success(cnt_x,cnt_y);
                }
                else {
                    rGLCD_cursor_off(cnt_x,cnt_y);
                    rGLCD_fail(cnt_x,cnt_y);
                }
                break;
            }
            if(flag==1) break;
        }
    }
}
