#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <util/delay.h>

int Flasher_main(void)
{
	//Setup ---------------
	
	
	DDRD |= (1<<DDD0);							//Define Pin 0 on Port D as Output [1] [|] [OR]
	
	
	PORTD &= ~(1<<PORTD0);						//RESET Pin 0 on Port D as LOW [0] [&] [AND]
	
		
	//Loop -----------------
	
	while (1)
	{
		PORTD ^= (1<<PORTD0);					//Reverse Status On Pin 0 of Port D (PD0)
		
		_delay_ms(500);							//Delay 500 MiliSeconds
	
	}
}



