/*
 * LED.h
 *
 * Created: 4/1/2022 11:11:55 AM
 *  Author: MaxBayne
 
 #include "Drivers/LED/LED.h"
 
 LED_Setup(PortA,Pin0);
 
 if(SWITCH_Status(PortB,Pin0,SWITCH_PULLUP)==ON)
 {
	 LED_On(PortA,Pin0);
 }
 else
 {
	 LED_Off(PortA,Pin0);
 }
 
 */ 


#ifndef LED_H_
#define LED_H_

//Set Direction For Led Pin as Output and LOW as Default Status
void LED_Setup(PORT port,PIN pin);

//Set Direction For Led Pin as Output with Default Value
void LED_Setup_With_Value(PORT port,PIN pin,VALUE value);

//Switch ON Led by write HIGH
void LED_On(PORT port,PIN pin);

//Switch OFF Led by write LOW
void LED_Off(PORT port,PIN pin);

#endif /* LED_H_ */