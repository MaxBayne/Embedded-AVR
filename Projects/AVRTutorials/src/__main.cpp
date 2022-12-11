#define  F_CPU 16000000UL


#include <avr/io.h>
#include <util/delay.h>


int __main(void)
{
	DDRA = 0xFF;
	
    /* Replace with your application code */
    while (1) 
    {
		PORTA=0x00;
		_delay_ms(1000);
		PORTA=0b00000101;
		_delay_ms(1000);
	
    }
}

