#define F_CPU 16000000LL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <util/delay.h>

int Register_main(void)
{
	/////////////////////////////////////////////////
	//Data Direction Register (Input/Output)
	//Input = 0 = Reset [AND &]
	//OUTPUT= 1 = SET   [OR  |]
	/////////////////////////////////////////////////
	
	//For Full Port ------------
	DDRB&=0;						//Reset All Port B Pins As Input [Decimal Value]
	DDRB&=0x00;						//Reset All Port B Pins As Input [Hex Value]
	DDRB&=0b00000000;				//Reset All Port B Pins As Input [Binary Value]
	
	DDRC|=1;						//Set All Port C Pins As Output [Decimal Value]
	DDRC|=0xFF;						//Set All Port C Pins As Output [Hex Value]
	DDRC|=0b11111111;				//Set All Port C Pins As Output [Binary Value]
	
	//For Special Pins ----------
	DDRD|=(1<<DDD0);				//Set PIN 0 For Port D as OUTPUT
	DDRD|=0b00000001;				//Set PIN 0 For Port D as OUTPUT
	DDRD|=(1<<DDD1)|(1<<DDD2);		//Set PIN 1,PIN 2 For Port D as OUTPUT
	DDRD|=0b00000110;				//Set PIN 1,PIN 2 For Port D as OUTPUT
	
	DDRD&=~(1<<DDD3);				//Reset PIN 3 For PORT D as INPUT
	
	//To Make Pin as Pull-Up [Define as input and out HIGH]
	//------------------------------------------------------
	
	DDRB&=~(1<<DDB0);				//Define Pin 0 For Port B as Input [0]=[Reset]=[AND &]
	PORTB|=(1<<PORTB0);				//Out HIGH @ Pin 0 For Port B [1]=[SET]=[OR |]
	
	
	/////////////////////////////////////////////////
	//Data Register (HIGH/LOW)
	/////////////////////////////////////////////////
	
	//For Full Port ------------
	PORTC|=1;						//Set HIGH For All Port C Pins [Decimal Value]
	PORTC|=0xFF;					//Set HIGH For All Port C Pins [Hex Value]
	PORTC|=0b11111111;				//Set HIGH For All Port C Pins [Binary Value]
	
	
	//For Special Pins ----------
	PORTD|=(1<<PORTD0);				//Set HIGHT For Pin 0 Of Port D
	PORTD|=0x01;					//Set HIGHT For Pin 0 Of Port D
	PORTD|=0b00000001;				//Set HIGHT For Pin 0 Of Port D
	PORTD|=0b00000010;				//Set HIGHT For Pin 1 Of Port D
	
	PORTD&=~(1<<PORTD2);			//Reset LOW For Pin 2 Of Port D
	PORTD&=0xFB;					//Reset LOW For Pin 2 Of Port D
	PORTD&=0b11111011;				//Reset LOW For Pin 2 Of Port D
	
	PORTB ^=(1<<PORTB5);			//Reverse Value For Pin 5 of Port B
	
	/////////////////////////////////////////////////
	//Read Pin Statue Register (HIGH/LOW)
	/////////////////////////////////////////////////
	
	//if((PINB&(1<<PINB0))==0)			//Mean The Pull-Up Pin 0 SWITCH are Pressed To Ground So its LOW
	//{
		////LIGHT LED ON OUT HIGH [SET, OR |]
		//
	//}
	//else
	//{
		////LIGHT LED OFF
		//
	//}
	
	
	while (1)
	{
	
	}
}



