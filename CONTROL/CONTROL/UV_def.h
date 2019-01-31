/*
 * UV_def.h
 *
 * Created: 2019-01-29 오후 5:55:20
 *  Author: Jae
 */ 


#ifndef UV_DEF_H_
#define UV_DEF_H_

#define ON				1
#define OFF				0

#define UV_LOW			PORTA0
#define UV_MEDIUM		PORTA1
#define UV_HIGH			PORTA2
#define BUZZER			PORTB2
#define STATE_LED		PORTA3
#define WS2812_LED		PORTA7
#define POW_LOW				0
#define	POW_MEDIUM			1
#define POW_HIGH			2

#define TOUCH_FLAG_POW		0x01					// PCINT8(PB0)
#define TOUCH_FLAG_ON		0x02					// PCINT9(PB1)

#define ON_STATE		PORTA |= (1 << STATE_LED);



#endif /* UV_DEF_H_ */