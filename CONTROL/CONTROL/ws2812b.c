/*
 * ws2812b.c
 *
 * Created: 2019-01-04 오후 3:16:08
 *  Author: Jae
 */ 

#include <avr/io.h>
#include "ws2812b.h"

void ws2812b_init(void)
{
	for(int i=0; i<ws2812b_NUM; i++)
	{
		ws2812b_op(0);
		ws2812b_op(0);
		ws2812b_op(0);
	}
}

void ws2812b_op(char scan)
{
	char i;
	for(i=0; i<8; i++)
	{
		if(scan & 0x80)
		{
			PORTA |= (1 << PORTA7);
			nop3;
			PORTA &= ~(1 << PORTA7);
		}
		else
		{
			PORTA |= (1 << PORTA7);
			nop;
			PORTA &= ~(1 << PORTA7);
		}
		scan <<= 1;
	}
}

void ws2812b_show_color(char num, char green, char red, char blue)
{
	for(char i=0; i<num; i++)
	{
		ws2812b_op(green);
		ws2812b_op(red);
		ws2812b_op(blue);
	}
}