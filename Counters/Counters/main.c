/*
 * Counters.c
 *
 * Created: 7/25/2018 10:47:02 PM
 * Author : Nick Hulsey
 https://eleccelerator.com/avr-timer-calculator/
 https://www.youtube.com/watch?v=cAui6116XKc&list=PLA6BB228B08B03EDD&index=6
 */ 

/************************************************************************/
/*					         HOW TIMERS WORK                            

SET THE SETTINGS USING TCCR0A AND TCCR0B (WHICH MODE IT IS IN...CTC OR PWM)
	- YOU CAN ALSO STATE HOW OCR0A AND OCR0B ARE CONTROLED
OCR0A IS A COMPARE VALUE THAT YOU SET, WHICH WILL CALL A FUNCTION WHEN THE TIMER = 0CR0A
YOU CAN USE THE CALCULATOR ABOVE FOR TIMING SINCE WE HAVE A 16MHZ CRYSTAL, 0.01632 (for 1/2024) FOR THE TIMER TO REACH 255
TIMSK0 = INTERRUPT MASK REGISTER WHICH CHANGES HOW THE INTERRUPT WILL BE CALLED
	- WHEN YOU COMPARE THE TIMER TO OCR0A
	- WHEN YOU COMAPER THE TIMER TO OCR0B		
	- WHEN YOU THERE IS AN OVERFLOW (IDK)
	
sei(); INITIATES THE INTERRUPT METHOD (ESSENTIALLY) YOU ALSO NEED... #include<avr/interrupt.h>

TCCR0B is theother "settings" register this sets the prescaler which can slow down our timer

explaining prescalers:
since this is an 8-bit counter the max value is 255: (there is a 16-bit)
the counter/timer ticks until it gets to 255 then goes back to 0

a prescaler states that 1 "tick" happens ever X amount
so if our prescaler is 1024 then a tick is only counted ever 1024 ticks
so 1 tick = 1 tick / prescaler

ISR (TIMER0_COMPA_vect) IS THE INTERRUPT FUNCTION THAT GETS CALLED WHEN TIMER 0 IS COMPARED TO OCR0A
																		*/
/************************************************************************/


#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>

#define LED_PIN PORTB3

int extraTime = 0;
int main(void)
{
	DDRB |= (1<<LED_PIN);

	//Put counter in CTC (clear timer on compare)
	TCCR0A |= (1 << WGM01);

	//set compare value
	OCR0A = 156; //once the 
	
	TIMSK0 |= (1 << OCIE0A); //OCIE0A = when compare value = timer value then call interrupt
	sei();//set external interrupt
		
	//SET prescaler to 1024 and starts the counter
	TCCR0B |= (1 << CS02) | (1 << CS00);
		
    while (1) 
    {
		
		
    }

}

//called when timer a compares to OCR0A
ISR (TIMER0_COMPA_vect){
	extraTime ++;
	if(extraTime >= 100){
		PORTB ^= (1 << LED_PIN);
		extraTime = 0;
	}
	
}