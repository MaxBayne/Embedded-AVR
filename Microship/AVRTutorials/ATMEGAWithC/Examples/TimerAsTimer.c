#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int TimerAsTimer_main(void)
{
	//Timer as Timer
	
	/*
	- ATMEGA328P have 3 Timer
		Timer0:is 8 Bit so it count from 0:255
		Timer1:is 16 Bit so it count from 0:65535
		Timer2:is 8 Bit so it count from 0:255
	- Timer Can Work with many Mode
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
	- To Set Initial Value For Timer To Begin With (PreLoader) use Register [TCNT0] its 8 bit=1byte Register
	- Calculate the Preloader Value For Timer use the Below Equation
		TCNT0=255-(FrequencyExternal/Prescaler/DesiredFrequency)
		TCNT0=255-(16 MHZ/1024/100 HZ)=99
		So when set Timer 0 =99 then it will overflow every 10ms
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
	
		 
	//1) Select Clock Source For Timer0 by using Register TCCR0B with Bits (CS02-CS01-CS00)
	
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

	
	
	//2) Set Initial Value For Timer To Count From it , the Default value is zero
	//TCNT0=255-(FrequencyExternal/Prescaler/DesiredFrequency)
	//TCNT0=255-(16 MHZ/1024/100 HZ)=99
	//TCNT0=0;										//Set 0 as Start Count Value For Timer
	//TCNT0=255-(F_CPU/256/100);						//Set 99 as Start Count Value For Timer to insure that timer will overflow every 10 ms
	//TCNT0=99;
	
	//3) Enable Interrupt With Time by using Register [TIMSK0] with bit0 [TOIE0]
	//TIMSK0 |= (1<<TOIE0);							//Enable Timer 0 Overflow Interrupt
	
	//4) Enable Global Interrupt inside Register (SREG) Bit7 (I)
	sei();											//Enable Global Interrupt
	//cli();										//Disable Global Interrupt [Default]
	
	//5) Check if Interrupt Occurred when Overflow by check value of Register [TIFR0] with bit [TOV0]
	//(TIFR0 &(1<<TOV0))==1							//Timer Interrupt Overflow Occured
	
	
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

//Interrupt Service For Timer 0 as Timer/Counter it will fire every 10 ms
ISR (TIMER0_OVF_vect)
{
	
	
	//TCNT0=255-(F_CPU/1024/100);					//Set 99 as Start Count Value For Timer to insure that timer will overflow every 10 ms
	//TCNT0=99;
	
	//PORTD ^= (1<<PORTD4);
	
}


