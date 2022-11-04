/*
 * SEVENSEGEMENT.h
 *
 * Created: 4/2/2022 2:33:24 PM
 *  Author: MaxBayne
 
 #include "Drivers/SEVENSEGMENT/SEVENSEGEMENT.h"
 
 //SEVENSEGMENT_Setup(PortA,Pin4,Pin5,Pin6,Pin7,PortB,Pin6,Pin7);
 
 //SEVENSEGMENT_Write_Value(PortA,Pin4,Pin5,Pin6,Pin7,PortB,Pin6,7);
 
 */ 

#include "Common.h"
#include "GPIO.h"


#ifndef SEVENSEGEMENT_H_
#define SEVENSEGEMENT_H_

//enum Definitions -------------------------------------------------

typedef enum
{
	Number0,
	Number1,
	Number2,
	Number3,
	Number4,
	Number5,
	Number6,
	Number7,
	Number8,
	Number9
}SEVEN_NUMBER;

//Methods Operations ---------------------------------------------------

//Setup Seven Segment powered by Decoder
void SEVENSEGMENT_Setup(PORT portData,PIN pinA,PIN pinB,PIN pinC,PIN pinD,PORT portEnable,PIN enable1,PIN enable2);

//Enable or Disable SeventSegment using Enable Port,Pin
void SEVENSEGMENT_Control(PORT portEnable,PIN pinEnable,VALUE value);

//Write Number To Seven Segment [0:9]
void SEVENSEGMENT_Write(PORT portData,PIN pinA,PIN pinB,PIN pinC,PIN pinD,PORT portEnable,PIN pinEnable,SEVEN_NUMBER number);

//Write Number To Seven Segment [0:9]
void SEVENSEGMENT_Write_Value(PORT portData,PIN pinA,PIN pinB,PIN pinC,PIN pinD,PORT portEnable,PIN pinEnable,uint8 value);


#endif /* SEVENSEGEMENT_H_ */