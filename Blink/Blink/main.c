/*
 * Blink.c
 *
 * Created: 7/22/2018 8:08:19 PM
 * Author : Nick Hulsey
	
	AVRDUDE: 
	http://www.ladyada.net/learn/avr/avrdude.html
	https://www.youtube.com/watch?v=ERY7d7W-6nA
	
	WE ARE PROGRAMMING IN AVR C

 */ 

#include <avr/io.h> //this is needed
#define F_CPU 16000000UL //tell the CPU which crystal we are using, not sure this is needed

#include <util/delay.h> //so is this....idk what they do
//NOTE: programming it from here will make the chip run 8 times faster becasue fuses are not set to run with 16mhz crystal



/************************************************************************//*
So, This is a basic blink sketch, but theres some things to know: 
since I am using a USBTINYisp, it is not supported with atmel studio
I had to tell it to use avrDude which can use the USBTINYISP
go to tools>>External Tools
-c usbtiny -p m328p -v -v -v -U flash:w:$(TargetDir)$(TargetName).hex:i  -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m
This is the AVRDude command line
atmel studio will preset fuses for you, but since I did not have the right isp (you need isp mkii) avrdude had to do it

fuses are like the presettings to how your microcontroller will run (and I dont get it)
These are the fuse settings for the atmega328p with a 16mhz crystal -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m
There are: Low_fuses, high_fuses and extended_fuses
the fuse settings above are how to run the atmega328p on a 16Mhz crystal

HUGE NOTE:
the define F_CPU must be after the #include<avr/io.h> command (no = is needed)

This code runs fine in ARDUINO IDE
I think I just need the proper isp


*//************************************************************************/
int main(void)
{
	//DDRB |= (1<<2); //Data direction register for port B making the first bit(0) a 1 for output
	DDRB |= (1 << PORTB2) | (1<<PORTB3); //turn on two shift registers
	
    /* Replace with your application code */
    while (1) 
    {
		//PORTB |= (1<<2); //turn PB0 on  or use the command PORTB |= (1<<PORTB0)
						 //1<<2 turns on PB2 or 1<<PORTB2
						 
		PORTB |= (1 << PORTB2) | (1<<PORTB3);	//this will turn on 2 PORTS At the same time
		
		_delay_ms(1000); //wait 1000ms requires the delay.h include
		
		//PORTB &= ~(1<<2); //turn PB0 off
		PORTB &= ~((1 << PORTB2) | (1<<PORTB3));//To turn off two leds
		
		_delay_ms(1000); 
		
		
		//ANOTHER WAY TO BLINK A REGISTER:
		PORTB ^= (1<<PORTB3); //if 0 XOR 1 --> 1
							  //then its 1 XOR 1 --> 0 
							  //now its back to 0 XOR 1 --> 1
		_delay_ms(1000);
		
				
    }
}