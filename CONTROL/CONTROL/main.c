
/*
 * CONTROL.c
 *
 * Created: 2019-01-09 오후 4:05:59
 * Author : Jae
 */ 

#define F_CPU			8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__			// _delay 함수 변수인자 허용
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ws2812b.h"
#include "UV_def.h"

volatile uint8_t key_flag_ON = 0, key_flag_Power = 0;
volatile uint8_t state_ON = 0, state_Power = 0, state = 0;
volatile uint16_t cnt = 0;

void select_pow(uint8_t selection);
void UV_util_init(void);
void bz_operation(uint16_t hz, uint16_t count);

ISR(PCINT1_vect)
{
	uint8_t check_pin = PINB & 0x03;				// PCINT8, PCINT9 check (PB0, PB1)
// 	_delay_ms(100);									// debounce (if use cap(up to 100nF), remove this)
	switch(check_pin)
	{
		case TOUCH_FLAG_ON:							// PCINT9(PB1) Clicked
			if(key_flag_ON == 0)
			{
				key_flag_ON = 1;
				state_ON = !state_ON;
				bz_operation(1047, 150);
			}
			break;
		case TOUCH_FLAG_POW:						// PCINT8(PB0) Clicked
			if(!key_flag_Power)
			{
				key_flag_Power = 1;
				state_Power++;
				if(state_Power>2)
				{
					state_Power=0;
				}
				select_pow(state_Power);
				bz_operation(1047, 150);
			}
			break;
		default:									// key_flag init
			key_flag_ON = 0;
			key_flag_Power = 0;
			break;
	}
}

ISR(TIM0_COMPA_vect) 
{	
	cnt++;
	if(cnt >= 1499)
	{
		PORTA ^= (1 << STATE_LED);
		cnt = 0;
	}
}

void select_pow(uint8_t selection)
{
	switch(selection)
	{
		case POW_LOW:
		ON_UV_LOW;
		ws2812b_show_color(1, 0, 255, 0);
		break;
		case POW_MEDIUM:
		ON_UV_MEDIUM;
		ws2812b_show_color(1, 255, 0, 0);		
		break;
		case POW_HIGH:
		ON_UV_HIGH;
		ws2812b_show_color(1, 0, 0, 255);
		break;
	}
}

void bz_operation(uint16_t hz, uint16_t count)
{
	uint16_t i, ms, us;
	ms = 1000/(2*hz);								// 1개 펄스의 ON 또는 OFF의 ms 단위 시간
	us = (1000.0/(2*hz) - ms) * 1000;				// 1개 펄스의 ON 또는 OFF의 us 단위 시간
	for(i=0; i<count; i++)
	{
		PORTB ^= (1 << PORTB2);						// BUzzer ON - OFF
		_delay_us(us);								// (us)us 동안 delay
	}
}

void UV_util_init(void)
{
	// OUTPUT init
	DDRA |=	(1 << PORTA0) | (1 << PORTA1) | (1 << PORTA2) | (1 << PORTA3) | (1 << PORTA7);
	DDRB |= (1 << PORTB2);

	// PIN Change Interrupt
	GIMSK = (1 << PCIE1);
	GIFR = (1 << PCIF1);
	PCMSK1 = (1 << PCINT8) | (1 << PCINT9);
	
	// TIMER0 8bit
	TCCR0A = (1 << WGM01);						// CTC MODE
	TIMSK0 = (1 << OCIE0A);						// TIM0 COMP INT Enable
	OCR0A = 249;								// F_CPU/(2*Prescale*(1+249))

	ws2812b_init();
	_delay_ms(10);
	ws2812b_show_color(1, 0, 255, 0);
}

int main(void)
{
	UV_util_init();
	sei();
	
    while (1) 
    {
		if(state_ON == 0)
		{
			ON_STATE;							// 상태 LED ON
			TCCR0B = 0;							// 타이머 OFF		
		}
		else
		{
			TCCR0B = (1 << CS01);				// 타이머 ON
		}
	}
}
