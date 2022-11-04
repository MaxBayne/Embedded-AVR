#include "ADC/ADC.h"

//Config ADC Module
void ADC_Initialize(ADC_Mode mode,ADC_TriggerSource triggerSource,ADC_ReferenceType volateReference ,ADC_AdjustedType adjustedMethod)
{
	//Set when ADC conversion will start Auto Trigger Start or Manual Start
	//Set ADC Mode Manual/Auto by edit register [ADCSRA] ADC Control and Status Register
	//ADATE (5) : set to 1 to make ADC Auto Trigger Enable and when rising edge start by target select method then conversion will be start
	//ADATE (5) : set to 0 to make ADC Auto Trigger Disabled and conversion will work manual when calling start
	if(mode==ADC_Mode_Manual)
	{
		//Manual Mode
		
		clear_bit(ADC_REG_ADCSRA,5);	//ADATE = 0
	}
	else
	{
		//Auto Trigger Mode
		set_bit(ADC_REG_ADCSRA,5);		//ADATE = 1
	}
	
	
	
	 
	 
	 
	 //Set Trigger Selector used with Auto Trigger Mode Only to Determine when Conversion start
	 //Register [SFIOR] with Bits [ADTS2(7),ADTS1(6),ADTS0(5)]
	 if(mode==ADC_Mode_Auto)
	 {
		 switch(triggerSource)
		 {
			 case ADC_TRIGGER_FREE_Running:
			 //ADTS2=0,ADTS1=0,ADTS0=0
			 clear_bit(ADC_REG_SFIOR,7);
			 clear_bit(ADC_REG_SFIOR,6);
			 clear_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 
			 case ADC_TRIGGER_ANALOG_COMPARATOR:
			 //ADTS2=0,ADTS1=0,ADTS0=1
			 clear_bit(ADC_REG_SFIOR,7);
			 clear_bit(ADC_REG_SFIOR,6);
			 set_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_EXTERNAL_INTERRUPT:
			 //ADTS2=0,ADTS1=1,ADTS0=0
			 clear_bit(ADC_REG_SFIOR,7);
			 set_bit(ADC_REG_SFIOR,6);
			 clear_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TIMER0_COMPARATOR:
			 //ADTS2=0,ADTS1=1,ADTS0=1
			 clear_bit(ADC_REG_SFIOR,7);
			 set_bit(ADC_REG_SFIOR,6);
			 set_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TIMER0_OVERFLOW:
			 //ADTS2=1,ADTS1=0,ADTS0=0
			 set_bit(ADC_REG_SFIOR,7);
			 clear_bit(ADC_REG_SFIOR,6);
			 clear_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TIMER1_COMPARATOR:
			 //ADTS2=1,ADTS1=0,ADTS0=1
			  set_bit(ADC_REG_SFIOR,7);
			  clear_bit(ADC_REG_SFIOR,6);
			  set_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TIMER1_OVERFLOW:
			 //ADTS2=1,ADTS1=1,ADTS0=0
			  set_bit(ADC_REG_SFIOR,7);
			  set_bit(ADC_REG_SFIOR,6);
			  clear_bit(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TIMER1_CAPTURE:
			 //ADTS2=1,ADTS1=1,ADTS0=1
			  set_bit(ADC_REG_SFIOR,7);
			  set_bit(ADC_REG_SFIOR,6);
			  set_bit(ADC_REG_SFIOR,5);
			 break;
			 
		 }
		 
	 }
	 
	 //Set Voltage Reference that will be used by ADC
	 //settings this by control bits [REFS1(7) , REFS0(6)] inside Register [ADMUX]
	 switch(volateReference)
	 {
		 case ADC_ReferenceType_AVCC:
		 clear_bit(ADC_REG_ADMUX,7); //Set REFS1=0
		 set_bit(ADC_REG_ADMUX,6);	//Set REFS0=1
		 break;
		 
		 case ADC_ReferenceType_AREF:
		 clear_bit(ADC_REG_ADMUX,7); //Set REFS1=0
		 clear_bit(ADC_REG_ADMUX,6);	//Set REFS0=0
		 break;
		 
		 case ADC_ReferenceType_2_5V:
		 set_bit(ADC_REG_ADMUX,7);	//Set REFS1=1
		 set_bit(ADC_REG_ADMUX,6);	//Set REFS0=1
		 break;
	 }
	 
	 //Set Data Adjustment For Result Stored inside 2 Register [ADCH+ADCL] for total 10 bits
	 if(adjustedMethod==ADC_AdjustedType_Right)
	 {
		 clear_bit(ADC_REG_ADMUX,5);
	 }
	 else
	 {
		 set_bit(ADC_REG_ADMUX,5);
	 }
	 
	 //Clear Interrupt Flag For ADIF
	 set_bit(ADC_REG_ADCSRA,4);
	 
	 //Enabling ADC Module using Register [ADCSRA] with bits ADEN[7]
	 set_bit(ADC_REG_ADCSRA,7);
}

//Start Conversion For ADC with Specific Channel
void ADC_StartConversion(ADC_Channel channel)
{
	//Set the Channel that will be used to start conversion
	//using Register [ADMUX] with bits [4:0]
	
	switch(channel)
	{
		case ADC0:
		//00000
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		clear_bit(ADC_REG_ADMUX,2);
		clear_bit(ADC_REG_ADMUX,1);
		clear_bit(ADC_REG_ADMUX,0);
		
		Pin_Direction(PortA,0,OUTPUT);
		
		break;
		
		case ADC1:
		//00001
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		clear_bit(ADC_REG_ADMUX,2);
		clear_bit(ADC_REG_ADMUX,1);
		set_bit(ADC_REG_ADMUX,0);
		
		Pin_Direction(PortA,1,OUTPUT);
		
		break;
		
		case ADC2:
		//00010
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		clear_bit(ADC_REG_ADMUX,2);
		set_bit(ADC_REG_ADMUX,1);
		clear_bit(ADC_REG_ADMUX,0);
		
		Pin_Direction(PortA,2,OUTPUT);
		
		break;
		
		case ADC3:
		//00011
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		clear_bit(ADC_REG_ADMUX,2);
		set_bit(ADC_REG_ADMUX,1);
		set_bit(ADC_REG_ADMUX,0);
		
		Pin_Direction(PortA,3,OUTPUT);
		
		break;
		
		case ADC4:
		//00100
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		set_bit(ADC_REG_ADMUX,2);
		clear_bit(ADC_REG_ADMUX,1);
		clear_bit(ADC_REG_ADMUX,0);
		
		Pin_Direction(PortA,4,OUTPUT);
		
		break;
		
		case ADC5:
		//00101
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		set_bit(ADC_REG_ADMUX,2);
		clear_bit(ADC_REG_ADMUX,1);
		set_bit(ADC_REG_ADMUX,0);

		Pin_Direction(PortA,5,OUTPUT);
		
		break;
		
		case ADC6:
		//00110
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		set_bit(ADC_REG_ADMUX,2);
		set_bit(ADC_REG_ADMUX,1);
		clear_bit(ADC_REG_ADMUX,0);
		
		Pin_Direction(PortA,6,OUTPUT);
		
		break;
		
		case ADC7:
		//00111
		clear_bit(ADC_REG_ADMUX,4);
		clear_bit(ADC_REG_ADMUX,3);
		set_bit(ADC_REG_ADMUX,2);
		set_bit(ADC_REG_ADMUX,1); 
		set_bit(ADC_REG_ADMUX,0);
		Pin_Direction(PortA,7,OUTPUT);
		break;
	}
	
	//Start Conversion by using Register[ADCSRA] with bits [ADSC(6)]
	set_bit(ADC_REG_ADCSRA,6);
	
	
}

//Read Result of ADC Conversion as 16 Bit (2Byte) Register Because ADC Result Stored inside 2 ADC Data Register [ADCH+ADCL]
uint16 ADC_ReadResult()
{
	//Return Conversion Data Result from two Register [ADCL+ADCH]

	while(get_bit(ADC_REG_ADCSRA,6));
	
	return ADC_REG_ADCLH;
}

//Start Conversion For ADC with Specific Channel and Read the Result
uint16  ADC_StartConversionAndRead(ADC_Channel channel)
{
	ADC_StartConversion(channel);
	
	return ADC_ReadResult();
}