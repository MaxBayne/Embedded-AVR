/*
 * SWITCH.c
 *
 * Created: 4/1/2022 11:43:41 AM
 *  Author: MaxBayne
 
 
 */ 
#include "GPIO.h"
#include "SWITCH.h"


//Setup Switch as INPUT or PULLUP
void SWITCH_Setup(PORT port , PIN pin,SwitchType type)
{
	if(type==SWITCH_PULLUP)
	{
		Pin_Direction(port,pin,PULLUP);
	}
	else if(type==SWITCH_PULLDOWN)
	{
		Pin_Direction(port,pin,INPUT);
	}
}

//Read Switch Status ON,OFF
SwitchStatus SWITCH_Status(PORT port,PIN pin,SwitchType type)
{
	VALUE val = Pin_Read(port,pin);
	
	if(type==SWITCH_PULLDOWN)
	{
		if(val==HIGH)
		{
			return ON;
		}
		else
		{
			return OFF;
		}
		
	}
	else if(type==SWITCH_PULLUP)
	{
		if(val==HIGH)
		{
			return OFF;
		}
		else
		{
			return ON;
		}
	}
	
}