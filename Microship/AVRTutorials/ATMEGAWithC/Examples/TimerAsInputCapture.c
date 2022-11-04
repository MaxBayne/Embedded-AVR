#define F_CPU 16000000UL //Define Frequency Of CPU 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int InputCapture_main(void)
{
	//Timer as Input Capture 
	
	/*
	Mean When Input Signal over Pin [PB0] then Processor will copy the Current value of Timer1 as 16 bit 
	From Timer1 Register called [TCNT1H] as High Byte , [TCNT1L] as Low Byte
	To Input Capture Register called [ICR1H] for High Byte, [ICR1L] for Low Byte
	Then Fire Input Capture Interrupt called [ICIE1] and Timer Overflow Interrupt Called [TOIE1]
	*/
	/*
	- ATMEGA328P has 1 Timer supported Input Capture
		Timer1 is 16 bit counter that Support Input Capture [PB0] (ICP1)
		Timer1 can Count From 0:65536
		
	- Timer1 Counter Register that Count inside it called [TCNT1H] for High Byte,[TCNT1L] for Low Byte its 16 bit Register
	- Input Capture Register that Processor copy Value of [TCNT1] and save it inside Register [ICR1H] for High Byte, [ICR1L] for Low Byte when Input Capture Get Signal over Pin [PB0](ICP1)
	- Config Input Capture using Register [TCCR1B] with bits below
		Bit7 (ICNC1 as Input Capture Noise Canceler) : set this Bit Will Enable Noise Canceler to Filter Signal Inputed over Pin [PB0] 
		Bit6 (ICES1 as Input Capture Edge Select) : set this Bit Will use Rising Edge For Input Capture , clear it will use Falling Edge (Default)
		Bit4 (WGM13 as Wave Generation Mode)
		Bit3 (WGM12 as Wave Generation Mode)
		Bit2 (CS12) : using For Set Prescalar
		Bit1 (CS11) : using For Set Prescalar
		Bit0 (CS10) : using For Set Prescalar
		
	- Set Prescalar using Register [TCCR1B] with Bits (CS12[2],CS11[1],CS10[0])
		CS12=0,CS11=0,CS10=1 : for No Prescaling
		CS12=0,CS11=1,CS10=0 : for 8 prescaler
		CS12=0,CS11=1,CS10=1 : for 64 prescaler
		CS12=1,CS11=0,CS10=0 : for 256 prescaler
		CS12=1,CS11=0,CS10=1 : for 1024 prescaler
		CS12=1,CS11=1,CS10=0 : for External Clock Source on T1 pin on Falling Edge
		CS12=1,CS11=1,CS10=1 : for External Clock Source on T1 pin on Rising Edge
		
		
	- Enable Interrupt for Input Capture by using Register [TIMSK1]
		Bit 5 as [ICIE1] to enable Input Capture Interrupt
		Bit 0 as [TOIE1] to enable Timer 1 Overflow Interrupt
		
	- Enable Global Interrupt for using Interrupt
		call Function sei(); For Enable Global Interrupt
		call Function cli(); For Disable Global Interrupt
	
	- Detect Input Capture Interrupt Fired used Flag inside Register [TIFR1]
		if bit 5 called [ICF1] as Input Capture Interrupt Flag is set mean Interrupt Fired otherwise not interrupt
		
	*/
	//--------------------------------------------------------------------
	
	
	//Setup ---------------
	
	
	//1) Set Input Capture Pin [PB0] as Input Direction
	DDRB &= ~(1<<DDB0);									//Make Pin [PB0] as Input
	
	
	//2) Set Timer1 as Normal Mode using Wave Generation Mode Bits inside Register [TCCR1A] with bits [WGM10(0),WGM11(1)] and Register [TCCR1B] with bits [WGM12]
	TCCR1A &= ~(1<<WGM11) & (1<<WGM10);					//using Normal Mode
	
	//3) Set Input Capture Noise Canceler For Filter Input Signal over pin [PB0]
	//TCCR1B |= (1<<ICNC1);								//Enable Input Capture Noise Canceler
	TCCR1B &= ~(1<<ICNC1);								//Disable Input Capture Noise Canceler (Default)

	//4) Set Edge For Input Signal to work Capture with rising or Falling Edge over pin [PB0]
	TCCR1B |= (1<<ICES1);								//work with Rising Edge will fire Input Capture over pin [PB0]
	//TCCR1B &= ~(1<<ICES1);							//work with Falling Edge will fire Input Capture over pin [PB0] (Default)
	
		
	//5) Set Clock Source with Prescalar For Timer1 by using Register TCCR1B with Bits (CS12-CS11-CS10)
														
	//use External Clock Frequency 16 MHZ With No Prescaling
	//TCCR1B &= ~(1<<CS12) & (1<<CS11);				//Clear CS12,CS11 input Capture
	//TCCR1B |=  (1<<CS10);							//Set CS10
	
	//use External Clock Frequency With Prescaler 8 mean 16/8 = 2 MHZ input Capture
	TCCR1B &= ~(1<<CS12) & (1<<CS10);				//Clear CS12,CS10
	TCCR1B |=  (1<<CS11);							//Set CS11
	
	//use External Clock Frequency With Prescaler 64 mean 16/64 = .25 MHZ input Capture
	//TCCR1B &= ~(1<<CS12);							//Clear CS12
	//TCCR1B |=  (1<<CS11) | (1<<CS10);				//Set CS11 , CS10
	
	//use External Clock Frequency With Prescaler 256 mean 16/256 = .06 MHZ input Capture
	//TCCR1B &= ~(1<<CS11) & (1<<CS10);				//Clear CS11,CS10
	//TCCR1B |=  (1<<CS12);							//Set CS12
	
	//use External Clock Frequency With Prescaler 1024 mean 16/1024 = .015 MHZ input Capture
	//TCCR1B &= ~(1<<CS11);							//Clear CS11
	//TCCR1B |=  (1<<CS12) | (1<<CS10);					//Set CS12 , CS10
	
	//use External Clock Source On Pin T0 (PD4) , Clock On Falling Edge
	//TCCR1B &= ~(1<<CS10);							//Clear CS10
	//TCCR1B |=  (1<<CS12) |  (1<<CS11);				//Set CS12 , CS11
	
	//use External Clock Source On Pin T0 (PD4) , Clock On Rising Edge
	//TCCR1B &= ~(1<<CS10);							//Clear CS10
	//TCCR1B |=  (1<<CS12) |  (1<<CS11);				//Set CS12 , CS11

	
	//6) Enable Input Capture Interrupt by using Register [TIMSK1] with bit5 [ICIE1] For Input Capture
	//TIMSK1 |= (1<<ICIE1);							//Enable Input Capture Interrupt To Overflow when Input Signal over Pin [PB0] with selected Edge
	//TIMSK1 |= (1<<TOIE1);							//Enable Timer 1 Overflow Interrupt when timer1 reached to max value for timer1=65536
	
	//7) Enable Global Interrupt inside Register (SREG) Bit7 (I)
	sei();											//Enable Global Interrupt
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

short isFirst=1;
volatile uint16_t start;
volatile uint16_t end;
long OverflowCount;
long clk;
long freq;
float tickTime;

//Interrupt Service For Timer 1 as Timer/Counter it will fire every 10 ms
ISR (TIMER1_OVF_vect)
{
	OverflowCount++;							//when timer1 overflow when reached to 65536
}


//Interrupt Service For Input Capture over pin [PB0] with selected edge [Rising Edge]
ISR (TIMER1_CAPT_vect)
{ 
	
	//Read Value of Input Capture Register that Represent Timer1 Value
	//ICR1 as 16 bit
	//ICR1H as Most Significant Byte
	//ICR1L as Least Significant Byte

	if(isFirst)
	{
		//First Input Capture
		start=ICR1;
		isFirst=0;
	}
	else
	{
		//Second Input Capture
		end=ICR1;	
		
		//Get the time of One Cyle (Tick)
		//Freq(inputCapture)=Freq(crystal)/Prescalar = 16MHZ/8=2 MHZ
		//Freq(inputCapture)=2 MHZ
		//Time=1/Freq
		//Time=1/2=0.5 uS
		tickTime=0.5;
		
		//Calculate the Time of First Input Capture Rising Signal and Last Input Capture Rising Signal
		clk=(end+OverflowCount*65536)-start;
		//clk=clk * TickTime;
		clk=clk * tickTime;
		
		//Calculate Freq
		freq=1/clk;
		
		
		OverflowCount=0;
	}
	
}




