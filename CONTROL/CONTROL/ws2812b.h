/*
 * ws2812b.h
 *
 * Created: 2019-01-04 오후 3:11:18
 *  Author: Jae
 */ 


#ifndef WS2812B_H_
#define WS2812B_H_

#define ws2812b_NUM			3

// 1/16Mhz = 62.5ns : 1cycle : asm volatile("NOP");
// nop2 = 62.5 * 2	= 125ns
// nop6 = 62.5 * 6	= 375ns
// nop16 = 62.5 * 16= 1000ns
// #define nop2	{asm volatile(" NOP "); asm volatile(" NOP ");}
// #define nop6	{nop2; nop2; nop2;}
// #define nop16	{nop2; nop2; nop2; nop2; nop2; nop2; nop2; nop2;}


// ATtiny44a - internal 8MHz ->
// 1/8Mhz = 125ns : 1cycle : asm volatile(" NOP ");
// nop3 = 125 * 3	= 375ns
// nop8 = 125 * 8	= 1000ns
#define nop		{asm volatile(" NOP ");}
#define nop3	{nop; nop; nop;}

void ws2812b_init(void);
void ws2812b_op(char d);
void ws2812b_show_color(char num, char green, char red, char blue);

#endif /* WS2812B_H_ */