/*
 * PWM.c
 *
 * Created: 7/23/2018 12:13:43 AM
 * Author : Nick Hulsey
 */ 
/*
 * Blink.c
 *
 * Created: 7/22/2018 8:08:19 PM
 * Author : Nick Hulsey
 
 GREAT GUY To Watch:
 https://www.youtube.com/watch?v=ZhIRRyhfhLM
 https://www.youtube.com/watch?v=YR4I0G_ILk4
 */ 

#include <avr/io.h> //this is needed
#define F_CPU 16000000UL //tell the CPU which crystal we are using, not sure this is needed
#include <util/delay.h> //so is this....idk what they do
#include <avr/interrupt.h>
/************************************************************************/
/*
HOW TO PWM
SET DATA DIRECITON TO 1 (PINMODE OUTPUT) USING: DDRD |= (1 << PORTD6)
SET DESCRIPTION FOR PWM USING TCCR0A to select (Wave generation mode and compare out mode)
SET TIME MASK DESCRIPTION (TIMSK0)
Turn brightness off OCR0A = (dutyCycle /100.0) * 255;
dutyCycle = 0; (declare above int main(void)
sei();  = set external interrupt

declare function: ISR(TIMER0_0VF_vect){
	OCR0A = (dutyCycle / 100) * 255.0;
}

	 
now in your while loop change duty cycle to change brightness of an led

remember to include <avr/interrupt.h>
*************************************************************************** */

/************************************************************************/
/* ACCORDING TO THIS:
http://extremeelectronics.co.in/avr-tutorials/pwm-signal-generation-by-using-avr-timers-part-ii/
You do not need to do TIMSK0 or sei(); or TCCR0B

They just use the following:
OCR0 = dutycycle (0 - 255);
TCCR0 |= (Settings)
DDRB |= (1 << Pin);      
ALSO THEY DO NOT USE A AND B but avr c throws an error if you dont have them???
TCCR0A != TCCR0 <- Doesnt work

^^^ I dont think this works AT ALL dont follow link                                                          */
/************************************************************************/

double dutyCycle = 0; //the percent brightness (or percent of the time on)

int main(void)
{
	DDRD |= (1 << PORTD6);	
	//(Timer Counter Control Register A For Timer 0
	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1<< WGM01); //set the description of the 0C0A (PD6) PWM description
	//COM0A1 = COMPARE OUTPUT MODE (in this case (A1 = 1, A0 =0) = Clear OC0A on Campare Match, set OC0A at BOTTOM (non-inverting mode)
	//WGM00 = WAVE GENERATION MODE
	
	TIMSK0 = (1 << TOIE0);//Time mask what kind of interupt you want set
						  //In this case it is Overflow interrupt Enable so call interupt when we get to the top and loop over again
	
	OCR0A = (dutyCycle / 100.0) * 255.0; //set percent time we are on (max value is 255 = 8 bits) in this case turn it off = 0
	
	sei(); //set external interrupt
	
	TCCR0B |= (1 << CS00) | (1 << CS02); //Start the clock and set prescaler
    
	while (1) 
    {
		if(dutyCycle > 100){
			dutyCycle = 0;
		}
		_delay_ms(500);
		dutyCycle += 20;//change the % brightness
		
		
		
    }
}


ISR(TIMER0_OVF_vect){ //Timer overflow vector or when OCR0A == 255 = Dutycycle == 100
	OCR0A = (dutyCycle / 100) * 255.0; //update the brightness ...   255 => OCR0A >= 0

}

