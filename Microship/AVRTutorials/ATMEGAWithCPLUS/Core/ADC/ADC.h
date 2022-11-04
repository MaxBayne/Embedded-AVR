/*

#####################
Analog Converters
#####################

ADC (Analog To Digital Converters) with 10 Bits Size

Convert Analog Values Input Over Pin To Digital Values

####
Pins
####

PA0 (ADC0) : 
PA1 (ADC1) : 
PA2 (ADC2) : 
PA3 (ADC3) : 
PA4 (ADC4) : 
PA5 (ADC5) : 
PA6 (ADC6) : 
PA7 (ADC7) : 

AVCC : ADC Power
GND  : ADC GND
AREF : ADC Reference



*/


#ifndef ADC_H_
#define ADC_H_


////////////////////////////////////////////
//ENUMS
////////////////////////////////////////////

#pragma region Enums



//Define The ADC Modes
typedef enum
{
	//Single Mode
	ADC_MODE_MANUAL, 
	//Auto Trigger Mode
	ADC_MODE_AUTO
}ADC_MODE;

//Define the ADC Channel
typedef enum
{
	ADC_CHANNEL_ADC0,
	ADC_CHANNEL_ADC1,
	ADC_CHANNEL_ADC2,
	ADC_CHANNEL_ADC3,
	ADC_CHANNEL_ADC4,
	ADC_CHANNEL_ADC5,
	ADC_CHANNEL_ADC6,
	ADC_CHANNEL_ADC7
}ADC_CHANNEL;

//Define the Voltage Reference Type for ADC [ADMUX] Register with Bits [REFS1(7),REFS0(6)]
typedef enum 
{
	//AREF , Internal VREF turned off => REFS1=0 , REFS0=0
	ADC_REFERENCE_TYPE_AREF,
	//AVCC with External capacitor at AREF pin => REFS1=0,REFS0=1
	ADC_REFERENCE_TYPE_AVCC,
	//Internal 2.56 V Reference with External Capacitor at AREF pin => REFS1=1 , REFS0=1
	ADC_REFERENCE_TYPE_2_5V
	
}ADC_REFERENCE_TYPE;
	
//Define the Adjustment Method For Data inside 2 Register [ADCDL,ADCDH]
typedef enum
{
	ADC_ADJUSTED_TYPE_RIGHT,
	ADC_ADJUSTED_TYPE_LEFT
}ADC_ADJUSTED_TYPE;

//Define the PRESCALER used to control the speed to ADC Clock
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
}ADC_PRESCALER;

//Define the Auto Trigger Source that Used To Start conversion inside Register [SFIOR] with Bits [ADTS2(7),ADTS1(6),ADTS0(5)]
typedef enum
{
	//ADTS2=0,ADTS1=0,ADTS0=0
	ADC_TRIGGER_TYPE_FREE_RUNNING,
	
	//ADTS2=0,ADTS1=0,ADTS0=1
	ADC_TRIGGER_TYPE_ANALOG_COMPARATOR,
	
	//ADTS2=0,ADTS1=1,ADTS0=0
	ADC_TRIGGER_TYPE_EXTERNAL_INTERRUPT,
	
	//ADTS2=0,ADTS1=1,ADTS0=1
	ADC_TRIGGER_TYPE_TIMER0_COMPARATOR,
	
	//ADTS2=1,ADTS1=0,ADTS0=0
	ADC_TRIGGER_TYPE_TIMER0_OVERFLOW,
	
	//ADTS2=1,ADTS1=0,ADTS0=1
	ADC_TRIGGER_TYPE_TIMER1_COMPARATOR,
	
	//ADTS2=1,ADTS1=1,ADTS0=0
	ADC_TRIGGER_TYPE_TIMER1_OVERFLOW,
	
	//ADTS2=1,ADTS1=1,ADTS0=1
	ADC_TRIGGER_TYPE_TIMER1_CAPTURE
	
}ADC_TRIGGER_TYPE;


#pragma endregion Enums

class ADConverter
{
	private:
	
	ADC_CHANNEL _channel;
	ADC_MODE _mode;
	ADC_TRIGGER_TYPE _triggerSource;
	ADC_REFERENCE_TYPE _volateReference;
	ADC_ADJUSTED_TYPE _adjustedMethod;
	
	
	public:
	
	//Constructor
	ADConverter(ADC_CHANNEL channel);
	
	//Initialize ADC Channel
	void Initialize(ADC_MODE mode,ADC_TRIGGER_TYPE triggerSource,ADC_REFERENCE_TYPE volateReference ,ADC_ADJUSTED_TYPE adjustedMethod);
	
	//Start Conversion For ADC Channel
	void StartConversion();

	//Read Result of ADC Conversion as 16 Bit (2Byte) Register Because ADC Result Stored inside 2 ADC Data Register [ADCH+ADCL]
	uint16 ReadResult();

	//Start Conversion For ADC with Specific Channel and Read the Result
	uint16  StartConversionAndRead();	
	
	
	//Enable ADC Interrupt
	void Enable_ADC_Interrupt(void(* functionPtr)());
	
	//Disable ADC Interrupt
	void Disable_ADC_Interrupt();
	
	//Clear ADC Interrupt Flag
	void Clear_ADC_Flag();


};





#endif /* ADC_H_ */