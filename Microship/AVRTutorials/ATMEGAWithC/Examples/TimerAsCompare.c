#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int TimerAsCompare_main(void)
{
	//Timer as Compare
	
	/*
	- ATMEGA328P have 3 Timer
		Timer0:is 8 Bit so it count from 0:255 with Two Unit For Compare A,B
		Timer1:is 16 Bit so it count from 0:65535 with Two Unit For Compare A,B
		Timer2:is 8 Bit so it count from 0:255 with Two Unit For Compare A,B
	- Timer Can Work with Many Mode
		Timing Mode   (Normal Mode)
		Clear Timer On Compare Mode (CTC)
		Fast PWM Mode
		Phase Correct PWM Mode
	- To Control Timer use Timer/Counter Control Register
		On Compare Mode Use Register (TCCR0A)
		On Normal Timing Mode Use Register (TCCR0B)
	- Timer Can Work With Clock Source
		External Clock Source (16MHz)
		External Clock Source With Prescaling to Down the Frequency
		External Clock Source Via Pin T0 [PD4] or Pin T1 [PD5]
	
	- Enable Output Compare Mode using Register [TCCR0A] with bits [WGM01(1),WGM00(0)]
		Set WGM01 =1, WGM00=0 to enable Output Compare Mode
	
	- Config Output Compare Mode using Register [TCCR0A] with bits [COM0A0(6),COM0A1(7),COM0B0(4),COM0B1(5)]
		For OC0A(PD6) use bits [COM0A0(6),COM0A1(7)] inside Register [TCCR0A]
		For OC0B(PD5) use bits [COM0B0(4),COM0B1(5)] inside Register [TCCR0A]
	
		Set COM0A1 =0 , COM0A0=0 to make OC0A(PD6) as Normal Port Operation
		Set COM0A1 =0 , COM0A0=1 to make OC0A(PD6) as Toggle OC0A on Compare Match with Timer Counter and OCR0A
		Set COM0A1 =1 , COM0A0=0 to make OC0A(PD6) as Clear OC0A on Compare Match with Timer Counter and OCR0A
		Set COM0A1 =1 , COM0A0=1 to make OC0A(PD6) as set on Compare Match with Timer Counter and OCR0A
	
	
	- To Set Value For Compare by Timer use Register [OCR0A] For Unit A , Reigster[OCR0B] For Unit B
		OCR0A=(FrequencyExternal/Prescaler/DesiredFrequency)
		OCR0A=16 MHZ/1024/100 HZ=157
		So when Timer 0 =157 then it will triggered every 10 ms and will reset automatically
	- Interrupt With Timer has two Shape
		Interrupt with Timer Overflow (Reached To 255)
		Interrupt with Output Compare Matched then Fire Interrupt
	- Enable Interrupt by using Register [TIMSK0]
		Enable Timer Interrupt so Set bit 0 [TOIE0]
		Enable Timer Output Compare A Interrupt so Set bit 1 [OCIE0A]
		Enable Timer Output Compare B Interrupt so Set bit 1 [OCIE0B]
	*/
	//--------------------------------------------------------------------
	
	
	//Setup ---------------
	
	//1) Enable Output Compare Mode using Register [TCCR0A] with bits [WGM01(1),WGM00(0)]
	//TCCR0A |= (1<<WGM01);
		 
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

	
	
	//3) Set Value For Compare Matching inside Unit A 
	//OCR0A=FrequencyExternal/Prescaler/DesiredFrequency
	//OCR0A=16 MHZ/1024/100 HZ=157
	//OCR0A=F_CPU/1024/100;							//Set 157 as Compare Value when Timer Reach this Number then will output compare fired every 10 ms
	//OCR0A=157;
	
	//4) Enable Output Compare Interrupt by using Register [TIMSK0] with bit0 [OCIE0A] For Output Compare
	//TIMSK0 |= (1<<OCIE0A);							//Enable Output Compare Interrupt To Overflow when Compare Matched with Timer Value
	
	//5) Enable Global Interrupt inside Register (SREG) Bit7 (I)
	sei();											//Enable Global Interrupt
	//cli();										//Disable Global Interrupt [Default]
	
	//6) Check if Interrupt Occurred when Output Compare Matched by check value of Register [OCF0A] with bit [OCF0A]
	//(OCF0A &(1<<OCF0A))==1							//Timer Interrupt Overflow Occured
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
												//Define LED1 -------------
	DDRD |= (1<<DDD0);							//Define Pin 0 on Port D as Output [1] [|] [OR]
	PORTD &= ~(1<<PORTD0);						//RESET Pin 0 on Port D as LOW [0] [&] [AND]
	
  												//Define LED2 -------------
	DDRD |= (1<<DDD1);							//Define Pin 1 on Port D as Output [1] [|] [OR]
	PORTD &= ~(1<<PORTD1);						//RESET Pin 1 on Port D as LOW [0] [&] [AND]
	*/
												//Define Timer Out Pin PD4 -------------
	//DDRD |= (1<<DDD4);							//Define Pin 4 on Port D as Output [1] [|] [OR]
	//PORTD &= ~(1<<PORTD4);						//RESET Pin 4 on Port D as LOW [0] [&] [AND]
	
	/*											//Define B0 as [Pull-Up]
	DDRB &= ~(1<<DDB0);							//Define Pin 0 on Port B as Input [0] [&] [AND]
	PORTB |= (1<<PORTB0);						//Set Pin 0 on Port B as HIGH [1] [|] [OR]
	
												//Define C0 as [Pull-Up]
	DDRC &= ~(1<<DDC0);							//Define Pin 0 on Port C as Input [0] [&] [AND]
	PORTC |= (1<<PORTC0);						//Set Pin 0 on Port C as HIGH [1] [|] [OR]
	

												//Define D7 as [Pull-Up]
	DDRD &= ~(1<<DDD7);							//Define Pin 7 on Port D as Input [0] [&] [AND]
	PORTD |= (1<<PORTD7);						//Set Pin 7 on Port D as HIGH [1] [|] [OR]
												
	*/
	
	//Loop -----------------
	
	while (1)
	{
	
	}
	
}

//Interrupt Service For Timer 0 as Compare it will fire every 10 ms when Timer Matched with Compare Value
ISR (TIMER0_COMPA_vect)
{
	//PORTD ^= (1<<PORTD4);
}


