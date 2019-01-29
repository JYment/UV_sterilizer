
/*
 * CONTROL.c
 *
 * Created: 2019-01-09 오후 4:05:59
 * Author : Jae
 */ 

#define F_CPU		1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ws2812b.h"

#define UV_LOW			PORTA0
#define UV_MID			PORTA1
#define UV_HIGH			PORTA2
#define BUZZER			PORTB2
#define STATE_LED		PORTA3
#define WS2812_LED		PORTA7


#define ON_STATE		PORTA |= (1 << STATE_LED)
#define ON_UV_LOW		PORTA |= (1 << UV_LOW) & ~(1 << UV_MID) & ~(1 << UV_HIGH)
#define ON_UV_MID		PORTA |= (1 << UV_MID) & ~(1 << UV_LOW) & ~(1 << UV_HIGH)
#define ON_UV_HIGH		PORTA |= (1 << UV_HIGH) & ~(1 << UV_LOW) & ~(1 << UV_MID)
#define OFF_UV			PORTA |= ~(1 << UV_LOW) & ~(1 << UV_MID) & ~(1 << UV_HIGH)

volatile char key_flag_ON = 0, key_flag_Power = 0;
volatile char key_state_ON = 0;
volatile char cnt = 0;

ISR(PCINT1_vect)
{
	_delay_ms(50);
	if(!(PINB & 0x01))
	{
		if(!key_flag_ON)
		{
			key_flag_ON = 1;
			key_state_ON = !key_state_ON;
		}
	}
	else
	{
		key_flag_ON = 0;
	}
	if(!(PINB & 0x02))
	{
		if(!key_flag_Power)
		{
			key_flag_Power = 1;
		}
	}
	else
	{
		key_flag_Power = 0;
	}
}

// 0.5s
ISR(TIM0_COMPA_vect) 
{
	cnt++;
	if(cnt >= 2)
	{
		PORTA ^= (1 << STATE_LED);
		cnt = 0;
	}
}

int main(void)
{
	DDRA |=	(1 << PORTA0) | (1 << PORTA1) | (1 << PORTA2) | (1 << PORTA3) | (1 << PORTA7);
	DDRB |= (1 << PORTB2);
	DDRB &=	~(1 << PORTB0) | ~(1 << PORTB1);

	ON_STATE;		// 전원 입력 시 LED ON
	
	// PIN Change Interrupt
	MCUCR |= (1 << ISC01);
	GIMSK = (1 << PCIE1);
	GIFR = (1 << PCIF1);
	PCMSK1 = (1 << PCINT8) | (1 << PCINT9);
	
	//TIMER0 8bit
	TCCR0A = (1 << WGM01);						// CTC MODE
	TIMSK0 = (1 << OCIE0A);
	OCR0A = 249;
	sei();
    while (1) 
    {
		if(key_state_ON == 1)
		{
			TCCR0B = (1 << CS02) | (1 << CS00);		// 8000000 / 8*2*(1 + OCR0A)
		}
		else
		{
			TCCR0B = 0;								// 타이머 OFF
		}
    }
}