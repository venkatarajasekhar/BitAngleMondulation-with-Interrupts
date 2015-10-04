/*
 * BitAngleModulation_INTs.cpp
 *
 * Created: 02.10.2015 08:56:11
 *  Author: Arthur Palmer
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t helligkeit = 0;
volatile uint8_t BAM_i = 0;

ISR(TIMER0_OVF0_vect)
{
	uint8_t BitAngle = 1 << BAM_i;
	
	if (BitAngle & helligkeit)	
		PORTA = 0x03;
	else
		PORTA = 0x00;
	
	BAM_i = (BAM_i + 1) & 0x07;
	
	TCNT0 = -BitAngle;
}

void set_ints()
{
	// T0_Overflow_INT aktivieren
	TIMSK = (1 << TOIE0);
	sei();
}

void start_timer()
{
	// Prescale setzen und damit Timer starten
	TCCR0 = 0b00000011;
}

int main(void)
{
	int8_t dir = 1;
	
	DDRA = 0x03;
	
	set_ints();
	start_timer();
	
    while(1)
    {
		helligkeit += dir;
		if (helligkeit == 0) dir = 1;
		if (helligkeit == 255) dir = -1;
		_delay_ms(3);
    }
}