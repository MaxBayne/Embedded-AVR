//#include <avr/io.h>

#include "Core/Common/Common.h"
#include "Core/Common/Registers.h"
#include "Core/GPIO/GPIO.h"
//#include "Core/Interrupts/ExternalInterrupt.h"



void toggleLEDYellow()
{
	GPIO::Pin_Toggle(GPIO_IO_PA0);
}

int main(void)
{
	
	GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);
	GPIO::Port_Direction(GPIO_PORT_B,GPIO_DIRECTION_OUTPUT);
	
	GPIO::Pin_Set(GPIO_IO_PD2);
	
	//ExternalInterrupt ExternalINT0(EXTI_PIN_INT0,EXTI_TRIGGER_MODE_FALLING_EDGE,&toggleLEDYellow);
	

	
	uint8 counter=0;
    while (1) 
    {
		counter++;
		GPIO::Port_Write(GPIO_PORT_B,counter);
		delay_ms(1000);
    }
}


	