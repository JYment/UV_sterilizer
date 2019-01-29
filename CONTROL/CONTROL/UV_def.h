﻿/*
 * UV_def.h
 *
 * Created: 2019-01-29 오후 5:55:20
 *  Author: Jae
 */ 


#ifndef UV_DEF_H_
#define UV_DEF_H_

#define UV_LOW			PORTA0
#define UV_MEDIUM		PORTA1
#define UV_HIGH			PORTA2
#define BUZZER			PORTB2
#define STATE_LED		PORTA3
#define WS2812_LED		PORTA7
#define POW_LOW				0
#define	POW_MEDIUM			1
#define POW_HIGH			2

#define ON_STATE		PORTA |= (1 << STATE_LED);
#define ON_UV_LOW		PORTA |= ((1 << UV_LOW) & ~(1 << UV_MEDIUM) & ~(1 << UV_HIGH));
#define ON_UV_MEDIUM	PORTA |= ((1 << UV_MEDIUM) & ~(1 << UV_LOW) & ~(1 << UV_HIGH));
#define ON_UV_HIGH		PORTA |= ((1 << UV_HIGH) & ~(1 << UV_MEDIUM) & ~(1 << UV_LOW));
#define OFF_UV			PORTA |= (~(1 << UV_LOW) & ~(1 << UV_MEDIUM) & ~(1 << UV_HIGH));


#endif /* UV_DEF_H_ */