/*
 * PRACTICEPROJECT.c
 *
 * Created: 7/23/2018 10:07:33 PM
 * Author : Nick Hulsey
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{	
	
	//PINB1 == PORTB1????? PINB != PORTB
	// P[O]RT = [O]UTPUT   I/O   P[I]N = [I]NPUT
	//Button is connected to PB1 and LED is PB3
	DDRB |= (1 << PORTB3); //turn on output for pin B3 input is default
	PORTB |= (1 << PORTB1); //this enables internal resistor 
							//when button is not pressed = 1 when it is = 0
							//youre essentially turning on pin that is set to input
	
	//output mode for PD3
	DDRD |= (1 << PORTD3);
	
	//SETUP PWM on PB3 0C2A
	//TCCR2A AND TCCR2B CONTROL THE CLOCK REGISTER
	//COM2A1 CONTROLS HOW OCR2A OPPERATES
	//COM2B1 CONTROLS HOW OCR2B OPPERATES
	//WGM21 AND WGM20 CONTROL HOW 
	TCCR2A |= (1<<WGM20) | (1<<COM2A1)| (1<<WGM21) | (1 << COM2B1);
	OCR2A = 0;
	
	TCCR2B |= (1 << CS20) | (1 << CS22); //CLOCK SELECT (CS00 seems to have no effect) this turns on the counter 
	//sets prescaler ^^
	
	int brightness = 0;
    while (1) 
    {
		if(!(PINB & (1 << PINB1))) //REMEMBER ([1<<]PINB1)
		{
			brightness += 10;
		
			if(brightness >= 150)
				brightness = 0;
		
			//PORTB |= (1 << PORTB3);
			_delay_ms(80);
		}
		else{
			//PORTB &= ~(1 << PORTB3);
			brightness = 0;
		}
		OCR2A = brightness;	
		OCR2B = 150 - brightness;
		
    }
}

