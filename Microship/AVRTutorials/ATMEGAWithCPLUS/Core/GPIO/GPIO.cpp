//#include <avr/io.h>

#include "Common/Registers.h"
#include "GPIO.h"


#pragma region Port Methods

void GPIO::Port_Direction(GPIO_PORT port,GPIO_DIRECTION direction)
{
	// Add 1 to go from GPIO_PIN To DDR Register Address
	
	if(direction==GPIO_DIRECTION_INPUT)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+1)) = 0x00; break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+1)) = 0x00; break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+1)) = 0x00; break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+1)) = 0x00; break;
		}
	}
	else if(direction==GPIO_DIRECTION_OUTPUT)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+1)) = 0xFF; break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+1)) = 0xFF; break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+1)) = 0xFF; break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+1)) = 0xFF; break;
		}
	}
	else if(direction==GPIO_DIRECTION_PULLUP)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+1)) = 0x00; (*(volatile unsigned char *) (IO_REG_PORTA+2)) = 0xFF; break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+1)) = 0x00; (*(volatile unsigned char *) (IO_REG_PORTB+2)) = 0xFF; break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+1)) = 0x00; (*(volatile unsigned char *) (IO_REG_PORTC+2)) = 0xFF; break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+1)) = 0x00; (*(volatile unsigned char *) (IO_REG_PORTD+2)) = 0xFF; break;
		}
	}
}

void GPIO::Port_Write(GPIO_PORT port, unsigned char value)
{
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	switch (port)
	{
		case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) = value; break;
		case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) = value; break;
		case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) = value; break;
		case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) = value; break;
	}
}

void GPIO::Port_Write(GPIO_PORT port, GPIO_VALUE value)
{
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	
	if(value==GPIO_VALUE_LOW)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) = 0x00; break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) = 0x00; break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) = 0x00; break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) = 0x00; break;
		}
	}
	else if(value==GPIO_VALUE_HIGH)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) = 0xFF; break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) = 0xFF; break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) = 0xFF; break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) = 0xFF; break;
		}
	}
	else if(value==GPIO_VALUE_TOGGLE)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^= 0xFF; break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^= 0xFF; break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^= 0xFF; break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^= 0xFF; break;
		}
	}
	
}

char GPIO::Port_Read(GPIO_PORT port)
{
	switch (port)
	{
		case GPIO_PORT_A: return (*(volatile unsigned char *) IO_REG_PORTA) ; break;
		case GPIO_PORT_B: return (*(volatile unsigned char *) IO_REG_PORTB) ; break;
		case GPIO_PORT_C: return (*(volatile unsigned char *) IO_REG_PORTC) ; break;
		case GPIO_PORT_D: return (*(volatile unsigned char *) IO_REG_PORTD) ; break;
	}
}

#pragma endregion Port Methods

#pragma region Pin Methods

void GPIO::Pin_Direction(GPIO_PORT port,GPIO_PIN pin,GPIO_DIRECTION direction)
{
	// Add 1 to go from GPIO_PIN To DDR Register Address
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	
	if(direction==GPIO_DIRECTION_INPUT)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<pin); break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<pin); break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<pin); break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<pin); break;
		}
	}
	else if(direction==GPIO_DIRECTION_OUTPUT)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+1)) |=(1<<pin); break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+1)) |=(1<<pin); break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+1)) |=(1<<pin); break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+1)) |=(1<<pin); break;
		}
	}
	else if(direction==GPIO_DIRECTION_PULLUP)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+1)) &=~(1<<pin); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<pin); break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+1)) &=~(1<<pin); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<pin); break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+1)) &=~(1<<pin); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<pin); break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+1)) &=~(1<<pin); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<pin); break;
		}
	}
}

void GPIO::Pin_Direction(GPIO_IO pin,GPIO_DIRECTION direction)
{
	// Add 1 to go from GPIO_PIN To DDR Register Address
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	
	if(direction==GPIO_DIRECTION_INPUT)
	{
		switch (pin)
		{
			case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<0); break;
			case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<1); break;
			case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<2); break;
			case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<3); break;
			case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<4); break;
			case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<5); break;
			case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<6); break;
			case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<7); break;
			
			case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<0); break;
			case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<1); break;
			case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<2); break;
			case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<3); break;
			case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<4); break;
			case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<5); break;
			case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<6); break;
			case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<7); break;
			
			case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<0); break;
			case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<1); break;
			case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<2); break;
			case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<3); break;
			case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<4); break;
			case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<5); break;
			case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<6); break;
			case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<7); break;
			
			case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<0); break;
			case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<1); break;
			case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<2); break;
			case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<3); break;
			case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<4); break;
			case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<5); break;
			case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<6); break;
			case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<7); break;
		}
	}
	else if(direction==GPIO_DIRECTION_OUTPUT)
	{
		switch (pin)
		{
			case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<0); break;
			case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<1); break;
			case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<2); break;
			case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<3); break;
			case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<4); break;
			case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<5); break;
			case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<6); break;
			case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+1))  |=(1<<7); break;
			
			case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<0); break;
			case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<1); break;
			case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<2); break;
			case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<3); break;
			case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<4); break;
			case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<5); break;
			case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<6); break;
			case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+1))  |=(1<<7); break;
			
			case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<0); break;
			case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<1); break;
			case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<2); break;
			case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<3); break;
			case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<4); break;
			case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<5); break;
			case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<6); break;
			case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+1))  |=(1<<7); break;
			
			case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<0); break;
			case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<1); break;
			case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<2); break;
			case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<3); break;
			case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<4); break;
			case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<5); break;
			case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<6); break;
			case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+1))  |=(1<<7); break;
		}
		
		
	}
	else if(direction==GPIO_DIRECTION_PULLUP)
	{
		switch (pin)
		{
			case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<0); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<0);  break;
			case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<1); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<1);  break;
			case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<2); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<2);  break;
			case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<3); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<3);  break;
			case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<4); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<4);  break;
			case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<5); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<5);  break;
			case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<6); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<6);  break;
			case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+1))  &=~(1<<7); (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<7);  break;
			
			case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<0); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<0);  break;
			case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<1); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<1);  break;
			case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<2); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<2);  break;
			case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<3); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<3);  break;
			case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<4); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<4);  break;
			case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<5); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<5);  break;
			case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<6); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<6);  break;
			case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+1))  &=~(1<<7); (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<7);  break;
			
			case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<0); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<0);  break;
			case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<1); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<1);  break;
			case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<2); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<2);  break;
			case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<3); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<3);  break;
			case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<4); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<4);  break;
			case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<5); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<5);  break;
			case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<6); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<6);  break;
			case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+1))  &=~(1<<7); (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<7);  break;
			
			case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<0); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<0);  break;
			case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<1); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<1);  break;
			case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<2); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<2);  break;
			case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<3); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<3);  break;
			case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<4); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<4);  break;
			case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<5); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<5);  break;
			case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<6); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<6);  break;
			case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+1))  &=~(1<<7); (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<7);  break;
		}
		
	}

}


void GPIO::Pin_Write(GPIO_PORT port,GPIO_PIN pin, GPIO_VALUE value)
{
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	
	if(value==GPIO_VALUE_LOW)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<pin);break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<pin);break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<pin);break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<pin);break;
		}
		
		
	}
	else if(value==GPIO_VALUE_HIGH)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<pin);break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<pin);break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<pin);break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<pin);break;
		}
		
		
	}
	else if(value==GPIO_VALUE_TOGGLE)
	{
		switch (port)
		{
			case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<pin);break;
			case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<pin);break;
			case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<pin);break;
			case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<pin);break;
		}
	}
	
}

void GPIO::Pin_Write(GPIO_IO pin,GPIO_VALUE value)
{
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	
	if(value==GPIO_VALUE_LOW)
	{
		switch (pin)
		{
			case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<0);break;
			case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<1);break;
			case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<2);break;
			case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<3);break;
			case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<4);break;
			case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<5);break;
			case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<6);break;
			case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<7);break;
			
			case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<0);break;
			case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<1);break;
			case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<2);break;
			case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<3);break;
			case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<4);break;
			case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<5);break;
			case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<6);break;
			case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<7);break;
			
			case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<0);break;
			case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<1);break;
			case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<2);break;
			case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<3);break;
			case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<4);break;
			case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<5);break;
			case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<6);break;
			case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<7);break;
			
			case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<0);break;
			case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<1);break;
			case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<2);break;
			case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<3);break;
			case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<4);break;
			case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<5);break;
			case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<6);break;
			case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<7);break;
		}
		
		
	}
	else if(value==GPIO_VALUE_HIGH)
	{
		switch (pin)
		{
			case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<0);break;
			case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<1);break;
			case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<2);break;
			case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<3);break;
			case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<4);break;
			case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<5);break;
			case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<6);break;
			case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<7);break;
			
			case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<0);break;
			case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<1);break;
			case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<2);break;
			case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<3);break;
			case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<4);break;
			case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<5);break;
			case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<6);break;
			case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<7);break;
			
			case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<0);break;
			case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<1);break;
			case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<2);break;
			case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<3);break;
			case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<4);break;
			case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<5);break;
			case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<6);break;
			case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<7);break;
			
			case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<0);break;
			case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<1);break;
			case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<2);break;
			case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<3);break;
			case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<4);break;
			case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<5);break;
			case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<6);break;
			case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<7);break;
		}
		
		
	}
	else if(value==GPIO_VALUE_TOGGLE)
	{
		switch (pin)
		{
			case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<0);break;
			case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<1);break;
			case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<2);break;
			case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<3);break;
			case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<4);break;
			case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<5);break;
			case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<6);break;
			case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<7);break;
			
			case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<0);break;
			case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<1);break;
			case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<2);break;
			case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<3);break;
			case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<4);break;
			case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<5);break;
			case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<6);break;
			case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<7);break;
			
			case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<0);break;
			case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<1);break;
			case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<2);break;
			case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<3);break;
			case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<4);break;
			case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<5);break;
			case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<6);break;
			case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<7);break;
			
			case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<0);break;
			case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<1);break;
			case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<2);break;
			case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<3);break;
			case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<4);break;
			case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<5);break;
			case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<6);break;
			case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<7);break;
		}
	}

}


void GPIO::Pin_Set(GPIO_PORT port,GPIO_PIN pin)
{
	// Add 2 to go from GPIO_PIN To GPIO_PORT Register Address
	
	switch (port)
	{
		case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<pin);break;
		case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<pin);break;
		case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<pin);break;
		case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<pin);break;
	}
	
}

void GPIO::Pin_Set(GPIO_IO pin)
{
	switch (pin)
	{
		case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<0);break;
		case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<1);break;
		case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<2);break;
		case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<3);break;
		case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<4);break;
		case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<5);break;
		case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<6);break;
		case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+2)) |=(1<<7);break;
		
		case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<0);break;
		case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<1);break;
		case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<2);break;
		case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<3);break;
		case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<4);break;
		case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<5);break;
		case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<6);break;
		case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+2)) |=(1<<7);break;
		
		case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<0);break;
		case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<1);break;
		case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<2);break;
		case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<3);break;
		case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<4);break;
		case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<5);break;
		case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<6);break;
		case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+2)) |=(1<<7);break;
		
		case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<0);break;
		case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<1);break;
		case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<2);break;
		case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<3);break;
		case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<4);break;
		case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<5);break;
		case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<6);break;
		case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+2)) |=(1<<7);break;
	}
}


void GPIO::Pin_Clear(GPIO_PORT port,GPIO_PIN pin)
{
	switch (port)
	{
		case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<pin);break;
		case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<pin);break;
		case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<pin);break;
		case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<pin);break;
	}
}

void GPIO::Pin_Clear(GPIO_IO pin)
{
	switch (pin)
	{
		case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<0);break;
		case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<1);break;
		case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<2);break;
		case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<3);break;
		case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<4);break;
		case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<5);break;
		case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<6);break;
		case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+2)) &=~(1<<7);break;
		
		case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<0);break;
		case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<1);break;
		case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<2);break;
		case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<3);break;
		case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<4);break;
		case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<5);break;
		case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<6);break;
		case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+2)) &=~(1<<7);break;
		
		case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<0);break;
		case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<1);break;
		case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<2);break;
		case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<3);break;
		case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<4);break;
		case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<5);break;
		case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<6);break;
		case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+2)) &=~(1<<7);break;
		
		case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<0);break;
		case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<1);break;
		case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<2);break;
		case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<3);break;
		case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<4);break;
		case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<5);break;
		case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<6);break;
		case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+2)) &=~(1<<7);break;
	}
	
}


void GPIO::Pin_Toggle(GPIO_PORT port,GPIO_PIN pin)
{
	switch (port)
	{
		case GPIO_PORT_A: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<pin);break;
		case GPIO_PORT_B: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<pin);break;
		case GPIO_PORT_C: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<pin);break;
		case GPIO_PORT_D: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<pin);break;
	}
}

void GPIO::Pin_Toggle(GPIO_IO pin)
{
	switch (pin)
	{
		case GPIO_IO_PA0: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<0);break;
		case GPIO_IO_PA1: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<1);break;
		case GPIO_IO_PA2: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<2);break;
		case GPIO_IO_PA3: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<3);break;
		case GPIO_IO_PA4: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<4);break;
		case GPIO_IO_PA5: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<5);break;
		case GPIO_IO_PA6: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<6);break;
		case GPIO_IO_PA7: (*(volatile unsigned char *) (IO_REG_PORTA+2)) ^=(1<<7);break;
		
		case GPIO_IO_PB0: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<0);break;
		case GPIO_IO_PB1: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<1);break;
		case GPIO_IO_PB2: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<2);break;
		case GPIO_IO_PB3: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<3);break;
		case GPIO_IO_PB4: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<4);break;
		case GPIO_IO_PB5: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<5);break;
		case GPIO_IO_PB6: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<6);break;
		case GPIO_IO_PB7: (*(volatile unsigned char *) (IO_REG_PORTB+2)) ^=(1<<7);break;
		
		case GPIO_IO_PC0: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<0);break;
		case GPIO_IO_PC1: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<1);break;
		case GPIO_IO_PC2: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<2);break;
		case GPIO_IO_PC3: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<3);break;
		case GPIO_IO_PC4: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<4);break;
		case GPIO_IO_PC5: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<5);break;
		case GPIO_IO_PC6: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<6);break;
		case GPIO_IO_PC7: (*(volatile unsigned char *) (IO_REG_PORTC+2)) ^=(1<<7);break;
		
		case GPIO_IO_PD0: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<0);break;
		case GPIO_IO_PD1: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<1);break;
		case GPIO_IO_PD2: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<2);break;
		case GPIO_IO_PD3: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<3);break;
		case GPIO_IO_PD4: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<4);break;
		case GPIO_IO_PD5: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<5);break;
		case GPIO_IO_PD6: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<6);break;
		case GPIO_IO_PD7: (*(volatile unsigned char *) (IO_REG_PORTD+2)) ^=(1<<7);break;
	}
}


GPIO_VALUE GPIO::Pin_Read(GPIO_PORT port,GPIO_PIN pin)
{
	char result;
	switch (port)
	{
		case GPIO_PORT_A: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<pin); break;
		case GPIO_PORT_B: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<pin); break;
		case GPIO_PORT_C: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<pin); break;
		case GPIO_PORT_D: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<pin); break;
	}
	
	if(result==1)
	{
		return GPIO_VALUE_HIGH;
	}
	else if(result==0)
	{
		return GPIO_VALUE_LOW;
	}
	else
	{
		return GPIO_VALUE_UnKnown;
	}
}

GPIO_VALUE GPIO::Pin_Read(GPIO_IO pin)
{
	char result;
	switch (pin)
	{
		case GPIO_IO_PA0: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<0); break;
		case GPIO_IO_PA1: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<1); break;
		case GPIO_IO_PA2: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<2); break;
		case GPIO_IO_PA3: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<3); break;
		case GPIO_IO_PA4: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<4); break;
		case GPIO_IO_PA5: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<5); break;
		case GPIO_IO_PA6: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<6); break;
		case GPIO_IO_PA7: result=(*(volatile unsigned char *) IO_REG_PORTA) & (1<<7); break;
		
		case GPIO_IO_PB0: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<0); break;
		case GPIO_IO_PB1: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<1); break;
		case GPIO_IO_PB2: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<2); break;
		case GPIO_IO_PB3: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<3); break;
		case GPIO_IO_PB4: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<4); break;
		case GPIO_IO_PB5: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<5); break;
		case GPIO_IO_PB6: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<6); break;
		case GPIO_IO_PB7: result=(*(volatile unsigned char *) IO_REG_PORTB) & (1<<7); break;
		
		case GPIO_IO_PC0: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<0); break;
		case GPIO_IO_PC1: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<1); break;
		case GPIO_IO_PC2: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<2); break;
		case GPIO_IO_PC3: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<3); break;
		case GPIO_IO_PC4: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<4); break;
		case GPIO_IO_PC5: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<5); break;
		case GPIO_IO_PC6: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<6); break;
		case GPIO_IO_PC7: result=(*(volatile unsigned char *) IO_REG_PORTC) & (1<<7); break;
		
		case GPIO_IO_PD0: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<0); break;
		case GPIO_IO_PD1: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<1); break;
		case GPIO_IO_PD2: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<2); break;
		case GPIO_IO_PD3: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<3); break;
		case GPIO_IO_PD4: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<4); break;
		case GPIO_IO_PD5: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<5); break;
		case GPIO_IO_PD6: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<6); break;
		case GPIO_IO_PD7: result=(*(volatile unsigned char *) IO_REG_PORTD) & (1<<7); break;
	}
	
	if(result==1)
	{
		return GPIO_VALUE_HIGH;
	}
	else if(result==0)
	{
		return GPIO_VALUE_LOW;
	}
	else
	{
		return GPIO_VALUE_UnKnown;
	}
}

#pragma endregion Pin Methods





