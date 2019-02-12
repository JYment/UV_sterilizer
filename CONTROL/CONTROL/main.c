
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
#include "pitches.h"

volatile uint8_t key_flag_ON = 0, key_flag_Power = 0;
volatile uint8_t state_ON = 0, state_Power = 0, state = 0;
volatile uint16_t cnt = 0, sec_cnt = 0;

void select_pow_color(uint8_t selection);
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
				bz_operation(NOTE_C6, 150);
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
				select_pow_color(state_Power);
				bz_operation(NOTE_C6, 150);
			}
			break;
		default:									// key_flag init
			key_flag_ON = 0;
			key_flag_Power = 0;
			break;
	}
}

ISR(TIM1_COMPA_vect)
{
	cnt++;
	if(state_ON == ON)
	{
		if(cnt >= 1999)								// 0.5s CNT
		{
			if(sec_cnt < 1800)						// 900s (15min) CNT
			{
				PORTA ^= (1 << STATE_LED);
				switch(state_Power)
				{
					case POW_LOW:
					OCR0A = 100;
					break;
					case POW_MEDIUM:
					OCR0A = 70;
					break;
					case POW_HIGH:
					OCR0A = 30;
					break;
				}
				
				sec_cnt++;
			}
			else
			{
				state_ON = OFF;
				sec_cnt = 0;
			}
			cnt = 0;
		}
	}
	else
	{
		sec_cnt = 0;
		cnt = 0;
	}
}

void select_pow_color(uint8_t selection)
{
	switch(selection)
	{
		case POW_LOW:
		ws2812b_show_color(1, 0, 255, 0);
		break;
		case POW_MEDIUM:
		ws2812b_show_color(1, 255, 0, 0);		
		break;
		case POW_HIGH:
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
		PORTA ^= (1 << PORTA0);						// Buzzer ON - OFF
		_delay_us(us);								// (us)us 동안 delay
	}
}

void UV_util_init(void)
{
	// OUTPUT init
	DDRA |=	(1 << PORTA3) | (1 << PORTA7) | (1 << PORTA0);
	DDRB |= (1 << PORTB2);

	// PIN Change Interrupt
	GIMSK = (1 << PCIE1);
	GIFR = (1 << PCIF1);
	PCMSK1 = (1 << PCINT8) | (1 << PCINT9);
	
	// TIMER0 8bit
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);		// FAST PWM MODE
	TCCR0B = (1 << CS01);										// F_CPU/(2*Prescale*(1+249))
	

	// TIMER1 16bit
	TCCR1B = (1 << WGM12);
	TIMSK1 = (1 << OCIE1A);
	OCR1A = 249;
	
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
		if(state_ON == OFF)
		{
			ON_STATE;							// 상태 LED ON
			TCCR1B &= ~(1 << CS11);
			OCR0A = 0;
			PORTB &= ~(1 << PORTB2);
		}
		else
		{
			TCCR1B |= (1 << CS11);				// 타이머 ON
		}
	}
}
