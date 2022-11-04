#ifndef ADC_H_
#define ADC_H_

#include "Common/Common.h"
#include "GPIO/GPIO.h"

//Registers Address Definitions -------------------------------------------------

#define ADC_REG_ADCL *((volatile uint8*) 0x24)		//Address of Register (ADC Data Register Low)
#define ADC_REG_ADCH *((volatile uint8*) 0x25)      //Address of Register (ADC Data Register High)
#define ADC_REG_ADCLH *((volatile uint16*) 0x24)    //Read Data 16 Bit From ADC Data Register LOW+High
#define ADC_REG_ADCSRA *((volatile uint8*) 0x26)	//Address of Register (ADC Control and Status Register)
#define ADC_REG_ADMUX *((volatile uint8*) 0x27)		//Address of Register (ADC Multiplexer Selection Register)
#define ADC_REG_SFIOR *((volatile uint8*) 0x50)		//Address of Register (Special Function Input/Output Register)

//enum Definitions -------------------------------------------------

//Define The ADC Modes
typedef enum
{
	//Single Mode
	ADC_Mode_Manual, 
	//Auto Trigger Mode
	ADC_Mode_Auto
}ADC_Mode;

//Define the ADC Channel
typedef enum
{
	ADC0,
	ADC1,
	ADC2,
	ADC3,
	ADC4,
	ADC5,
	ADC6,
	ADC7
}ADC_Channel;

//Define the Voltage Reference Type for ADC [ADMUX] Register with Bits [REFS1(7),REFS0(6)]
typedef enum 
{
	//AREF , Internal VREF turned off => REFS1=0 , REFS0=0
	ADC_ReferenceType_AREF,
	//AVCC with External capacitor at AREF pin => REFS1=0,REFS0=1
	ADC_ReferenceType_AVCC,
	//Internal 2.56 V Reference with External Capacitor at AREF pin => REFS1=1 , REFS0=1
	ADC_ReferenceType_2_5V
	
}ADC_ReferenceType;
	
//Define the Adjustment Method For Data inside 2 Register [ADCDL,ADCDH]
typedef enum
{
	ADC_AdjustedType_Right,
	ADC_AdjustedType_Left
}ADC_AdjustedType;

//Define the Prescaler used to control the speed to ADC Clock
typedef enum
{
	//ADPS2=0,ADPS1=0,ADPS0=0
	ADC_PRESCALER_2, 
	//ADPS2=0,ADPS1=1,ADPS0=0
	ADC_PRESCALER_4,
	//ADPS2=0,ADPS1=1,ADPS0=1
	ADC_PRESCALER_8,
	//ADPS2=1,ADPS1=0,ADPS0=0
	ADC_PRESCALER_16,
	//ADPS2=1,ADPS1=0,ADPS0=1
	ADC_PRESCALER_32,
	//ADPS2=1,ADPS1=1,ADPS0=0
	ADC_PRESCALER_64,
	//ADPS2=1,ADPS1=1,ADPS0=1
	ADC_PRESCALER_128
}ADC_PrescalerType;

//Define the Auto Trigger Source that Used To Start conversion inside Register [SFIOR] with Bits [ADTS2(7),ADTS1(6),ADTS0(5)]
typedef enum
{
	//ADTS2=0,ADTS1=0,ADTS0=0
	ADC_TRIGGER_FREE_Running,
	
	//ADTS2=0,ADTS1=0,ADTS0=1
	ADC_TRIGGER_ANALOG_COMPARATOR,
	
	//ADTS2=0,ADTS1=1,ADTS0=0
	ADC_TRIGGER_EXTERNAL_INTERRUPT,
	
	//ADTS2=0,ADTS1=1,ADTS0=1
	ADC_TRIGGER_TIMER0_COMPARATOR,
	
	//ADTS2=1,ADTS1=0,ADTS0=0
	ADC_TRIGGER_TIMER0_OVERFLOW,
	
	//ADTS2=1,ADTS1=0,ADTS0=1
	ADC_TRIGGER_TIMER1_COMPARATOR,
	
	//ADTS2=1,ADTS1=1,ADTS0=0
	ADC_TRIGGER_TIMER1_OVERFLOW,
	
	//ADTS2=1,ADTS1=1,ADTS0=1
	ADC_TRIGGER_TIMER1_CAPTURE
	
}ADC_TriggerSource;


//Operations Definitions -------------------------------------------------

//Config ADC Module
void ADC_Initialize(ADC_Mode mode,ADC_TriggerSource triggerSource,ADC_ReferenceType volateReference ,ADC_AdjustedType adjustedMethod);

//Start Conversion For ADC with Specific Channel
void ADC_StartConversion(ADC_Channel channel);

//Read Result of ADC Conversion as 16 Bit (2Byte) Register Because ADC Result Stored inside 2 ADC Data Register [ADCH+ADCL] 
uint16 ADC_ReadResult();

//Start Conversion For ADC with Specific Channel and Read the Result
uint16  ADC_StartConversionAndRead(ADC_Channel channel);

#endif /* ADC_H_ */