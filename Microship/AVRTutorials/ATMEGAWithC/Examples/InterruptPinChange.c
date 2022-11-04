#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int sharedVariableBetweenInterrupts;

int InterruptPinChange_main(void)
{
	//Setup ---------------
	
	//Pin Interrupt Change Over Pins PCINT0:PCINT23 On Every Change Status
	//--------------------------------------------------------------------
	
	
	//1) Enable Pin Change Interrupt using Register PCICR (Pin Change Interrupt Control Register)
	
	//PCICR |= (1<<PCIE0);							//Enable Pin Change Interrupt For (PCINT0:PCINT7) Or Use Mask To Special Select
	//PCICR |= (1<<PCIE1);							//Enable Pin Change Interrupt For (PCINT8:PCINT14) Or Use Mask To Special Select
	//PCICR |= (1<<PCIE2);							//Enable Pin Change Interrupt For (PCINT16:PCINT23) Or Use Mask To Special Select
	
	//2) Using Mask To Select Special Pins For Pin Change Interrupt and using Register PCMSK (Pin Change Mask)
	
	//PCMSK0 |= (1<<PCINT0);							//Enable PCINT0 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT1);							//Enable PCINT1 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT2);							//Enable PCINT2 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT3);							//Enable PCINT3 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT4);							//Enable PCINT4 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT5);							//Enable PCINT5 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT6);							//Enable PCINT6 For Pin Change Interrupt
	//PCMSK0 |= (1<<PCINT7);							//Enable PCINT7 For Pin Change Interrupt
	
	//PCMSK1 |= (1<<PCINT8);							//Enable PCINT8 For Pin Change Interrupt
	//PCMSK1 |= (1<<PCINT9);							//Enable PCINT9 For Pin Change Interrupt
	//PCMSK1 |= (1<<PCINT10);							//Enable PCINT10 For Pin Change Interrupt
	//PCMSK1 |= (1<<PCINT11);							//Enable PCINT11 For Pin Change Interrupt
	//PCMSK1 |= (1<<PCINT12);							//Enable PCINT12 For Pin Change Interrupt
	//PCMSK1 |= (1<<PCINT13);							//Enable PCINT13 For Pin Change Interrupt
	//PCMSK1 |= (1<<PCINT14);							//Enable PCINT14 For Pin Change Interrupt
	
	//PCMSK2 |= (1<<PCINT16);							//Enable PCINT16 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT17);							//Enable PCINT17 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT18);							//Enable PCINT18 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT19);							//Enable PCINT19 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT20);							//Enable PCINT20 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT21);							//Enable PCINT21 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT22);							//Enable PCINT22 For Pin Change Interrupt
	//PCMSK2 |= (1<<PCINT23);							//Enable PCINT23 For Pin Change Interrupt
	
	
	//3) Enable Global Interrupt inside Register (SREG) Bit7 (I)
	sei();										//Enable Global Interrupt
	//cli();									//Disable Global Interrupt [Default]
	
	//4) Check if Interrupt Occurred by check value of Register [PCIFR] (Pin Change Interrupt Flag Register)
	//PCIF0										//Pin Change Interrupt Fired For (PCINT0:PCINT7)
	//PCIF1										//Pin Change Interrupt Fired For (PCINT8:PCINT14)
	//PCIF2										//Pin Change Interrupt Fired For (PCINT16:PCINT23)
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
												//Define LED1 -------------
	DDRD |= (1<<DDD0);							//Define Pin 0 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD0);						//RESET Pin 0 on Port D as LOW [0] [&] [AND]
	
  												//Define LED2 -------------
	DDRD |= (1<<DDD1);							//Define Pin 1 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD1);						//RESET Pin 1 on Port D as LOW [0] [&] [AND]
	
												//Define B0 as [Pull-Up]
	DDRB &= ~(1<<DDB0);							//Define Pin 0 on Port B as Input [0] [&] [AND]
	//PORTB |= (1<<PORTB0);						//Set Pin 0 on Port B as HIGH [1] [|] [OR]
	
												//Define C0 as [Pull-Up]
	DDRC &= ~(1<<DDC0);							//Define Pin 0 on Port C as Input [0] [&] [AND]
	//PORTC |= (1<<PORTC0);						//Set Pin 0 on Port C as HIGH [1] [|] [OR]
	

												//Define D7 as [Pull-Up]
	DDRD &= ~(1<<DDD7);							//Define Pin 7 on Port D as Input [0] [&] [AND]
	PORTD |= (1<<PORTD7);						//Set Pin 7 on Port D as HIGH [1] [|] [OR]
												
	
	
	//Loop -----------------
	
	while (1)
	{
	
	}
	
}

//Interrupt Service For Pin Change Interrupt (PCINT0:PCINT7)
ISR (PCINT0_vect)
{
	if((PINB &(1<<PINB0))==1)
	{
		//On HIGH 
		//PORTD &= ~(1<<PORTD0);						
	}
	else
	{
		//ON LOW
		//PORTD |= (1<<PORTD0);						
	}
	
	
}

//Interrupt Service For Pin Change Interrupt (PCINT8:PCINT14)
ISR (PCINT1_vect)
{
	if((PINC &(1<<PINC0))==1)
	{
		//On HIGH
		//PORTD &= ~(1<<PORTD1);
	}
	else
	{
		//ON LOW
		//PORTD |= (1<<PORTD1);
	}
}

//Interrupt Service For Pin Change Interrupt (PCINT16:PCINT23)
ISR (PCINT2_vect)
{
	if((PIND &(1<<PIND7))==0)
	{
		//PORTD ^= (1<<PORTD0);
		//PORTD ^= (1<<PORTD1);
	}
							
}

