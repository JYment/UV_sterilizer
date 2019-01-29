/*
 * ws2812b.h
 *
 * Created: 2019-01-04 오후 3:11:18
 *  Author: Jae
 */ 


#ifndef WS2812B_H_
#define WS2812B_H_

// 1/16Mhz = 62.5ns : 1cycle : asm volatile("NOP");
// nop2 = 62.5 * 2	= 125ns
// nop6 = 62.5 * 6	= 375ns
// nop16 = 62.5 * 16= 1000ns
#define nop2	{asm volatile(" NOP "); asm volatile(" NOP ");}
#define nop6	{nop2; nop2; nop2;}
#define nop16	{nop2; nop2; nop2; nop2; nop2; nop2; nop2; nop2;}
#define ws2812b_NUM			3

void ws2812b_init(void);
void ws2812b_op(char d);


#endif /* WS2812B_H_ */