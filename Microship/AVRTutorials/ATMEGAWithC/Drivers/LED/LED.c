/*
 * LED.c
 *
 * Created: 4/1/2022 11:12:06 AM
 *  Author: MaxBayne
 */ 

#include "GPIO.h"
#include "LED.h"

//Set Direction For Led Pin as Output and LOW as Default Status
void LED_Setup(PORT port,PIN pin)
{
	//Set Direction For Pin as OUTPUT
	Pin_Direction(port,pin,OUTPUT);
	
	//Switch Led for Default Value
	Pin_Write(port,pin,LOW);
}

//Set Direction For Led Pin as Output with Default Value
void LED_Setup_With_Value(PORT port,PIN pin,VALUE value)
{
	//Set Direction For Pin as OUTPUT
	Pin_Direction(port,pin,OUTPUT);
	
	//Switch Led for Default Value
	Pin_Write(port,pin,value);
}



//Switch ON Led by write HIGH
void LED_On(PORT port,PIN pin)
{
	//Switch Led ON
	Pin_Write(port,pin,HIGH);
}

//Switch OFF Led by write LOW
void LED_Off(PORT port,PIN pin)
{
	//Switch Led ON
	Pin_Write(port,pin,LOW);
}