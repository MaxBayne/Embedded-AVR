#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <util/delay.h>

int PullUp_main(void)
{
	//Setup ---------------
	
	//Pull-Up (Mean use Internal Resistor For Pin and Default Status is High So when Status is Ground Mean Switch is Pressed)
	
												//Define Pull up Switch --------------
	DDRD &= ~(1<<DDD1);							//Define Pin 1 on Port D as Input [0] [&] [AND]
	PORTD &= ~(1<<PORTD1);						//Set Pin 1 on Port D as High [1] [|] [OR]
	
												//Define LED -------------
	DDRD |= (1<<DDD0);							//Define Pin 0 on Port D as Output [1] [|] [OR]
	PORTD &= ~(1<<PORTD0);						//RESET Pin 0 on Port D as LOW [0] [&] [AND]
												
	//Loop -----------------
	
	while (1)
	{
		if(PIND & (1<<PIND1))					//Check Status of SWitch (PD1)
		{
												//On Button Released LED OFF
			PORTD &= ~(1<<PORTD0);				//RESET Pin 0 on Port D as LOW [0] [&] [AND]
		}
		else
		{
												//On Button Pressed LED ON
			_delay_ms(15);						//To Avoid DeBouncing and let user release the button
			PORTD |= (1<<PORTD0);				//SET Pin 0 on Port D as High [1] [|] [OR]
		}
	
	}
}



