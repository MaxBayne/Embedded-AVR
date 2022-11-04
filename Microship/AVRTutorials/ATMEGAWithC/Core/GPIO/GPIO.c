#include <avr/io.h>

#include "GPIO.h"


void Port_Direction(PORT port,DIRECTION direction)
{
	// Add 1 to go from PIN To DDR Register Address
	
	if(direction==INPUT)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+1)) = 0x00; break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+1)) = 0x00; break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+1)) = 0x00; break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+1)) = 0x00; break;
		}
	}
	else if(direction==OUTPUT)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+1)) = 0xFF; break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+1)) = 0xFF; break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+1)) = 0xFF; break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+1)) = 0xFF; break;
		}
	}
	else if(direction==PULLUP)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+1)) = 0x00; (*(volatile unsigned char *) (PORTA_ADDRESS+2)) = 0xFF; break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+1)) = 0x00; (*(volatile unsigned char *) (PORTB_ADDRESS+2)) = 0xFF; break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+1)) = 0x00; (*(volatile unsigned char *) (PORTC_ADDRESS+2)) = 0xFF; break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+1)) = 0x00; (*(volatile unsigned char *) (PORTD_ADDRESS+2)) = 0xFF; break;
		}
	}
}
void Port_Write_Value(PORT port, unsigned char value)
{
	// Add 2 to go from PIN To PORT Register Address
	switch (port)
	{
		case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) = value; break;
		case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) = value; break;
		case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) = value; break;
		case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) = value; break;
	}
}
void Port_Write(PORT port, VALUE value)
{
	// Add 2 to go from PIN To PORT Register Address
	
	if(value==LOW)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) = 0x00; break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) = 0x00; break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) = 0x00; break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) = 0x00; break;
		}
	}
	else if(value==HIGH)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) = 0xFF; break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) = 0xFF; break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) = 0xFF; break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) = 0xFF; break;
		}
	}
	else if(value==TOGGLE)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) ^= 0xFF; break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) ^= 0xFF; break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) ^= 0xFF; break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) ^= 0xFF; break;
		}
	}
	
}
char Port_Read(PORT port)
{
	switch (port)
	{
		case PortA: return (*(volatile unsigned char *) PORTA_ADDRESS) ; break;
		case PortB: return (*(volatile unsigned char *) PORTB_ADDRESS) ; break;
		case PortC: return (*(volatile unsigned char *) PORTC_ADDRESS) ; break;
		case PortD: return (*(volatile unsigned char *) PORTD_ADDRESS) ; break;
	}
}

void Pin_Direction(PORT port,PIN pin,DIRECTION direction)
{
	// Add 1 to go from PIN To DDR Register Address
	// Add 2 to go from PIN To PORT Register Address
	
	if(direction==INPUT)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+1))  &=~(1<<pin); break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+1))  &=~(1<<pin); break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+1))  &=~(1<<pin); break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+1))  &=~(1<<pin); break;
		}
	}
	else if(direction==OUTPUT)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+1)) |=(1<<pin); break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+1)) |=(1<<pin); break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+1)) |=(1<<pin); break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+1)) |=(1<<pin); break;
		}
	}
	else if(direction==PULLUP)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+1)) &=~(1<<pin); (*(volatile unsigned char *) (PORTA_ADDRESS+2)) |=(1<<pin); break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+1)) &=~(1<<pin); (*(volatile unsigned char *) (PORTB_ADDRESS+2)) |=(1<<pin); break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+1)) &=~(1<<pin); (*(volatile unsigned char *) (PORTC_ADDRESS+2)) |=(1<<pin); break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+1)) &=~(1<<pin); (*(volatile unsigned char *) (PORTD_ADDRESS+2)) |=(1<<pin); break;
		}
	}
}
void Pin_Write(PORT port,PIN pin, VALUE value)
{
	// Add 2 to go from PIN To PORT Register Address
	
	if(value==LOW)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) &=~(1<<pin);break; 
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) &=~(1<<pin);break;  
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) &=~(1<<pin);break;  
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) &=~(1<<pin);break;  
		}
		
		
	}
	else if(value==HIGH)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) |=(1<<pin);break;
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) |=(1<<pin);break;
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) |=(1<<pin);break;
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) |=(1<<pin);break;
		}
		
	
	}
	else if(value==TOGGLE)
	{
		switch (port)
		{
			case PortA: (*(volatile unsigned char *) (PORTA_ADDRESS+2)) ^=(1<<pin);break;  
			case PortB: (*(volatile unsigned char *) (PORTB_ADDRESS+2)) ^=(1<<pin);break;  
			case PortC: (*(volatile unsigned char *) (PORTC_ADDRESS+2)) ^=(1<<pin);break;  
			case PortD: (*(volatile unsigned char *) (PORTD_ADDRESS+2)) ^=(1<<pin);break;  
		}
	}
	
}
VALUE Pin_Read(PORT port,PIN pin)
{
	char result;
	switch (port)
	{
		//case PortA: result=(*(volatile unsigned char *) PORTA_ADDRESS) & (1<<pin); break;
		case PortB: result=(*(volatile unsigned char *) PORTB_ADDRESS) & (1<<pin); break;
		case PortC: result=(*(volatile unsigned char *) PORTC_ADDRESS) & (1<<pin); break;
		case PortD: result=(*(volatile unsigned char *) PORTD_ADDRESS) & (1<<pin); break;
	}
	
	if(result==1)
	{
		return HIGH;
	}
	else if(result==0)
	{
		return LOW;
	}
	else
	{
		return UnKnown;
	}
}

