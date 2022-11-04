/*
 * SEVENSEGEMENT.c
 *
 * Created: 4/2/2022 2:33:10 PM
 *  Author: MaxBayne
 */ 

#include "SEVENSEGEMENT.h"

//Setup Seven Segment powered by Decoder
void SEVENSEGMENT_Setup(PORT portData,PIN pinA,PIN pinB,PIN pinC,PIN pinD,PORT portEnable,PIN enable1,PIN enable2)
{
	//Set Direction For Data Pins
	Pin_Direction(portData,pinA,OUTPUT);
	Pin_Direction(portData,pinB,OUTPUT);
	Pin_Direction(portData,pinC,OUTPUT);
	Pin_Direction(portData,pinD,OUTPUT);
	
	//Set Direction For Control Pins
	Pin_Direction(portEnable,enable1,OUTPUT);
	Pin_Direction(portEnable,enable2,OUTPUT);
	
}

//Enable or Disable SeventSegment using Enable Port,Pin
void SEVENSEGMENT_Control(PORT portEnable,PIN pinEnable,VALUE value)
{
	//Set High/Low for enable Pin to enable or disable
	Pin_Write(portEnable,pinEnable,value);
}

//Write Number To Seven Segment [0:9]
void SEVENSEGMENT_Write(PORT portData,PIN pinA,PIN pinB,PIN pinC,PIN pinD,PORT portEnable,PIN pinEnable,SEVEN_NUMBER number)
{
	
	SEVENSEGMENT_Control(portEnable,pinEnable,HIGH);
	
	switch (number)
	{
		case Number0:
			//DCBA
			//0000
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,LOW);  //C
			Pin_Write(portData,pinB,LOW);  //B
			Pin_Write(portData,pinA,LOW);  //A
		break;
			
		case Number1:
			//DCBA
			//0001
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,LOW);  //C
			Pin_Write(portData,pinB,LOW);  //B
			Pin_Write(portData,pinA,HIGH); //A
		break;
		
		case Number2:
			//DCBA
			//0010
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,LOW);  //C
			Pin_Write(portData,pinB,HIGH); //B
			Pin_Write(portData,pinA,LOW);  //A
		break;
		
		case Number3:
			//DCBA
			//0011
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,LOW);  //C
			Pin_Write(portData,pinB,HIGH); //B
			Pin_Write(portData,pinA,HIGH); //A
		break;
		
		case Number4:
			//DCBA
			//0100
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,HIGH); //C
			Pin_Write(portData,pinB,LOW);  //B
			Pin_Write(portData,pinA,LOW);  //A
		break;
		
		case Number5:
			//DCBA
			//0101
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,HIGH); //C
			Pin_Write(portData,pinB,LOW);  //B
			Pin_Write(portData,pinA,HIGH); //A
		break;
		
		case Number6:
			//DCBA
			//0110
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,HIGH); //C
			Pin_Write(portData,pinB,HIGH); //B
			Pin_Write(portData,pinA,LOW);  //A
		break;
		
		case Number7:
			//DCBA
			//0111
			Pin_Write(portData,pinD,LOW);  //D
			Pin_Write(portData,pinC,HIGH); //C
			Pin_Write(portData,pinB,HIGH); //B
			Pin_Write(portData,pinA,HIGH); //A
		break;
		
		case Number8:
			//DCBA
			//1000
			Pin_Write(portData,pinD,HIGH); //D
			Pin_Write(portData,pinC,LOW);  //C
			Pin_Write(portData,pinB,LOW);  //B
			Pin_Write(portData,pinA,LOW);  //A
		break;
		
		case Number9:
			//DCBA
			//1001
			Pin_Write(portData,pinD,HIGH); //D
			Pin_Write(portData,pinC,LOW);  //C
			Pin_Write(portData,pinB,LOW);  //B
			Pin_Write(portData,pinA,HIGH); //A
		break;
	}
	
	
}

//Write Number To Seven Segment [0:9]
void SEVENSEGMENT_Write_Value(PORT portData,PIN pinA,PIN pinB,PIN pinC,PIN pinD,PORT portEnable,PIN pinEnable,uint8 value)
{
	SEVENSEGMENT_Control(portEnable,pinEnable,HIGH);
	
	if(value <10)
	{
		Pin_Write(portData,pinA,get_bit(value,0));  //A
		Pin_Write(portData,pinB,get_bit(value,1));  //B
		Pin_Write(portData,pinC,get_bit(value,2));  //C
		Pin_Write(portData,pinD,get_bit(value,3));  //D
	}
}
