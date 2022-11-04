#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int sharedVariableBetweenInterrupts;

int InterruptExternal_main(void)
{
	//Setup ---------------
	
	//External Interrupt Request Over Pin [D2 (INT0),D3 (INT1)]
	//-------------------------------------------------
	
	//1) Define Interrupt Sense Triggered Type [Only LOW - Any Change - Falling Edge - Rising Edge]
	
	//EICRA |= ~(1<<ISC00) | ~(1<<ISC01);			//Set ISC00=0, ISC01=0 To Fire Interrupt Only LOW Over INT0
	//EICRA |=  (1<<ISC00) | ~(1<<ISC01);			//Set ISC00=1, ISC01=0 To Fire Interrupt Any Change Over INT0
	//EICRA |= ~(1<<ISC00) |  (1<<ISC01);			//Set ISC00=0, ISC01=1 To Fire Interrupt With Falling Edge Over INT0
	//EICRA |=  (1<<ISC00) |  (1<<ISC01);			//Set ISC00=1, ISC01=1 To Fire Interrupt With Rising Edge Over INT0
	
	
	//EICRA |= ~(1<<ISC10) | ~(1<<ISC11);			//Set ISC10=0, ISC11=0 To Fire Interrupt Only LOW Over INT1
	//EICRA |=  (1<<ISC10) | ~(1<<ISC11);			//Set ISC10=1, ISC11=0 To Fire Interrupt Any Change Over INT1
	//EICRA |= ~(1<<ISC10) |  (1<<ISC11);			//Set ISC10=0, ISC11=1 To Fire Interrupt With Falling Edge Over INT1
	//EICRA |=  (1<<ISC10) |  (1<<ISC11);			//Set ISC10=1, ISC11=1 To Fire Interrupt With Rising Edge Over INT1
	
	
	
	//2) Enable External Interrupt INT0,INT1 inside Register (EIMSK) Set Bit0[INT0] =1 For Enable (INT0) & Set Bit1[INT1]=1 For Enable (INT1)
	//EIMSK |= (1<<INT0);							//Enable External Interrupt For INT0 [D2]
	//EIMSK |= (1<<INT1);							//Enable External Interrupt For INT1 [D3]
	
	
	//3) Enable Global Interrupt inside Register (SREG) Bit7 (I)
	sei();										//Enable Global Interrupt
	//cli();									//Disable Global Interrupt [Default]
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
												//Define LED1 -------------
	DDRD |= (1<<DDD0);							//Define Pin 0 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD0);						//RESET Pin 0 on Port D as LOW [0] [&] [AND]
	
  												//Define LED2 -------------
	DDRD |= (1<<DDD1);							//Define Pin 1 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD1);						//RESET Pin 1 on Port D as LOW [0] [&] [AND]
	
	
												
	
	
	//Loop -----------------
	
	while (1)
	{
	
	}
	
}
/*
//Interrupt Service For INT0 (External Interrupt Request Over Pin D2)
ISR(INT0_vect)
{
	
	//PORTD ^= (1<<PORTD0);						//Reverse D0 Status
}

//Interrupt Service For INT1 (External Interrupt Request Over Pin D3)
ISR(INT1_vect)
{
	//PORTD ^= (1<<PORTD1);						//Reverse D1 Status
}

*/