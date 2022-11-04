#include "Common/Common.h"
#include "Common/Registers.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"

#include "Interrupts/GlobalInterrupt.h"
#include "Interrupts/TimerInterrupt.h"
#include "Timers/Timer.h"


#pragma region Constructors

Timer::Timer(TIMER_SELECTOR timer,CLOCK_SOURCE clock,uint64 frequency)
{
	_Timer = timer;
	_Clock=clock;
	_Frequency = frequency;
	
	//Set Timer Clock Source
	Config_Timer_Clock_Source(timer,clock);
}

#pragma endregion

#pragma region Methods

//Delay With Some Time By Timer
void Timer::Delay(float delayTime)
{
	//Start Timer as Normal Mode
	StartTimer(delayTime);

	//Wait For Timer To Overflow mean Timer Finish The Delay Time
	while (IsOverflow()==false);
	
	//Stop Timer and Clear Flag
	StopTimer();
}

//Start Timer as Normal Mode (Delay) with Some Period per Seconds Without using Interrupt ISR , just Monitor [IsOverflow]
void Timer::StartTimer(float delayTime)
{
	_DelayTime = delayTime;
	
	//Set Initial Value For Timer inside Register [TCNT] depend on Frequency,No of Clocks
	Config_Timer_Initial_Value(_Timer,_Frequency,delayTime);
	
	//Set Timer Mode To Normal/Overflow
	Config_Timer_Mode(_Timer,MODE_NORMAL);
	
	//Update Timer Status
	_IsRunning=true;
	_IsInterrupt=false;	
}


//Start Timer as Normal Mode (Delay) with Some Period per Seconds With using Interrupt To Fire function Callback
void Timer::StartTimer(float delayTime,void(*functionPtr)())
{
	_DelayTime = delayTime;
	

	//Set Initial Value For Timer inside Register [TCNT] depend on Frequency,No of Clocks
	Config_Timer_Initial_Value(_Timer,_Frequency,delayTime);
	
	//Set Timer Mode To Normal/Overflow
	Config_Timer_Mode(_Timer,MODE_NORMAL);

	//Enable Timer Interrupt
	switch (_Timer)
	{
		case TIMER_0:
			TimerInterrupt::Enable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_0,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL,functionPtr);
		break;

		case TIMER_1:
			TimerInterrupt::Enable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL,functionPtr);
		break;

		case TIMER_1_A:
			TimerInterrupt::Enable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL,functionPtr);
		break;

		case TIMER_1_B:
			TimerInterrupt::Enable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL,functionPtr);
		break;

		case TIMER_2:
			TimerInterrupt::Enable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_2,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL,functionPtr);
		break;
	}

	//Update Timer Status
	_IsRunning=true;
	_IsInterrupt=true;	
}

//Stop Active Timer
void Timer::StopTimer()
{
	//Stop Timer By Select No Clock inside Clock Source
	Config_Timer_Clock_Source(_Timer,CLOCK_SOURCE::CLOCK_STOPPED);

	//if Timer use Interrupt then Stop it
	if(_IsInterrupt)
	{
		switch (_Timer)
		{
			case TIMER_0:
				TimerInterrupt::Disable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_0,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
			break;

			case TIMER_1:
				TimerInterrupt::Disable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
			break;

			case TIMER_1_A:
				TimerInterrupt::Disable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
			break;

			case TIMER_1_B:
				TimerInterrupt::Disable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
			break;

			case TIMER_2:
				TimerInterrupt::Disable_Interrupt(INTERRUPT_TIMER::INTERRUPT_TIMER_2,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
			break;
		}
	}

	//update Timer Status
	_IsRunning=false;
	_IsInterrupt=false;

}


//Get Status of Active Timer (Running/Stopped)
bool Timer::IsRunning()
{
	return _IsRunning;
}

//Detect if Current Timer Use Interrupt or Not
bool Timer::IsInterrupt()
{
	return _IsInterrupt;
}

//Detect if Current Timer with Normal Mode is Overflow
bool Timer::IsOverflow()
{
	//Read Interrupt Timer Flag To Detect if OVerflow or Not [TIFR] Register with Bits [TOV0-TOV1-TOV2]
	switch (_Timer)
	{
		case TIMER_0:
			return TimerInterrupt::Read_Interrupt_Flag(INTERRUPT_TIMER::INTERRUPT_TIMER_0,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
		break;

		case TIMER_1:
			return TimerInterrupt::Read_Interrupt_Flag(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
		break;

		case TIMER_1_A:
			return TimerInterrupt::Read_Interrupt_Flag(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
		break;

		case TIMER_1_B:
			return TimerInterrupt::Read_Interrupt_Flag(INTERRUPT_TIMER::INTERRUPT_TIMER_1,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
		break;

		case TIMER_2:
			return TimerInterrupt::Read_Interrupt_Flag(INTERRUPT_TIMER::INTERRUPT_TIMER_2,INTERRUPT_MODE::INTERRUPT_MODE_NORMAL);
		break;
	}
}

#pragma endregion

#pragma region Helpers

//Config Timer Clock Source (NoClock-Clock-Clock/8-Clock/64-Clock/256-Clock/1024-ExternalClockFallingEdge-ExternalClockRisingEdge)
void Timer::Config_Timer_Clock_Source(TIMER_SELECTOR timer,CLOCK_SOURCE source)
{
	//Config Timer Clock Source by Using Register [TCCR] (Timer/Counter Control Register) with Bits [CS]

	/*
	D2  D1  D0
	0	0	0     No Clock Source (Timer Stopped)
	0	0	1     Clock (No Prescalling)
	0	1	0     Clock / 8
	0	1	1     Clock / 64
	1	0	0     Clock / 256
	1	0	1     Clock / 1024
	1	1	0     External Clk Source on T0 Falling Edge
	1	1	1     External Clk Source on T0 Rising Edge
	*/
	
	switch(timer)
	{
		case TIMER_0: //Use Timer Counter Control Register [TCCR0]
		
			if(source==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_No)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1
			}
			else if(source==CLOCK_PRESCAL_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1
			}
		
		break;
		
		case TIMER_1_A://Use Timer Counter Control Register [TCCR1A]
		
			if(source==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_No)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1
			}
			else if(source==CLOCK_PRESCAL_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1
			}
			
		
		break;
		
		case TIMER_1_B://Use Timer Counter Control Register [TCCR1B]
		
			if(source==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_No)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1
			}
			else if(source==CLOCK_PRESCAL_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1
			}
		
		
		break;
		
		case TIMER_2://Use Timer Counter Control Register [TCCR2]
		
			if(source==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_No)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0
			}
			else if(source==CLOCK_PRESCAL_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1
			}
			else if(source==CLOCK_PRESCAL_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1
			}
			else if(source==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1
			}
		
		break;
	}
}

//Calculate the Initial Value For Timer Depend on Frequency , Needed Delay Time Per Seconds and set it
void Timer::Config_Timer_Initial_Value(TIMER_SELECTOR timer,uint64 frequency,uint16 delayTime)
{
	//get the Time of One Cycle (Clock) Based On Frequency
	float timeOfOneClock = 1/frequency;
	
	//get How Many Clocks Need For Required Time (DelayTime)
	uint16 numberOfClocks = delayTime / timeOfOneClock;
	
	//Calculate the Initial Value For Timer if Required
	uint16 initialTimerValue=0;
	bool needOverflowCount=false;
	uint8 overflowCount=0;
	uint8 reminderValue=0;

	if(timer == TIMER_0 || timer == TIMER_2)
	{
		//For 8 Bit Timer Register
		
		if(numberOfClocks<=256)
		{
			//if required clocks lower than the overflow value for timer value register then will set initial timer for some value and after overflow then mean
			//timer reached to required time and count the number of clocks needed
			initialTimerValue = 256 - numberOfClocks;
			
			//Number of Clocks is Equal or Lower Than 256 then No Need For Overflow Count
			needOverflowCount = false;
			overflowCount=0;
			reminderValue=0;
		}
		else
		{
			// if required clocks is larger than 256 then we will use loop after every full overflow with some count
			initialTimerValue=0;
			
			//Number of Clocks is Greater than 256 then Need for Overflow Count and may be need reminder
			needOverflowCount = true;
			overflowCount = numberOfClocks / 256;
			reminderValue = numberOfClocks % 256;
		}
		
	}
	else if(timer == TIMER_1)
	{
		//For 16 Bit Timer Register
		
		if(numberOfClocks<=65536)
		{
			//if required clocks lower than the overflow value for timer value register then will set initial timer for some value and after overflow then mean
			//timer reached to required time and count the number of clocks needed
			initialTimerValue = 65536 - numberOfClocks;
			
			//Number of Clocks is Equal or Lower Than 65536 then No Need For Overflow Count
			needOverflowCount = false;
			overflowCount=0;
			reminderValue=0;
		}
		else
		{
			// if required clocks is larger than 256 then we will use loop after every full overflow with some count
			initialTimerValue=0;
			
			//Number of Clocks is Greater than 256 then Need for Overflow Count and may be need reminder
			needOverflowCount = true;
			overflowCount = numberOfClocks / 65536;
			reminderValue = numberOfClocks % 65536;
			
		}
	}
	
	//Set Initial Value For Timer
	Set_Timer_Initial_Value(timer,initialTimerValue);
	
	//Save Some Values
	_NumberOfClock = numberOfClocks;
	_InitialTimerValue = initialTimerValue;
	_OverflowCount = overflowCount;
	_NeedOverflowCount = needOverflowCount;
	_ReminderValue = reminderValue;
}

//Set Initial Value For Active Timer inside Register [TCNT]
void Timer::Set_Timer_Initial_Value(TIMER_SELECTOR timer,uint16 value)
{
	switch(timer)
	{
		case TIMER_0:
			TIMER_REG_TCNT0 = value;
		break;
		
		case TIMER_1:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(value);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(value);
		break;

		case TIMER_1_A:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(value);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(value);
		break;

		case TIMER_1_B:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(value);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(value);
		break;
		
		case TIMER_2:
			TIMER_REG_TCNT2 = value;
		break;
	}
}

//Config Timer Mode (Normal,Compare,Correct PWM,Fast PWM)
void Timer::Config_Timer_Mode(TIMER_SELECTOR timer,MODE_SELECTOR mode)
{
	switch(timer)
	{
		case TIMER_0: //Use Timer Counter Control Register [TCCR0]
		    
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 0
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 0
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 1
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 1
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 0
			}
		
		break;
		
		case TIMER_1_A://Use Timer Counter Control Register [TCCR1A]
			
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 0
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 0
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 1
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 1
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 0
			}
			
		break;
		
		case TIMER_1_B://Use Timer Counter Control Register [TCCR1B]
		
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 0
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 0
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 1
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 1
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 0
			}
		
		break;
		
		case TIMER_2://Use Timer Counter Control Register [TCCR2]
			
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 0
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 0
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 1
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 1
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 0
			}
			
		break;
	}
}


#pragma endregion