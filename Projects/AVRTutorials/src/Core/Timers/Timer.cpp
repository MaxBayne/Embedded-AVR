#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"
#include "Timers/Common"
#include "Timers/Timer.h"

#include "Interrupts/GlobalInterrupt.h"
#include "Interrupts/TimerInterrupt.h"



#pragma region Constructors

Timer::Timer(TIMER_SELECTOR timer,uint64 frequency)
{
	_log = Logs();
	_log.Initialize(LOGS_SOURCE_UART0);

	_Timer = timer;
	_Frequency = frequency;
	
	//Set Timer Clock Source (Stopped)
	Config_Timer_Clock_Source(timer,CLOCK_STOPPED);
	
}

#pragma endregion

#pragma region Methods

//Delay With Some Time By Timer
void Timer::Delay(float delayTime,TIME_UNIT unit)
{
	//Start Timer as Normal Mode
	StartTimer(delayTime,unit,DURATION_MODE::ONCE);

	_log.WriteLine("Timer Started");
	
	//Wait For Timer To Overflow mean Timer Finish The Delay Time
	while (IsOverflow()==false)
	{
		_log.WriteLine("Timer Running");
	};
	
	_log.WriteLine("Timer Stopped");

	//Stop Timer and Clear Flag
	StopTimer();
}

//Start Timer as Normal Mode (Delay) with Some Period per Seconds Without using Interrupt ISR , just Monitor [IsOverflow]
void Timer::StartTimer(float delayTime,TIME_UNIT unit,DURATION_MODE mode)
{
	_DelayTime = delayTime;

	//Set Initial Value For Timer inside Register [TCNT] depend on Frequency,No of Clocks
	//Config_Timer_Initial_Value(_Timer,_Frequency,delayTime,unit);

	//Set Timer Mode To Normal/Overflow
	Config_Timer_Mode(_Timer,MODE_NORMAL);
	
	//Set Timer Clock Source
	Config_Timer_Clock_Source(_Timer,_Clock);

	//Update Timer Status
	_IsRunning=true;
	_IsInterrupt=false;	
}


//Start Timer as Normal Mode (Delay) with Some Period per Seconds With using Interrupt To Fire function Callback
void Timer::StartTimer(float delayTime,TIME_UNIT unit,DURATION_MODE mode,void(*functionPtr)())
{
	_DelayTime = delayTime;
	
	//1) Get the Best Prescaler to achieve the delay time by select lowest overflow count prescaler
	PrescalerInfo bestPrescaler = Get_Best_Prescaler(_Timer,_Frequency,delayTime,unit);

	//2) Set Initial Value For Timer inside Register [TCNT] depend on (No of Clocks)
	Config_Timer_Initial_Value(_Timer,bestPrescaler);
	
	//3) Set Timer Mode To Normal/Overflow
	Config_Timer_Mode(_Timer,MODE_NORMAL);

	//4) Set Timer Clock Source With/Without Prescaler
	Config_Timer_Clock_Source(_Timer,_Prescaler.Prescaler);

	//5) Enable Timer Interrupt For Normal Mode
	InterruptInfo info;

	info.Time=_Prescaler.Time;
	info.TimeByMicroSecond=_Prescaler.TimeByMicroSecond;
	info.TimeUnit=_Prescaler.TimeUnit;
	info.InitialTimerValue=_InitialTimerValue;
	info.RequiredOverflowCounts=_Prescaler.RequiredOverflowCounts;
	info.RequiredReminderValue=_Prescaler.RequiredReminderValue;
	info.CurrentOverflowCounts=0;
	info.DurationMode=mode;

	TimerInterrupt::Enable_Interrupt(_Timer,MODE_NORMAL,info,functionPtr);
	
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
		TimerInterrupt::Disable_Interrupt(_Timer,_Mode);
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
	return TimerInterrupt::Read_Interrupt_Flag(_Timer,_Mode);
}

#pragma endregion

#pragma region Helpers

//[Tested]
//Convert Time From Any Unit To MicroSecond
uint64 Timer::Time_To_MicroSecond(float time,TIME_UNIT unit)
{
	//Convert Time To MicroSeconds
	switch (unit)
	{
		case TIME_UNIT::HOUR:
			return time * 60 * 60 * 1000 * 1000;
		break;

		case TIME_UNIT::MINUTE:
			return time * 60 * 1000 * 1000;
		break;

		case TIME_UNIT::SECOND:
			return time * 1000 * 1000;
		break;

		case TIME_UNIT::MILLI:
			return time * 1000;
		break;

		case TIME_UNIT::MICRO:
			return time;
		break;
	}

}

//[Tested]
//Calc All Prescaler info like Overflow Counts depend on Timer, Base Frequency,time by microSecond Unit,preScaler (clock_Source)
PrescalerInfo Timer::Get_Prescaler_Info(TIMER_SELECTOR timer,CLOCK_SOURCE preScaler,uint64 frequency,float time,TIME_UNIT unit)
{
	PrescalerInfo result;

	result.Time=time;
	result.TimeUnit = unit;
	result.BaseFrequency = frequency;
	result.Prescaler=preScaler;

	//Convert Time From Any Unit To MicroSeconds
	result.TimeByMicroSecond = Time_To_MicroSecond(time,unit);

	//Get the Final Frequency used with Timer
	switch (preScaler)
	{
		case CLOCK_PRESCAL_No:
			result.FinalFrequency = (float)frequency/1000000;
		break;

		case CLOCK_PRESCAL_8:
			result.FinalFrequency = (float)frequency/1000000/8;
		break;

		case CLOCK_PRESCAL_64:
			result.FinalFrequency = (float)frequency/1000000/64;
		break;

		case CLOCK_PRESCAL_256:
			result.FinalFrequency = (float)frequency/1000000/256;
		break;

		case CLOCK_PRESCAL_1024:
			result.FinalFrequency = (float)frequency/1000000/1024;
		break;
	}

	//Get time of One Clock (MicroSecond) tc=1/frequency
	result.TimeOfClock = 1.0/result.FinalFrequency;

	//Get the Number of Clocks  needed to achieve time
	result.NumberOfClocks = result.TimeByMicroSecond/result.TimeOfClock;

	//Get the Overflow Counts
	if(timer==TIMER_SELECTOR::TIMER_0 || timer==TIMER_SELECTOR::TIMER_2)
	{
		result.OverflowCounts= (float)result.NumberOfClocks / 256; //2^8
	} 
	else
	{
		result.OverflowCounts= (float)result.NumberOfClocks / 65536; //2^16
	}
	

	return result;
}

//[Tested]
//Select the Best Prescaler depend on Timer , Frequency , DelayTime , Time Unit and get the most accurated value
PrescalerInfo Timer::Get_Best_Prescaler(TIMER_SELECTOR timer , uint64 frequency,float delayTime,TIME_UNIT unit)
{
	//We Will Calc Overflow Counts For All PreScalers and Take the Lowest Overflow Counts With Less Deciemal
	
	//We Will Calc PrescalerInfo For Every Prescaler and get the lowest value
	PrescalerInfo prescaler_1024 = Get_Prescaler_Info(timer,CLOCK_PRESCAL_1024,frequency,delayTime,unit);
	PrescalerInfo prescaler_256 = Get_Prescaler_Info(timer,CLOCK_PRESCAL_256,frequency,delayTime,unit);
	PrescalerInfo prescaler_64 = Get_Prescaler_Info(timer,CLOCK_PRESCAL_64,frequency,delayTime,unit);
	PrescalerInfo prescaler_8 = Get_Prescaler_Info(timer,CLOCK_PRESCAL_8,frequency,delayTime,unit);
	PrescalerInfo prescaler_no = Get_Prescaler_Info(timer,CLOCK_PRESCAL_No,frequency,delayTime,unit);
	PrescalerInfo best_prescaler;

	//Get the Lowest overflow Counts from all prescalers
	float lowestCount=prescaler_1024.OverflowCounts;
	if(prescaler_1024.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_1024.OverflowCounts;
	}
	if(prescaler_256.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_256.OverflowCounts;
	}
	if(prescaler_64.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_64.OverflowCounts;
	}
	if(prescaler_8.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_8.OverflowCounts;
	}
	if(prescaler_no.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_no.OverflowCounts;
	}

	//Detect the Lowest Prescaler Info
	if(lowestCount==prescaler_1024.OverflowCounts)
	{
		best_prescaler= prescaler_1024;
	}
	else if(lowestCount==prescaler_256.OverflowCounts)
	{
		best_prescaler= prescaler_256;
	}
	else if(lowestCount==prescaler_64.OverflowCounts)
	{
		best_prescaler= prescaler_64;
	}
	else if(lowestCount==prescaler_8.OverflowCounts)
	{
		best_prescaler= prescaler_8;
	}
	else if(lowestCount==prescaler_no.OverflowCounts)
	{
		best_prescaler= prescaler_no;
	}


	/*
	_log.WriteText("Pre.1024 Overflow Counts = ");
	_log.WriteFloat(prescaler_1024.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.256 Overflow Counts = ");
	_log.WriteFloat(prescaler_256.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.64 Overflow Counts = ");
	_log.WriteFloat(prescaler_64.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.8 Overflow Counts = ");
	_log.WriteFloat(prescaler_8.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.1 Overflow Counts = ");
	_log.WriteFloat(prescaler_no.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Best Prescaler = ");
	_log.WriteFloat(best_prescaler.OverflowCounts);
	_log.NewLine();

	*/


	return best_prescaler;

}


//[Tested]
//Set Initial Value For Timer inside Register [TCNT] depend on (No of Clocks)
void Timer::Config_Timer_Initial_Value(TIMER_SELECTOR timer,PrescalerInfo prescaler)
{
	
	//Calculate the Initial Value For Timer if Required
	uint16 initialTimerValue=0;

	if(timer == TIMER_0 || timer == TIMER_2)
	{
		//For 8 Bit Timer Register
		
		if(prescaler.NumberOfClocks<=256)
		{
			//if required clocks lower than the overflow value for timer value register then will set initial timer for some value and after overflow then mean
			//timer reached to required time and count the number of clocks needed
			initialTimerValue = 256 - prescaler.NumberOfClocks;
			
			//Number of Clocks is Equal or Lower Than 256 then No Need For Overflow Count
			
			prescaler.RequiredOverflowCounts=1;
			prescaler.RequiredReminderValue=0;
		}
		else
		{
			// if required clocks is larger than 256 then we will use loop after every full overflow with some count
			initialTimerValue=0;
			
			//Number of Clocks is Greater than 256 then Need for Overflow Count and may be need reminder
			prescaler.RequiredOverflowCounts = prescaler.NumberOfClocks / 256;
			prescaler.RequiredReminderValue = prescaler.NumberOfClocks % 256;
		}

	}
	else if(timer == TIMER_1)
	{
		//For 16 Bit Timer Register
		
		if(prescaler.NumberOfClocks<=65536)
		{
			//if required clocks lower than the overflow value for timer value register then will set initial timer for some value and after overflow then mean
			//timer reached to required time and count the number of clocks needed
			initialTimerValue = 65536 - prescaler.NumberOfClocks;
			
			//Number of Clocks is Equal or Lower Than 65536 then No Need For Overflow Count
			prescaler.RequiredOverflowCounts=1;
			prescaler.RequiredReminderValue=0;
		}
		else
		{
			// if required clocks is larger than 256 then we will use loop after every full overflow with some count
			initialTimerValue=0;
			
			//Number of Clocks is Greater than 256 then Need for Overflow Count and may be need reminder
			prescaler.RequiredOverflowCounts = prescaler.NumberOfClocks / 65536;
			prescaler.RequiredReminderValue = prescaler.NumberOfClocks % 65536;
			
		}
	}
	
	//Set Initial Value For Timer
	switch(timer)
	{
		case TIMER_0:
			TIMER_REG_TCNT0 = BITWISE_GET_LOW_BYTE(initialTimerValue);
		break;
		
		case TIMER_1:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(initialTimerValue);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(initialTimerValue);
		break;

		case TIMER_1_A:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(initialTimerValue);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(initialTimerValue);
		break;

		case TIMER_1_B:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(initialTimerValue);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(initialTimerValue);
		break;
		
		case TIMER_2:
			TIMER_REG_TCNT2 = BITWISE_GET_LOW_BYTE(initialTimerValue);
		break;
	}
	
	//Save Some Values
	
	_InitialTimerValue = initialTimerValue;
	_RequiredOverflowCount = prescaler.RequiredOverflowCounts;
	_RequiredReminderValue = prescaler.RequiredReminderValue;

	_Prescaler = prescaler;

	/*
	_log.WriteText("Number Of Clocks = ");
	_log.WriteFloat(_Prescaler.NumberOfClocks,0);
	_log.NewLine();
	_log.WriteText("Initial Timer Value Of Clocks = ");
	_log.WriteFloat(_InitialTimerValue,0);
	_log.NewLine();
	_log.WriteText("Prescaler Overflow Counts = ");
	_log.WriteFloat(prescaler.OverflowCounts);
	_log.NewLine();
	_log.WriteText("Required Overflow Count = ");
	_log.WriteFloat(_RequiredOverflowCount,0);
	_log.NewLine();
	_log.WriteText("Required Reminder Value = ");
	_log.WriteFloat(_RequiredReminderValue,0);
	_log.NewLine();
	*/
	

}

//[Tested]
//Config Timer Mode (Normal,Compare,Correct PWM,Fast PWM)
void Timer::Config_Timer_Mode(TIMER_SELECTOR timer,MODE_SELECTOR mode)
{

	
	//_log.WriteLine("TCCR0 = WGM01(3),WGM00(6)");
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	

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

	
	//_log.WriteLine("TCCR0 = WGM01(3),WGM00(6)");
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	

}

//[Tested]
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
	
	//_log.WriteByteInfo(TIMER_REG_TCCR0);

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
	
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
}


#pragma endregion
