#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int PWM_main(void)
{
	//Timer as PWM
	
	//Duty Cycle = (Thigh / Tperiod) * 100
	//Ducty Cyle = (50/100)*100 = 50%
	
	//Output Voltage = Duty Cyle * Input Voltage
	//Output Voltage = 50% * 5 = 2.5 V
	
	/*
	- ATMEGA328P has 3 Timer supported PWM related to Output Compare Units total is (6 PWM Pins)
		Timer0 Support 2 PWM Pins [OC0A==PD6,OC0B==PD5]
		Timer1 Support 2 PWM Pins [OC1A==PB1,OC1B==PB2]
		Timer2 Support 2 PWM Pins [OC2A==PB3,OC2B==PD3]
	
	- PWM Frequency Calculated by using this :
		Freq(PWM)=Freq(Crystal)/(Prescaler*2^8) = HZ
		Freq(PWM)=16000000/(1024*256)= 61 Hz
	
	- Enable Fast PWM Mode using Wave Generation using Register [TCCR0A] with bits [WGM01(1),WGM00(0)] and Register[TCCR0B] with bits [WGM02]
		Set WGM02=1,WGM01 =1, WGM00=1 to enable Fast PWM Mode where Top is value inside OCRA and update OCRA at Bottom and Timer Overflow On OCRA Value
		Set WGM02=0,WGM01 =1, WGM00=1 to enable Fast PWM Mode where Top is 255 and update OCRA at Bottom and TimerOverflow on max(255)
	
	- Config PWM Settings using Register [TCCR0A] with bits [COM0A0(6),COM0A1(7),COM0B0(4),COM0B1(5)]
		For OC0A(PD6) use bits [COM0A0(6),COM0A1(7)] inside Register [TCCR0A]
		For OC0B(PD5) use bits [COM0B0(4),COM0B1(5)] inside Register [TCCR0A]
		
		Set COM0A1 =0 , COM0A0=0 to make OC0A(PD6) as Normal Port Operation
		Set COM0A1 =1 , COM0A0=0 to make OC0A(PD6) as PWM that Clear OC0A on Compare Match,Set OC0A at Bottom (Non-Inverting Mode)
		Set COM0A1 =1 , COM0A0=1 to make OC0A(PD6) as PWM that Set OC0A on Compare Match,Clear OC0A at Bottom (Inverting Mode)
		
	*/
	//--------------------------------------------------------------------
	
	
	//Setup ---------------
	
	//1) Set PWM Pins [PD6,PD5] as Output
	//DDRD |= (1<<DDD6);									//Make Pin [PD6] as Output
	//DDRD |= (1<<DDD5);									//Make Pin [PD5] as Output
	
		
	//2) Select Clock Source For Timer0 by using Register TCCR0B with Bits (CS02-CS01-CS00)
	
	//TCCR0B &= ~(1<<CS02) & (1<<CS01) & (1<<CS00);		//No Clock Source Mean Timer/Counter is Stopped So Clear CS02,CS01,CS00
	
	//use External Clock Frequency 16 MHZ With No Prescaling
	//TCCR0B &= ~(1<<CS02) & (1<<CS01);				//Clear CS02,CS01
	//TCCR0B |=  (1<<CS00);							//Set CS00
	
	//use External Clock Frequency With Prescaler 8 mean 16/8 = 2 MHZ
	//TCCR0B &= ~(1<<CS02) & (1<<CS00);				//Clear CS02,CS00
	//TCCR0B |=  (1<<CS01);							//Set CS01
	
	//use External Clock Frequency With Prescaler 64 mean 16/64 = .25 MHZ
	//TCCR0B &= ~(1<<CS02);							//Clear CS02
	//TCCR0B |=  (1<<CS01) | (1<<CS00);				//Set CS01 , CS00
	
	//use External Clock Frequency With Prescaler 256 mean 16/256 = .06 MHZ
	//TCCR0B &= ~(1<<CS01) & (1<<CS00);				//Clear CS01,CS00
	//TCCR0B |=  (1<<CS02);							//Set CS02
	
	//use External Clock Frequency With Prescaler 1024 mean 16/1024 = .015 MHZ
	//TCCR0B &= ~(1<<CS01);							//Clear CS01
	//TCCR0B |=  (1<<CS02) | (1<<CS00);					//Set CS02 , CS00
	
	//use External Clock Source On Pin T0 (PD4) , Clock On Falling Edge
	//TCCR0B &= ~(1<<CS00);							//Clear CS00
	//TCCR0B |=  (1<<CS02) |  (1<<CS01);				//Set CS02 , CS01
	
	//use External Clock Source On Pin T0 (PD4) , Clock On Rising Edge
	//TCCR0B &= ~(1<<CS00);							//Clear CS00
	//TCCR0B |=  (1<<CS02) |  (1<<CS01);				//Set CS02 , CS01

	
	//3) Enable PWM Over Pin [PD6] , [PD5] with Non-Inverting Mode using Register [TCCR0A] with bits [COM0A1,COM0A0,COM0B1,COM0B0]
	//TCCR0A |=  (1<<COM0A1);								//Enable Non Inverting PWM Over Pin PD6(OC0A)
	//TCCR0A &= ~(1<<COM0A0);
	
	//TCCR0A |=  (1<<COM0B1);								//Enable Non Inverting PWM Over Pin PD5(OC0B)
	//TCCR0A &= ~(1<<COM0B0);


	//4) Select PWM Mode using Wave Generation Mode Bits inside Register [TCCR0A] with bits [WGM00(0),WGM01(1)] and Register [TCCR0B] with bits [WGM02]
	//TCCR0A |= (1<<WGM01) | (1<<WGM00);					//using Fast PWM Mode
	//TCCR0A |= (1<<WGM00);								//using Phase Correct PWM Mode


	//5) Set PWM Duty Cycle inside Output Compare Unit [0:255]
	//OCR0A=180;											//Set Duty Cycle Percent For Pin PD6 (180/255*100) = 70%
	//OCR0B=50;											//Set Duty Cycle Percent For Pin PD5 (50/255*100) = 20%

	//Freq(PWM)=Freq(Crystal)/(Prescaler*2^8) = HZ

	//6) Enable Output Compare Interrupt by using Register [TIMSK0] with bit0 [OCIE0A] For Output Compare
	//TIMSK0 |= (1<<OCIE0A);							//Enable Output Compare Interrupt To Overflow when Compare Matched with Timer Value
	
	//7) Enable Global Interrupt inside Register (SREG) Bit7 (I)
	//sei();											//Enable Global Interrupt
	//cli();										//Disable Global Interrupt [Default]
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
												//Define LED1 -------------
	//DDRD |= (1<<DDD0);							//Define Pin 0 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD0);						//RESET Pin 0 on Port D as LOW [0] [&] [AND]
	
  												//Define LED2 -------------
	//DDRD |= (1<<DDD1);							//Define Pin 1 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD1);						//RESET Pin 1 on Port D as LOW [0] [&] [AND]
	
	
	
	//Loop -----------------
	
	while (1)
	{
	
	}
	
}



