#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "Timers/Common"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"


#include "Interrupts/GlobalInterrupt.h"
#include "Interrupts/TimerInterrupt.h"


//Function Pointer For Timers Overflow Callback Delegates
void(*Timer_0_Overflow_Callback_Ptr)();
void(*Timer_1_Overflow_Callback_Ptr)();
void(*Timer_2_Overflow_Callback_Ptr)();

//Function Pointer For Timers Compare Match Callback Delegates
void(*Timer_0_Compare_Match_Callback_Ptr)();
void(*Timer_1_Compare_Match_A_Callback_Ptr)();
void(*Timer_1_Compare_Match_B_Callback_Ptr)();
void(*Timer_2_Compare_Match_Callback_Ptr)();

//Function Pointer For Timers Capture Event Callback Delegates
void(*Timer_1_Capture_Event_Callback_Ptr)();


//Hold Interrupt Info For Timer0
static InterruptInfo TimerInterrupt::Timer0;

//Hold Interrupt Info For Timer1
static InterruptInfo TimerInterrupt::Timer1;

//Hold Interrupt Info For Timer2
static InterruptInfo TimerInterrupt::Timer2;


//Enable Timer Interrupt For Active Timer ,Mode  and Set Function Callback when Interrupt Fired
static void TimerInterrupt::Enable_Interrupt(TIMER_SELECTOR timer,MODE_SELECTOR mode,InterruptInfo interruptInfo,void(* functionPtr)())
{
	/*
	-------------------------
	Enable Timer Interrupt
	-------------------------
	- To Enable Timer Interrupt we use Register [TIMSK] and Set Bit

	- To Enable Timer Overflow Interrupt we Set Bit On :
	TOIE0 : (Bit0 Timer0 Overflow Interrupt Enable)
	TOIE1 : (Bit2 Timer1 Overflow Interrupt Enable)
	TOIE2 : (Bit6 Timer2 Overflow Interrupt Enable)
	
	- To Enable Timer Output Compare Interrupt we Set Bit On :
	OCIE0 : (Bit1 Timer0 Output Compare Interrupt Enable)
	OCIE1B : (Bit3 Timer1 Channel B Output Compare Interrupt Enable)
	OCIE1A : (Bit4 Timer1 Channel A Output Compare Interrupt Enable)
	OCIE2 : (Bit7 Timer2 Output Compare Interrupt Enable)
	
	- To Enable Timer1 Input Capture Flag Interrupt we Set Bit On :
	TICIE1 : (Bit5 Timer1 Input Capture Interrupt Enable)
	
	-------------------------
	Enable Global Interrupt
	-------------------------
	
	*/

	//Config Log 
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);

	//Enable Global Interrupt
	GlobalInterrupt::Enable_Global_Interrupt();

	//Enable Timer Interrupt
	switch(timer)
	{
		case TIMER_SELECTOR::TIMER_0 :
		
		if(mode==MODE_SELECTOR::MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 0
			
			_log.WriteLine("Enabled Timer0 Interrupt (Noraml Mode)");


			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,0); //[TOIE0]

			
			Timer_0_Overflow_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer0=interruptInfo;
			
		}
		else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 0
			
			_log.WriteLine("Enabled Timer0 Interrupt (Output Compare Mode)");

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,1); //[OCIE0]

			Timer_0_Compare_Match_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer0=interruptInfo;

		}
		
		
		break;
		
		case  TIMER_SELECTOR::TIMER_1:
		
		if(mode==MODE_SELECTOR::MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 1
			
			
			_log.WriteLine("Enabled Timer1 Interrupt (Noraml Mode)");
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,2); //[TOIE1]
			Timer_1_Overflow_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer1=interruptInfo;

			
		}
		else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 1 A,B Channel
			
			_log.WriteLine("Enabled Timer1 Interrupt (Output Compare Mode)");

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,3); //For Channel B  [OCIE1B]
			Timer_1_Compare_Match_B_Callback_Ptr = functionPtr;
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,4); //For Channel A  [OCIE1A]
			Timer_1_Compare_Match_A_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer1=interruptInfo;

			
		}
		else if(mode==MODE_SELECTOR::MODE_INPUT_CAPTURE_FLAG)
		{
			_log.WriteLine("Enabled Timer1 Interrupt (Input Capture Mode)");

			//For Input Capture Flag Timer 1
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,5);  //[TICIE1]
			Timer_1_Capture_Event_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer1=interruptInfo;
			
		}
		
		break;
		
		case  TIMER_SELECTOR::TIMER_2:
		
		if(mode==MODE_SELECTOR::MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 2
			
			_log.WriteLine("Enabled Timer2 Interrupt (Noraml Mode)");

			

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,6); //[TOIE2]
			Timer_2_Overflow_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer2=interruptInfo;
			

			
		}
		else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 2
			
			_log.WriteLine("Enabled Timer2 Interrupt (Output Compare Mode)");

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,7); //[OCIE2]
			Timer_2_Compare_Match_Callback_Ptr = functionPtr;

			TimerInterrupt::Timer2=interruptInfo;
			
		}
		
		break;
	}
	
}

//Disable Timer Interrupt For Timer,Mode
static void TimerInterrupt::Disable_Interrupt(TIMER_SELECTOR timer,MODE_SELECTOR mode)
{
	/*
	-------------------------
	Disable Timer Interrupt
	-------------------------
	- To disable Timer Interrupt we use Register [TIMSK] and Clear Bit

	- To disable Timer Overflow Interrupt we Clear Bit On :
	TOIE0 : (Bit0 Timer0 Overflow Interrupt Enable)
	TOIE1 : (Bit2 Timer1 Overflow Interrupt Enable)
	TOIE2 : (Bit6 Timer2 Overflow Interrupt Enable)
	
	- To disable Timer Output Compare Interrupt we Clear Bit On :
	OCIE0 : (Bit1 Timer0 Output Compare Interrupt Enable)
	OCIE1B : (Bit3 Timer1 Channel B Output Compare Interrupt Enable)
	OCIE1A : (Bit4 Timer1 Channel A Output Compare Interrupt Enable)
	OCIE2 : (Bit7 Timer2 Output Compare Interrupt Enable)
	
	- To disable Timer1 Input Capture Flag Interrupt we Clear Bit On :
	TICIE1 : (Bit5 Timer1 Input Capture Interrupt Enable)
	
	-------------------------
	Clear Interrupt Flag
	-------------------------
	*/
	
	//Config Log 
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);

	//Disable Timer Interrupt
	switch(timer)
	{
		case TIMER_SELECTOR::TIMER_0 :
		
		if(mode==MODE_SELECTOR::MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 0
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,0); //[TOIE0]

			_log.WriteLine("Disabled Timer0 Interrupt (Noraml Mode)");
		}
		else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 0
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,1); //[OCIE0]

			_log.WriteLine("Disabled Timer0 Interrupt (Output Compare Mode)");
		}
		
		
		break;
		
		case  TIMER_SELECTOR::TIMER_1:
		
		if(mode==MODE_SELECTOR::MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 1
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,2); //[TOIE1]

			_log.WriteLine("Disabled Timer1 Interrupt (Noraml Mode)");
		}
		else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 1 A,B Channel
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,3); //For Channel B  [OCIE1B]
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,4); //For Channel A  [OCIE1A]

			_log.WriteLine("Disabled Timer1 Interrupt (Output Compare Mode)");
		}
		else if(mode==MODE_SELECTOR::MODE_INPUT_CAPTURE_FLAG)
		{
			//For Input Capture Flag Timer 1
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,5);  //[TICIE1]

			_log.WriteLine("Disabled Timer1 Interrupt (Input Capture Mode)");
		}
		
		break;
		
		case  TIMER_SELECTOR::TIMER_2:
		
		if(mode==MODE_SELECTOR::MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 2
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,6); //[TOIE2]

			_log.WriteLine("Disabled Timer2 Interrupt (Noraml Mode)");
		}
		else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 2
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,7); //[OCIE2]

			_log.WriteLine("Disabled Timer2 Interrupt (Output Compare Mode)");
		}
		
		break;
	}
	
	

	//Clear Timer Interrupt Flag For Active Timer and Mode
	Clear_Interrupt_Flag(timer,mode);
	
}

//Clear Interrupt Flag For Timer , Mode when Interrupt fired we clear it inside ISR to avoid ReEnter ISR Again
static void TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR timer,MODE_SELECTOR mode)
{
	/*
	-----------------------
	Timer Interrupt Flag
	-----------------------

	- Interrupt Flag is Something Say that interrupt request generated mean Trigger Fired For Interrupt
	- To read the Status of Flag Interrupt we use Register (TIFR) (Timer Interrupt Flag Register)
	
	- On Normal Mode When Timer Value Register reach to max value of register then occured Overflow and interrupt fired
	Timer0 : TOV0 (Bit0 on Register TIFR)
	Timer1 : TOV1 (Bit2 on Register TIFR)
	Timer2 : TOV2 (Bit6 on Register TIFR)

	- On CTC Mode (Compare Match)
	Timer0 : OCF0 (Bit1 on Register TIFR)
	Timer1 : OCF1A (Bit4 on Register TIFR)
	Timer1 : OCF1B (Bit3 on Register TIFR)
	Timer2 : OCF2 (Bit7 on Register TIFR)

	- On Input Capture Flag For Timer1 Only
	Timer1 : ICF1 (Bit5 on Register TIFR)
	

	- when Timer Interrupt Triggered then Interrupt Flag Become 1 and Processor Jump TO Vector Table and Get the Interrupt Service Routine For That
	Interrupt and Execute it and when Finished then Clear Interrupt Flag inside Register (TIFR) to Avoid ReEnter the Service Routine Again

	*/
	
	//Config Log 
	//Logs _log=Logs(); 
	//_log.Initialize(LOGS_SOURCE_UART0);

	switch(timer)
	{
		case TIMER_SELECTOR::TIMER_0 :
		
			if(mode==MODE_SELECTOR::MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 0 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,0); //[TOV0]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 0 (Normal Mode)");
			}
			else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
			{
				//For Output Compare Timer 0 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,1); //[OCF0]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 0 (Output Compare Mode)");
			}
		
		
		break;
		
		case  TIMER_SELECTOR::TIMER_1 :
		
			if(mode==MODE_SELECTOR::MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 1 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,2); //[TOV1]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 1 (Normal Mode)");
			}
			else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
			{
				//For Output Compare Timer 1 A,B Channel
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,3); //For Channel B  [OCF1B]
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,4); //For Channel A  [OCF1A]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 1 (Output Compare Mode)");
			}
			else if(mode==MODE_SELECTOR::MODE_INPUT_CAPTURE_FLAG)
			{
				//For Input Capture Flag Timer 1 
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,5);  //[ICF1]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 1 (Input Capture Mode)");
			}
		
		break;
		
		case  TIMER_SELECTOR::TIMER_2:
		
			if(mode==MODE_SELECTOR::MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 2
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,6); //[TOV2]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 2 (Normal Mode)");
			}
			else if(mode==MODE_SELECTOR::MODE_COMPARE_CTC)
			{
				//For Output Compare Timer 2
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,7); //[OCF2]

				//_log.WriteLine("Cleared Interrupt Flag For Timer 2 (Output Compare Mode)");
			}
		
		break;
	}
	
}

//Read Interrupt Flag For Timer , Mode , return 1 when Flag On or 0 when flag off
static uint8 TimerInterrupt::Read_Interrupt_Flag(TIMER_SELECTOR timer,MODE_SELECTOR mode)
{

	switch (mode)
	{
		case MODE_SELECTOR::MODE_NORMAL :

			if(timer==TIMER_SELECTOR::TIMER_0)
			{
				//For Normal Mode (Overflow) Timer 0 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,0); //[TOV0]
			}
			else if(timer==TIMER_SELECTOR::TIMER_1)
			{
				//For Normal Mode (Overflow) Timer 1 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,2); //[TOV1]
			}
			else if(timer==TIMER_SELECTOR::TIMER_2)
			{
				//For Normal Mode (Overflow) Timer 2 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,6); //[TOV2]
			}

		break;

		case MODE_SELECTOR::MODE_COMPARE_CTC :

			if (timer == TIMER_SELECTOR::TIMER_0)
			{
				// For Output Compare Timer 0
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR, 1); //[OCF0]
			}
			else if (timer == TIMER_SELECTOR::TIMER_1_A)
			{
				//For Output Compare Timer 1 A Channel
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,4); //For Channel A  [OCF1A]
			}
			else if (timer == TIMER_SELECTOR::TIMER_1_B)
			{
				//For Output Compare Timer 1 B Channel
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,3); //For Channel B  [OCF1B]
			}
			else if (timer == TIMER_SELECTOR::TIMER_2)
			{
				//For Output Compare Timer 2
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,7); //[OCF2]
			}

		break;

		case MODE_SELECTOR::MODE_INPUT_CAPTURE_FLAG :
			if(timer==TIMER_SELECTOR::TIMER_1)
			{
				//For Input Capture Flag Timer 1 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,5);  //[ICF1]
			}
		break;
	}

}

//Stop Timer From External Location Like ISR Function
static void TimerInterrupt::Stop_Timer(TIMER_SELECTOR timer)
{
	//Config Log 
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);


	//Stop Timer By Select No Clock inside Clock Source
	switch (timer)
	{
		case TIMER_0: // Use Timer Counter Control Register [TCCR0]

			BITWISE_CLEAR_BIT(TIMER_REG_TCCR0, CS00); // CS00 [0] = 0
			BITWISE_CLEAR_BIT(TIMER_REG_TCCR0, CS01); // CS01 [1] = 0
			BITWISE_CLEAR_BIT(TIMER_REG_TCCR0, CS02); // CS02 [2] = 0

			_log.WriteLine("Disabled Timer0 (Noraml Mode)");

			break;

		case TIMER_1: // Use Timer Counter Control Register [TCCR1A]

			BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A, CS10); // CS10 [0] = 0
			BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A, CS11); // CS11 [1] = 0
			BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A, CS12); // CS12 [2] = 0

			_log.WriteLine("Disabled Timer1 (Noraml Mode)");

			break;

		case TIMER_2: // Use Timer Counter Control Register [TCCR2]

			BITWISE_CLEAR_BIT(TIMER_REG_TCCR2, CS20); // CS20 [0] = 0
			BITWISE_CLEAR_BIT(TIMER_REG_TCCR2, CS21); // CS21 [1] = 0
			BITWISE_CLEAR_BIT(TIMER_REG_TCCR2, CS22); // CS22 [2] = 0

			_log.WriteLine("Disabled Timer2 (Noraml Mode)");

			break;
	}
}


//---------------------------------------------------------
//Timer Interrupt Services Routine From Vector Table :
//---------------------------------------------------------

#pragma region Interrupts For Overflow

//Interrupt Service Routine For Timer 0 Overflow

ISR(TIMER0_OVF_vect)
{
	//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
	TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_0,MODE_SELECTOR::MODE_NORMAL);
	
	//Re Initial Timer Value To Start Count From it
  	//TCNT0=0;
	//TIMER_REG_TCNT0 = BITWISE_GET_LOW_BYTE(TimerInterrupt::Timer0.InitialTimerValue); (Make Delay So Cancel it)

	//Increase Overflow Counter
  	TimerInterrupt::Timer0.CurrentOverflowCounts++;

 	//Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  	if(TimerInterrupt::Timer0.CurrentOverflowCounts==TimerInterrupt::Timer0.RequiredOverflowCounts)
  	{
		//Timer Reach to overflow count needed to wait for 1 second
    	TimerInterrupt::Timer0.CurrentOverflowCounts=0;

		//Call Function inside Callback
		if(Timer_0_Overflow_Callback_Ptr!=0)
		{
			Timer_0_Overflow_Callback_Ptr();
		}
		
		//If Timer Mode Duration is Once then Stop Timer otherwise will be Repeated
		if(TimerInterrupt::Timer0.DurationMode==DURATION_MODE::ONCE)
		{
			//Stop Timer Here ---------------------
			TimerInterrupt::Stop_Timer(TIMER_SELECTOR::TIMER_0);

			TimerInterrupt::Disable_Interrupt(TIMER_SELECTOR::TIMER_0,MODE_SELECTOR::MODE_NORMAL);
			
		}
    	
	}
}

//Interrupt Service Routine For Timer 1 Overflow
ISR(TIMER1_OVF_vect)
{
	//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
	TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_NORMAL);
	
	//Re Initial Timer Value To Start Count From it
  	//TCNT0=0;
	TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(TimerInterrupt::Timer1.InitialTimerValue);
	TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(TimerInterrupt::Timer1.InitialTimerValue);

	//Increase Overflow Counter
  	TimerInterrupt::Timer1.CurrentOverflowCounts++;

 	//Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  	if(TimerInterrupt::Timer1.CurrentOverflowCounts==TimerInterrupt::Timer1.RequiredOverflowCounts)
  	{
		//Timer Reach to overflow count needed to wait for 1 second
    	TimerInterrupt::Timer1.CurrentOverflowCounts=0;

		//Call Function inside Callback
		if(Timer_1_Overflow_Callback_Ptr!=0)
		{
			Timer_1_Overflow_Callback_Ptr();
		}
		
		//If Timer Mode Duration is Once then Stop Timer otherwise will be Repeated
		if(TimerInterrupt::Timer1.DurationMode==DURATION_MODE::ONCE)
		{
			//Stop Timer Here ---------------------
			TimerInterrupt::Stop_Timer(TIMER_SELECTOR::TIMER_1);

			TimerInterrupt::Disable_Interrupt(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_NORMAL);
			
		}
    	
	}
}

//Interrupt Service Routine For Timer 2 Overflow

ISR(TIMER2_OVF_vect)
{
	

	//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
	TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_2,MODE_SELECTOR::MODE_NORMAL);
	
	//Re Initial Timer Value To Start Count From it
  	//TCNT0=0;
	TIMER_REG_TCNT2 = BITWISE_GET_LOW_BYTE(TimerInterrupt::Timer2.InitialTimerValue);

	//Increase Overflow Counter
  	TimerInterrupt::Timer2.CurrentOverflowCounts++;

 	//Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  	if(TimerInterrupt::Timer2.CurrentOverflowCounts==TimerInterrupt::Timer2.RequiredOverflowCounts)
  	{
		GPIO::Pin_Toggle(GPIO_IO_PA7);

		//Timer Reach to overflow count needed to wait for 1 second
    	TimerInterrupt::Timer2.CurrentOverflowCounts=0;

		//Call Function inside Callback
		if(Timer_2_Overflow_Callback_Ptr!=0)
		{
			Timer_2_Overflow_Callback_Ptr();
		}
		
		//If Timer Mode Duration is Once then Stop Timer otherwise will be Repeated
		if(TimerInterrupt::Timer2.DurationMode==DURATION_MODE::ONCE)
		{
			//Stop Timer Here ---------------------
			TimerInterrupt::Stop_Timer(TIMER_SELECTOR::TIMER_2);

			TimerInterrupt::Disable_Interrupt(TIMER_SELECTOR::TIMER_2,MODE_SELECTOR::MODE_NORMAL);
		}
    	
	}
}

#pragma endregion Interrupts For Overflow

#pragma region Interrupts For Compare Match CTC

//Interrupt Service Routine For Timer 0 Compare Match CTC
ISR(TIMER0_COMP_vect)
{
	if(Timer_0_Compare_Match_Callback_Ptr!=0)
	{
		//Call Function inside Callback
		Timer_0_Compare_Match_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_0,MODE_SELECTOR::MODE_COMPARE_CTC);
	}
}

//Interrupt Service Routine For Timer 1 Channel A Compare Match CTC
ISR(TIMER1_COMPA_vect)
{
	if(Timer_1_Compare_Match_A_Callback_Ptr!=0)
	{
		//Call Function inside Callback
		Timer_1_Compare_Match_A_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_COMPARE_CTC);
	}
}

//Interrupt Service Routine For Timer 1 Channel B Compare Match CTC
ISR(TIMER1_COMPB_vect)
{
	if(Timer_1_Compare_Match_B_Callback_Ptr!=0)
	{
		//Call Function inside Callback
		Timer_1_Compare_Match_B_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_COMPARE_CTC);
	}
}

//Interrupt Service Routine For Timer 2 Compare Match CTC
ISR(TIMER2_COMP_vect)
{
	if(Timer_2_Compare_Match_Callback_Ptr!=0)
	{
		//Call Function inside Callback
		Timer_2_Compare_Match_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_2,MODE_SELECTOR::MODE_COMPARE_CTC);
	}
}


#pragma endregion Interrupts For Overflow

#pragma region Interrupts For Capture Event [Timer1 Only]

//Interrupt Service Routine For Timer 1 Capture Event
ISR(TIMER1_CAPT_vect)
{
	if(Timer_1_Capture_Event_Callback_Ptr!=0)
	{
		//Call Function inside Callback
		Timer_1_Capture_Event_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_INPUT_CAPTURE_FLAG);
	}
}


#pragma endregion Interrupts For Overflow