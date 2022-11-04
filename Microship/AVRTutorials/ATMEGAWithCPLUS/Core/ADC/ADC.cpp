#include "Common/Common.h"
#include "Common/Registers.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"
#include "Interrupts/GlobalInterrupt.h"
#include "ADC/ADC.h"

//Function Pointer For ADC Callback Delegates
void(*ADC_Callback_Ptr)();

//Constructor
ADConverter::ADConverter(ADC_CHANNEL channel)
{
	_channel=channel;
}

//Config ADC Module
void ADConverter::Initialize(ADC_MODE mode,ADC_TRIGGER_TYPE triggerSource,ADC_REFERENCE_TYPE volateReference ,ADC_ADJUSTED_TYPE adjustedMethod)
{
	//Set when ADC conversion will start Auto Trigger Start or Manual Start
	//Set ADC Mode Manual/Auto by edit register [ADCSRA] ADC Control and Status Register
	//ADATE (5) : set to 1 to make ADC Auto Trigger Enable and when rising edge start by target select method then conversion will be start
	//ADATE (5) : set to 0 to make ADC Auto Trigger Disabled and conversion will work manual when calling start
	
	_mode=mode;
	_triggerSource=triggerSource;
	_volateReference=volateReference;
	_adjustedMethod=adjustedMethod;
	
	
	if(_mode==ADC_MODE_MANUAL)
	{
		//Manual Mode
		
		BITWISE_CLEAR_BIT(ADC_REG_ADCSRA,5);  //ADATE = 0
		
	}
	else
	{
		//Auto Trigger Mode
		BITWISE_SET_BIT(ADC_REG_ADCSRA,5);  //ADATE = 1
	}
	 
	 
	 
	 //Set Trigger Selector used with Auto Trigger Mode Only to Determine when Conversion start
	 //Register [SFIOR] with Bits [ADTS2(7),ADTS1(6),ADTS0(5)]
	 if(_mode==ADC_MODE_AUTO)
	 {
		 switch(_triggerSource)
		 {
			 case ADC_TRIGGER_TYPE_FREE_RUNNING:
			 //ADTS2=0,ADTS1=0,ADTS0=0
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,7);
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,6);
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 
			 case ADC_TRIGGER_TYPE_ANALOG_COMPARATOR:
			 //ADTS2=0,ADTS1=0,ADTS0=1
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,7);
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,6);
			 BITWISE_SET_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TYPE_EXTERNAL_INTERRUPT:
			 //ADTS2=0,ADTS1=1,ADTS0=0
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,7);
			 BITWISE_SET_BIT(ADC_REG_SFIOR,6);
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TYPE_TIMER0_COMPARATOR:
			 //ADTS2=0,ADTS1=1,ADTS0=1
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,7);
			 BITWISE_SET_BIT(ADC_REG_SFIOR,6);
			 BITWISE_SET_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TYPE_TIMER0_OVERFLOW:
			 //ADTS2=1,ADTS1=0,ADTS0=0
			 BITWISE_SET_BIT(ADC_REG_SFIOR,7);
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,6);
			 BITWISE_CLEAR_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TYPE_TIMER1_COMPARATOR:
			 //ADTS2=1,ADTS1=0,ADTS0=1
			  BITWISE_SET_BIT(ADC_REG_SFIOR,7);
			  BITWISE_CLEAR_BIT(ADC_REG_SFIOR,6);
			  BITWISE_SET_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TYPE_TIMER1_OVERFLOW:
			 //ADTS2=1,ADTS1=1,ADTS0=0
			  BITWISE_SET_BIT(ADC_REG_SFIOR,7);
			  BITWISE_SET_BIT(ADC_REG_SFIOR,6);
			  BITWISE_CLEAR_BIT(ADC_REG_SFIOR,5);
			 break;
			 
			 case ADC_TRIGGER_TYPE_TIMER1_CAPTURE:
			 //ADTS2=1,ADTS1=1,ADTS0=1
			  BITWISE_SET_BIT(ADC_REG_SFIOR,7);
			  BITWISE_SET_BIT(ADC_REG_SFIOR,6);
			  BITWISE_SET_BIT(ADC_REG_SFIOR,5);
			 break;
			 
		 }
		 
	 }
	 
	 //Set Voltage Reference that will be used by ADC
	 //settings this by control bits [REFS1(7) , REFS0(6)] inside Register [ADMUX]
	 switch(_volateReference)
	 {
		 case ADC_REFERENCE_TYPE_AVCC:
		 BITWISE_CLEAR_BIT(ADC_REG_ADMUX,7); //Set REFS1=0
		 BITWISE_SET_BIT(ADC_REG_ADMUX,6);	//Set REFS0=1
		 break;
		 
		 case ADC_REFERENCE_TYPE_AREF:
		  BITWISE_CLEAR_BIT(ADC_REG_ADMUX,7); //Set REFS1=0
		  BITWISE_CLEAR_BIT(ADC_REG_ADMUX,6);	//Set REFS0=0
		 break;
		 
		 case ADC_REFERENCE_TYPE_2_5V:
		 BITWISE_SET_BIT(ADC_REG_ADMUX,7);	//Set REFS1=1
		 BITWISE_SET_BIT(ADC_REG_ADMUX,6);	//Set REFS0=1
		 break;
	 }
	 
	 //Set Data Adjustment For Result Stored inside 2 Register [ADCH+ADCL] for total 10 bits
	 if(_adjustedMethod==ADC_ADJUSTED_TYPE_RIGHT)
	 {
		 BITWISE_CLEAR_BIT(ADC_REG_ADMUX,5);
	 }
	 else
	 {
		 BITWISE_SET_BIT(ADC_REG_ADMUX,5);
	 }
	 
	 //Clear Interrupt Flag For ADIF
	 BITWISE_SET_BIT(ADC_REG_ADCSRA,4);
	 
	 
	 //Enabling ADC Module using Register [ADCSRA] with bits ADEN[7]
	 BITWISE_SET_BIT(ADC_REG_ADCSRA,7);
}

//Start Conversion For ADC with Specific Channel
void ADConverter::StartConversion()
{
	//Set the Channel that will be used to start conversion
	//using Register [ADMUX] with bits [4:0]
	
	switch(_channel)
	{
		case ADC_CHANNEL_ADC0:
		//00000
		
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,2);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,1);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,0);
		
		GPIO::Pin_Direction(GPIO_IO_PA0,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC1:
		//00001
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,2);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,1);
		BITWISE_SET_BIT(ADC_REG_ADMUX,0);
		
		GPIO::Pin_Direction(GPIO_IO_PA1,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC2:
		//00010
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,2);
		BITWISE_SET_BIT(ADC_REG_ADMUX,1);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,0);
		
		GPIO::Pin_Direction(GPIO_IO_PA2,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC3:
		//00011
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,2);
		BITWISE_SET_BIT(ADC_REG_ADMUX,1);
		BITWISE_SET_BIT(ADC_REG_ADMUX,0);
		
		GPIO::Pin_Direction(GPIO_IO_PA3,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC4:
		//00100
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_SET_BIT(ADC_REG_ADMUX,2);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,1);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,0);
		
		GPIO::Pin_Direction(GPIO_IO_PA4,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC5:
		//00101
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_SET_BIT(ADC_REG_ADMUX,2);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,1);
		BITWISE_SET_BIT(ADC_REG_ADMUX,0);

		GPIO::Pin_Direction(GPIO_IO_PA5,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC6:
		//00110
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_SET_BIT(ADC_REG_ADMUX,2);
		BITWISE_SET_BIT(ADC_REG_ADMUX,1);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,0);
		
		GPIO::Pin_Direction(GPIO_IO_PA6,GPIO_DIRECTION_OUTPUT);
		
		break;
		
		case ADC_CHANNEL_ADC7:
		//00111
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,4);
		BITWISE_CLEAR_BIT(ADC_REG_ADMUX,3);
		BITWISE_SET_BIT(ADC_REG_ADMUX,2);
		BITWISE_SET_BIT(ADC_REG_ADMUX,1); 
		BITWISE_SET_BIT(ADC_REG_ADMUX,0);
		GPIO::Pin_Direction(GPIO_IO_PA7,GPIO_DIRECTION_OUTPUT);
		break;
	}
	
	//Start Conversion by using Register[ADCSRA] with bits [ADSC(6)]
	BITWISE_SET_BIT(ADC_REG_ADCSRA,6);
	
}

//Read Result of ADC Conversion as 16 Bit (2Byte) Register Because ADC Result Stored inside 2 ADC Data Register [ADCH+ADCL]
uint16 ADConverter::ReadResult()
{
	//Return Conversion Data Result from two Register [ADCL+ADCH]

	while(BITWISE_GET_BIT(ADC_REG_ADCSRA,6));
	
	return ADC_REG_ADCLH;
}

//Start Conversion For ADC with Specific Channel and Read the Result
uint16  ADConverter::StartConversionAndRead()
{
	StartConversion();
	
	return ReadResult();
}



//Enable ADC Interrupt
void ADConverter::Enable_ADC_Interrupt(void(* functionPtr)())
{
	/*
	-----------------------------------------------------------
	Enable ADC Interrupt (ADC Conversion Complete)
	-----------------------------------------------------------
	- for enable ADC Conversion Complete Interrupt  : then set (ADIE) (Bit-3) with HIGH inside Register (ADCSRA)
	*/
	
	//Save ADC Interrupt CAllback Function
	ADC_Callback_Ptr = functionPtr;
	
	//Enable ADC Interrupt
	BITWISE_SET_BIT(ADC_REG_ADCSRA,3);

}

//Disable ADC Interrupt
void ADConverter::Disable_ADC_Interrupt()
{
	//Clear Function Pointer
	ADC_Callback_Ptr=0;
	
	//Disable ADC Interrupt
	BITWISE_CLEAR_BIT(ADC_REG_ADCSRA,3);
}

//Clear External Interrupt Flag
void ADConverter::Clear_ADC_Flag()
{
	/*
	-----------------------
	ADC Interrupt Flag (ADIF)
	-----------------------
	
	- ADC Interrupt Flag Set = 1 When ADC Conversion and Data Registers are Updated
	- ADC Interrupt Fired If ADIE (ADC Interrupt Enabled) and Global Interrupt Enabled
	- To Clear ADIF Flag Just Set its Value
	- We Use Register [ADCSRA] (ADC Control Status Register) To Set ADC Interrupt Flag

	*/
	
	//Clear ADC Interrupt Flag By Set Bit (ADIF) (4)
	BITWISE_SET_BIT(ADC_REG_ADCSRA,4);
	
}



//---------------------------------------------------------
//ADC Interrupt Services Routine From Vector Table :
//---------------------------------------------------------

//Interrupt Service Routine For ADC
ISR(ADC_vect)
{
	if(ADC_Callback_Ptr!=0)
	{
		ADC_Callback_Ptr();
	}
}
