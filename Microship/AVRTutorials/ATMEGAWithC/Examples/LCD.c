#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ


#include <avr/io.h>
//#include "Core/Common/Common.h"
//#include "Core/GPIO/GPIO.h"
//#include "Drivers/LCD/LCD.h"

int LCD_main(void)
{
	
	//Setup ---------------
	
	//LCD_Initialize(EightBIT,PortC,Pin0,Pin1,Pin2,PortD,Pin0,Pin1,Pin2,Pin3,Pin4,Pin5,Pin6,Pin7);
	
	
	//LCD_Go_To_Position(0,5);
	
	//LCD_Write_Text_With_Delay("Hello World");
	
	delay_ms(1000);
	
	
	//LCD_Command(CMD_DISPLAY_CLEAR);
	
	//LCD_Go_To_Position(1,5);
	
	//LCD_Write_Text_With_Delay("Mohammed Salah");
	
	
	//Loop -----------------
	while (1)
	{
		
		
		
	}
	
}

