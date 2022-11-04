/*
 * SWITCH.h
 *
 * Created: 4/1/2022 11:43:35 AM
 *  Author: MaxBayne
 
 #include "Drivers/SWITCH/SWITCH.h"
 
 SWITCH_Setup(PortB,Pin0,SWITCH_PULLUP);
 
 if(SWITCH_Status(PortB,Pin0,SWITCH_PULLUP)==ON)
 {
	 LED_On(PortA,Pin0);
 }
 else
 {
	 LED_Off(PortA,Pin0);
 }
 
 
 */ 


#ifndef SWITCH_H_
#define SWITCH_H_

//enum Definitions -------------------------------------------------

typedef enum
{
	ON,
	OFF
}SwitchStatus;

typedef enum
{
	//Default Status is LOW
	SWITCH_PULLDOWN,
	
	//Default Status is High
	SWITCH_PULLUP
}SwitchType;


//Methods ----------------------------------------------------------

//Setup Switch as INPUT or PULLUP
void SWITCH_Setup(PORT port , PIN pin,SwitchType type);

//Read Switch Status ON,OFF
SwitchStatus SWITCH_Status(PORT port,PIN pin,SwitchType type);


#endif /* SWITCH_H_ */