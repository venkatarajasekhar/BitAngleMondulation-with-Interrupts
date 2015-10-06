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

volatile uint8_t helligkeit = 0;	// Die Helligkeit der LED die in der Endlosschleife verändert werden soll
volatile uint8_t BAM_i = 0;			// BAM-Index, läuft von 0-7

ISR(TIMER0_OVF0_vect)
{
	uint8_t BitAngle = 1 << BAM_i;
	
	if (BitAngle & helligkeit)
		PORTA = 0x03;	// LED(s) an
	else
		PORTA = 0x00;	// LED(s) aus
	
	BAM_i = (BAM_i + 1) & 0x07;	// BAM-Index hochzählen, aber auf die Werte 0-7 beschränken
	
	TCNT0 = -BitAngle;	// Den counter auf "negativen" Wert setzen, damit der overflow entsprechend schnell/langsam ausgelöst wird
}

void set_ints()
{
	TIMSK = (1 << TOIE0);	// "Timer 0 Overflow Interrupt" aktivieren
	sei();					// Interrupts allgemein aktivieren
}

void start_timer()
{
	// Prescale setzen und damit auch den Timer starten
	// CS0 = 1, CS1 = 1 ==> System-Clock / 64
	TCCR0 = 0b00000011; 
}

int main(void)
{
	int8_t dir = 1;	//"direction", die Richtung der Helligkeitsänderung

	DDRA = 0x03;	// Pin 0+1 von Port A als Ausgang konfigurieren

	set_ints();		// Interrupt(s) aktivieren
	start_timer();	// Counter starten

	while(1)
	{
		helligkeit += dir;
		if (helligkeit == 0) dir = 1;		// wenn die LED aus ist, soll sie wieder heller werden
		if (helligkeit == 255) dir = -1;	// wenn maximale Helligkeit erreicht, soll die LED wieder dunkler werden
		_delay_ms(3);						// Wartezeit damit man die Helligkeitsänderung sehen und nicht nur messen kann
    }
}