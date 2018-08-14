/*
 * SerialCommunication(TX).c
 *
 * Created: 8/3/2018 2:08:48 PM
 * Author : Nick Hulsey
 */ 

//TX = Transmission
//look at USART on datasheet for details
//BAUD Rate = bits per second, but the chip needs to know UBRR (register) which tells the chip how fast to run

//use the serial to usb commmuncation chip (red sparkfun) 
//open tera term
// atmega328 -> chip
// RX -> TX
// TX -> RX
//make sure power and gnds are the same 
//so use the chip to power the atmega328

#include <avr/io.h>
#define F_CPU  16000000UL
#define BAUD   9600
#define BRC    ((F_CPU/16/BAUD) - 1)
#define LED_BUILTIN PORTB3;
#define TX_BUFFER_SIZE 128

#include <util/delay.h>
#include <avr/interrupt.h>

char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;

//must declare methods above in your code
void appendSerial(char c);
void serialWrite(char c[]);

int count = 0;


int main(void)
{
	DDRB |= (1 << PORTB3);
	
    UBRR0H = (BRC << 8); //this tells the chip how fast to run by shifting the brc into the register
	UBRR0L = (BRC); //theres the high and lower part of the register
	
	//three control registers for usart: A,B,C
	
	//all of these sentings can be found in the datasheet under usart
	UCSR0B |= (1 << TXEN0);//enable transmission
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //parity mode: simplest form of error detection 
	//^^set how big our transmission is....set to 8 bits rn
	UCSR0B |= (1 << TXCIE0); //enable transmission complete interrupt
	sei(); 
	
    while (1) 
    {
		//UDR0 = 'H';//this is the register we write to when we want send or recieve data
		//CAN ONLY BE ONE BIT OF DATA AT A TIME SO JUST A SINGLE CHARACTER 'A' OR '8' 
		//_delay_ms(1000);
		serialWrite("Look! I can count!\n\r");
		
		char countArray[5];
		itoa(count,countArray,2);//must convert count into a char array using this method
								  //10 = decimal 2 = binary and so on
		serialWrite(countArray);
		count ++;
		
		serialWrite("\n\r");
		
		_delay_ms(1000);
		
		PORTB ^= (1 << PORTB3);
		
    }
	
	
}

/************************************************************************//* 
	HOW ALL OF THIS WORKS:
	SO YOU SETUP THE TX COMMUNICATION BY SETTING SOME SETTINGS ON THE 3 REGISTERS:
		UCSR0A, UCSR0B, UCSR0C
	THEN YOU TELL THE CHIP HOW FAST WE ARE SENDING DATA:
		USING THIS EQUATION: BRC = ((F_CPU/16/BAUD) - 1)
	THEN WE ADD THAT TO THE UBRR0H AND UBRR0L REGISTERS
	
	NOW THIS IS ALL YOU REALLY HAVE TO DO, TO WRITE A SINGLE CHAR OF DATA YOU JUST DO:
	UDR0 = 'A'
	
	BUT WHAT IF WE WANT TO SEND A STRING?
		WELL, JUST MAKE A CHAR ARRAY, ADD OUR STRING TO IT, AND THEN SEND EACH CHARACTER OVER INDIVIDUALLY
	
	WE CAN ACCOMPLISH THIS BY MAKING AN INTERRUPT
		THIS INTERRUPT WILL WILL BE CALLED EVERYTIME UDR0 IS CLEARED/THE DATA IS SENT
		
	SO, WE CALL serialWrite(char[] c) which adds our data to the character array using
	appendSerial(char c)
	
	THEN IT CHECKS IF UDR0 IS EMPTY, IF IT IS IT WILL TRIGGER THE INTERRUPT BY DOING: UDR0 = 0;
	
	IN THE INTERRUPT WE KEEP SENDING DATA UNTIL OUR READPOS = WRITEPOS
	
	DONT WORRY ABOUT OUR ARRAY RUNNING OUT OF SPACE BECAUSE IT JUST LOOPS BACK TO THE BEGINNING
	
	SAMPLE:
	[P][L][E][\][N][\][R][][][][][][][][E][X][A][M]
	AND THIS WILL SEND CORRECTLY SINCE OUR READPOS = INDEXOF(E) AND ADD UNTIL IT LOOPS AND GETS TO INDEXOF(R)
				
*//************************************************************************/


void appendSerial(char c){
	serialBuffer[serialWritePos] = c;
	serialWritePos ++;
	
	if(serialWritePos >= TX_BUFFER_SIZE){
		serialWritePos = 0;
	}
}

void serialWrite(char c[]){
	
	for(uint8_t i = 0; i < strlen(c); i++){
		appendSerial(c[i]);
	}
	
	if(UCSR0A & (1 << UDRE0)){//this checks if the data register is empty
		//this forces the interrupt to fire by sending a 0
		UDR0 = 0;
	}
	
}


ISR(USART_TX_vect){
	if(serialReadPos != serialWritePos){
		UDR0 = serialBuffer[serialReadPos];
		serialReadPos ++;
		
		
		if(serialReadPos >= TX_BUFFER_SIZE){
			serialReadPos = 0;
		}
		
		
	}
}


