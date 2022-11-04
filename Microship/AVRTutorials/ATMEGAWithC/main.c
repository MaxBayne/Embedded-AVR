#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ


#include <avr/io.h>
#include "Core/Common/Common.h"
#include "Core/GPIO/GPIO.h"
#include "Core/Interrupts/ExternalInterrupt.h"



void RUN_BUZZER();

int main(void)
{
	
	//Setup ---------------
	
	//LED Green Output
	//Pin_Direction(PortB,Pin0,OUTPUT);
	
	//LED Red Output
	//Pin_Direction(PortB,Pin1,OUTPUT);
	
	//LED Yellow Output
	//Pin_Direction(PortB,Pin2,OUTPUT);
	
	//Buzzer Output
	//Pin_Direction(PortB,Pin3,OUTPUT);
	
	Pin_Direction(PortA,Pin0,OUTPUT);
	
	//Port B as Output For 8-Led
	Port_Direction(PortB,OUTPUT);
	
	//Button Input
	Pin_Direction(PortD,Pin2,PULLUP);
	
	//Config External Interrupt Over INT0
	EXTI_Initialize(EXTI_Pin_INT0,EXTI_TriggerMode_FallingEdge,RUN_BUZZER);
	
	uint8 counter=0;
	
	//Loop -----------------
	while (1)
	{
		
		counter++;
		
		Port_Write_Value(PortB,counter);
		
		delay_ms(1000);
	}
	
}

void RUN_BUZZER()
{
	
}
