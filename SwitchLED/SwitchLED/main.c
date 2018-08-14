/*
 * SwitchLED.c
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
	
    /* Replace with your application code */
    while (1) 
    {
		if(!(PINB & (1 << PINB1))) //REMEMBER ([1<<]PINB1)
		{
			PORTB |= (1 << PORTB3);
		}
		else{
			PORTB &= ~(1 << PORTB3);
		}
		
		
    }
}

