#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"
#include "Timers/Common"
#include "Timers/Timer.h"

#include "Interrupts/GlobalInterrupt.h"
#include "Interrupts/TimerInterrupt.h"

#define ENABLE_LOGS_TIMER 					//(Required)
//#define ENABLE_LOGS_GET_PRESCALER_INFO 
//#define ENABLE_LOGS_GET_PRESCALER_AUTO
//#define ENABLE_LOGS_CONFIG_TIMER_PRESCALER
//#define ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
//#define ENABLE_LOGS_CONFIG_TIMER_INITIAL_VALUE
//#define ENABLE_LOGS_CONFIG_TIMER_MODE


#pragma region Constructors

//[Tested]
Timer::Timer(TimerType timer,uint64 baseFrequency)
{
	//Initialize Members
	_Timer = timer;
	_BaseFrequency = baseFrequency;
	_Clock = ClockType::CLOCK_WITH_PRESCALER;
	_Mode= TimerMode::MODE_NORMAL;
	_DurationMode = TimerDuration::DURATION_REPEAT;
	_TimeUnit=TimeUnit::UNIT_SECOND;
	_Time=0;

	_IsRunning=false;
	_IsInterrupt=false;
	_IsOverflow=false;

	//Enable Logs
	#ifdef ENABLE_LOGS_TIMER
	_log = Logs();
	_log.Initialize(LOGS_SOURCE_UART0);
	#endif
	
}

#pragma endregion

#pragma region Methods

//Start Timer and Execute Action Every Period (Repeated Timer)
void Timer::Start(float delayTime,TimeUnit unit,void(*functionPtr)(),ClockType clock=CLOCK_WITH_PRESCALER,PrescalerType prescaler=PRESCALER_Auto)
{
	_Time = delayTime;
	_TimeUnit = unit;
	Overflow_Callback_Handler = functionPtr;
	_Clock = clock;
	_DurationMode=TimerDuration::DURATION_REPEAT;
	
	//If Timer Running then Stop it
	if(_IsRunning)
	{
		//Stop Timer as Default State
		Config_Timer_Clock_Source(ClockType::CLOCK_STOPPED);
	}

	//1) Get Prescaler Info For Timer if Timer Will Use It Only
	if(_Clock==CLOCK_WITH_PRESCALER)
	{
		if(prescaler == PRESCALER_Auto)
		{
			_PrescalerInfo=Get_Prescaler_Auto();
		}
		else
		{
			_PrescalerInfo=Get_Prescaler_Info(prescaler);
		}
		
	}

	//2) Config Prescaler For Timer (No Prescaler - PreScaler8 - PreScaler64 - PreScaler256 - PreScaler1024)
	Config_Timer_Prescaler(_PrescalerInfo.PrescalerType);
	
	//3) Config Initial Value For Timer inside Register [TCNT] depend on (No of Clocks)
	//Config_Timer_Initial_Value(&_PrescalerInfo);
	Config_Timer_Initial_Value(&_PrescalerInfo);

	//4) Config Timer Mode To Normal/Overflow
	Config_Timer_Mode(TimerMode::MODE_NORMAL);
	
	//5) Set Timer Clock Source With/Without Prescaler
	Config_Timer_Clock_Source(clock);

	//6) Enable Timer Interrupt For Normal Mode
	TimerInterrupt::Enable_Interrupt(this);
	
	//Update Timer Status
	_IsRunning=true;
	_IsInterrupt=true;	

}

//Delay Timer and Execute Action Once after Period (Once Timer)
void Timer::Delay(float delayTime,TimeUnit unit,void(*functionPtr)(),ClockType clock=CLOCK_WITH_PRESCALER,PrescalerType prescaler=PRESCALER_Auto)
{
	/*
	_Time = delayTime;
	_TimeUnit = unit;
	_Timer_Overflow_Callback_Ptr = functionPtr;
	_TimeByMicroSecond = Convert_Time_To_MicroSecond(delayTime,unit);
	_DurationMode=DURATION_MODE::ONCE;
	
	//Stop Timer as Default State
	Config_Timer_Clock_Source(CLOCK_SOURCE::CLOCK_STOPPED);


	//Start Timer as Normal Mode
	//StartTimer(delayTime,unit,DURATION_MODE::ONCE);

	_log.WriteLine("Timer Started");
	
	//Wait For Timer To Overflow mean Timer Finish The Delay Time
	while (_IsOverflow==false)
	{
		_log.WriteLine("Timer Running");
	};
	
	_log.WriteLine("Timer Stopped");

	//Stop Timer and Clear Flag
	Stop();
	*/
}

//Stop Timer , Stop Interrupt
void Timer::Stop()
{
	//Stop Timer By Select No Clock inside Clock Source
	Config_Timer_Clock_Source(CLOCK_STOPPED);

	//if Timer use Interrupt then Stop it
	if(_IsInterrupt)
	{
		TimerInterrupt::Disable_Interrupt(this);
	}

	//update Timer Status
	_IsRunning=false;
	_IsInterrupt=false;

}

#pragma endregion

#pragma region Helpers


//Increase Current Overflow Count inside PrescalerInfo
void Timer::IncreaseCurrentOverflowCount()
{
	_PrescalerInfo.CurrentOverFlowCount++;
}

//Set the Current Overflow Count inside PrescalerInfo
void Timer::SetCurrentOverflowCount(uint16 count)
{
	_PrescalerInfo.CurrentOverFlowCount=count;
}

//Set the Initial Timer Value For Current Timer
void Timer::SetInitialTimerValue(uint16 value)
{
	//Set Initial Value For Timer
	switch(_Timer)
	{
		case TIMER_0:
			TIMER_REG_TCNT0 = BITWISE_GET_LOW_BYTE(value);
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
			TIMER_REG_TCNT2 = BITWISE_GET_LOW_BYTE(value);
		break;
	}

	_PrescalerInfo.InitialTimerValue = value;
}

//Get Initial Timer Value stored inside PresaclerInfo
uint16 Timer::GetInitialTimerValue()
{
	return _PrescalerInfo.InitialTimerValue;
}

//Get the Current Overflow Count inside PrescalerInfo
uint16 Timer::GetCurrentOverflowCount()
{
	return _PrescalerInfo.CurrentOverFlowCount;
}

//Get Timer Type (Timer0,Timer1,Timer2)
TimerType Timer::GetTimerType()
{
	return _Timer;
}

//Get Timer Mode (Normal,Compare,PWM,InputCapture)
TimerMode Timer::GetTimerMode()
{
	return _Mode;
}

//Get Timer Duration (Once,Repeat)
TimerDuration Timer::GetTimerDuration()
{
	return _DurationMode;
}

//Get Timer Prescaler Info
PrescalerInfo Timer::GetPrescalerInfo()
{
	return _PrescalerInfo;
}

//Get the Overflow integer inside PrescalerInfo
uint16 Timer::GetOverflowInteger()
{
	return _PrescalerInfo.OverflowInteger;

}





//[Tested]
//Convert Time From Any Unit To MicroSecond
uint64 Timer::Convert_Time_To_MicroSecond(float time,TimeUnit unit)
{
	//Convert Time To MicroSeconds
	switch (unit)
	{
		case TimeUnit::UNIT_HOUR :
			return time * 60 * 60 * 1000 * 1000;
		break;

		case TimeUnit::UNIT_MINUTE:
			return time * 60 * 1000 * 1000;
		break;

		case TimeUnit::UNIT_SECOND:
			return time * 1000 * 1000;
		break;

		case TimeUnit::UNIT_MILLI:
			return time * 1000;
		break;

		case TimeUnit::UNIT_MICRO:
			return time;
		break;
	}

}

//[Tested]
//Calc Prescaler info like Overflow Counts depend on Timer, Base Frequency,time by microSecond Unit,preScaler (clock_Source)
PrescalerInfo Timer::Get_Prescaler_Info(PrescalerType preScaler)
{
	PrescalerInfo result;

	result.PrescalerType = preScaler;
	result.BaseFrequency=_BaseFrequency;
	result.TimeByMicroSecond = Convert_Time_To_MicroSecond(_Time,_TimeUnit);
	
	//Get the Final Frequency used with Timer
	switch (preScaler)
	{
		case PRESCALER_8:
			result.FinalFrequency = (float)_BaseFrequency/1000000/8;
		break;

		case PRESCALER_32:
			result.FinalFrequency = (float)_BaseFrequency/1000000/32;
		break;

		case PRESCALER_64:
			result.FinalFrequency = (float)_BaseFrequency/1000000/64;
		break;

		case PRESCALER_128:
			result.FinalFrequency = (float)_BaseFrequency/1000000/128;
		break;

		case PRESCALER_256:
			result.FinalFrequency = (float)_BaseFrequency/1000000/256;
		break;

		case PRESCALER_1024:
			result.FinalFrequency = (float)_BaseFrequency/1000000/1024;
		break;
	}

	//Get time of One Clock (MicroSecond) tc=1/frequency
	result.TimeOfClock = 1.0/result.FinalFrequency;

	//Get the Number of Clocks  needed to achieve time
	result.NumberOfClocks = result.TimeByMicroSecond/result.TimeOfClock;

	//Get the Overflow Counts
	if(_Timer==TIMER_0)
	{
		result.OverflowCounts= (float)result.NumberOfClocks / 256; //2^8
	} 
	else if(_Timer==TIMER_1)
	{
		result.OverflowCounts= (float)result.NumberOfClocks / 65536; //2^16
	}
	else if(_Timer==TIMER_2)
	{
		result.OverflowCounts= (float)result.NumberOfClocks / 256; //2^8
	}
	
	

	#ifdef ENABLE_LOGS_GET_PRESCALER_INFO

	Print_Prescaler_Info(&result);
	
	
	#endif

	return result;
}

//[Tested]
//Select the Best Prescaler depend on Timer , Frequency , DelayTime , Time Unit and get the most accurated value
PrescalerInfo Timer::Get_Prescaler_Auto()
{
	//We Will Calc Overflow Counts For All PreScalers and Take the Lowest Overflow Counts With Less Deciemal
	
	//We Will Calc PrescalerInfo For Every Prescaler and get the lowest value
	PrescalerInfo prescaler_1024 = Get_Prescaler_Info(PrescalerType::PRESCALER_1024);
	PrescalerInfo prescaler_256 = Get_Prescaler_Info(PrescalerType::PRESCALER_256);
	PrescalerInfo prescaler_128 = Get_Prescaler_Info(PrescalerType::PRESCALER_128); //For Timer2 Only
	PrescalerInfo prescaler_64 = Get_Prescaler_Info(PrescalerType::PRESCALER_64);
	PrescalerInfo prescaler_32 = Get_Prescaler_Info(PrescalerType::PRESCALER_32); //For Timer2 Only
	PrescalerInfo prescaler_8 = Get_Prescaler_Info(PrescalerType::PRESCALER_8);
	
	PrescalerInfo result;

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
	if(prescaler_128.OverflowCounts<lowestCount)
	{
		//For Only Timer2
		lowestCount = prescaler_128.OverflowCounts;
	}
	if(prescaler_64.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_64.OverflowCounts;
	}
	if(prescaler_32.OverflowCounts<lowestCount)
	{
		//For Only Timer2
		lowestCount = prescaler_32.OverflowCounts;
	}
	if(prescaler_8.OverflowCounts<lowestCount)
	{
		lowestCount = prescaler_8.OverflowCounts;
	}
	

	//Detect the Lowest Prescaler Info
	if(lowestCount==prescaler_1024.OverflowCounts)
	{
		result= prescaler_1024;
	}
	else if(lowestCount==prescaler_256.OverflowCounts)
	{
		result= prescaler_256;
	}
	else if(lowestCount==prescaler_128.OverflowCounts)
	{
		result= prescaler_128;
	}
	else if(lowestCount==prescaler_64.OverflowCounts)
	{
		result= prescaler_64;
	}
	else if(lowestCount==prescaler_32.OverflowCounts)
	{
		result= prescaler_32;
	}
	else if(lowestCount==prescaler_8.OverflowCounts)
	{
		result= prescaler_8;
	}
	

	#ifdef ENABLE_LOGS_GET_PRESCALER_AUTO
	_log.WriteLine("====== Get Prescaler Auto ======");
	_log.WriteText("Pre.1024 Overflow Counts = ");
	_log.WriteFloat(prescaler_1024.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.256 Overflow Counts = ");
	_log.WriteFloat(prescaler_256.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.128 (Timer2 Only) Overflow Counts = ");
	_log.WriteFloat(prescaler_128.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.64 Overflow Counts = ");
	_log.WriteFloat(prescaler_64.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.32 (Timer2 Only) Overflow Counts = ");
	_log.WriteFloat(prescaler_32.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Pre.8 Overflow Counts = ");
	_log.WriteFloat(prescaler_8.OverflowCounts);
	_log.NewLine();

	_log.WriteText("Best Prescaler = ");
	_log.WriteFloat(result.OverflowCounts);
	_log.NewLine();

	#endif


	return result;

}

//Print Prescaler info to log
void Timer::Print_Prescaler_Info(PrescalerInfo* prescaleInfo)
{
	_log.WriteText("PrescalerName   	");
	switch (prescaleInfo->PrescalerType)
	{
		case PRESCALER_1024 :
		_log.WriteLine("1024");
		break;

		case PRESCALER_256 :
		_log.WriteLine("256");
		break;

		case PRESCALER_128 :
		_log.WriteLine("128");
		break;
		
		case PRESCALER_64 :
		_log.WriteLine("64");
		break;

		case PRESCALER_32 :
		_log.WriteLine("32");
		break;

		case PRESCALER_8 :
		_log.WriteLine("8");
		break;

		case PRESCALER_NO :
		_log.WriteLine("NO");
		break;
	}

	_log.WriteText("BaseFrequency   	");
	_log.WriteLong(prescaleInfo->BaseFrequency);
	_log.NewLine();

	_log.WriteText("FinalFrequency  	");
	_log.WriteFloat(prescaleInfo->FinalFrequency);
	_log.NewLine();

	_log.WriteText("TimeOfClock     ");
	_log.WriteFloat(prescaleInfo->TimeOfClock);
	_log.NewLine();

	_log.WriteText("TimeMicroSecond ");
	_log.WriteLong(prescaleInfo->TimeByMicroSecond);
	_log.NewLine();

	_log.WriteText("NumberOfClocks  ");
	_log.WriteLong(prescaleInfo->NumberOfClocks);
	_log.NewLine();

	_log.WriteText("OverflowCounts  ");
	_log.WriteFloat(prescaleInfo->OverflowCounts);
	_log.NewLine();

	_log.WriteText("OverflowInteger ");
	_log.WriteInteger(prescaleInfo->OverflowInteger);
	_log.NewLine();
	
	_log.WriteText("O.flowFractions ");
	_log.WriteInteger(prescaleInfo->OverflowFractions);
	_log.NewLine();

	_log.WriteText("CurrentOverflow ");
	_log.WriteInteger(prescaleInfo->CurrentOverFlowCount);
	_log.NewLine();

	_log.WriteText("InitialTimerVal ");
	_log.WriteInteger(prescaleInfo->InitialTimerValue);
	_log.NewLine();
	_log.WriteLine("===================================");

}

//[Tested]
//Config Timer Clock Source (NoClock-CLock Original-Clock With Prescaler-ExternalClock)
void Timer::Config_Timer_Clock_Source(ClockType clock)
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
	
	#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
	_log.WriteLine("====== Config Timer Clock Source ======");
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	#endif
		

	switch(_Timer)
	{
		case TIMER_0: //Use Timer Counter Control Register [TCCR0]
		
			if(clock==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0

				_IsRunning=false;

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer0 Clock Stopped");
				#endif
				

			}
			else if(clock==CLOCK_ORIGINAL)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer0 Clock Original");
				#endif
			}
			else if(clock==CLOCK_WITH_PRESCALER)
			{
				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer0 Clock With Prescaler");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer0 Clock External Falling Edge");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer0 Clock External Rissing Edge");
				#endif
			}
		
		break;
		
		case TIMER_1_A://Use Timer Counter Control Register [TCCR1A]
		
			if(clock==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0

				_IsRunning=false;

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock Stopped");
				#endif

			}
			else if(clock==CLOCK_ORIGINAL)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock Original");
				#endif
			}
			else if(clock==CLOCK_WITH_PRESCALER)
			{
				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock With Prescaler");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock External Falling Edge");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock External Rissing Edge");
				#endif
			}
			
		
		break;
		
		case TIMER_1_B://Use Timer Counter Control Register [TCCR1B]
		
			if(clock==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0

				_IsRunning=false;

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock Stopped");
				#endif

			}
			else if(clock==CLOCK_ORIGINAL)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock Original");
				#endif

			}
			else if(clock==CLOCK_WITH_PRESCALER)
			{
				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock With Prescaler");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock External Falling Edge");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1
				
				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer1 Clock External Rissing Edge");
				#endif
			}
		
		
		break;
		
		case TIMER_2://Use Timer Counter Control Register [TCCR2]
		
			if(clock==CLOCK_STOPPED)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0

				_IsRunning=false;

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer2 Clock Stopped");
				#endif

			}
			else if(clock==CLOCK_ORIGINAL)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer2 Clock Original");
				#endif
			}
			else if(clock==CLOCK_WITH_PRESCALER)
			{
				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer2 Clock With Prescaler");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_FALLING_EDGE)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer2 Clock External Falling Edge");
				#endif
			}
			else if(clock==CLOCK_EXTERNAL_Rissing_EDGE)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
				_log.WriteLine("Timer2 Clock External Rissing Edge");
				#endif
			}
		
		break;
	}
	

	#ifdef ENABLE_LOGS_CONFIG_TIMER_CLOCK_SOURCE
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	#endif
	

}

//[Tested]
//Config Timer Prescaler (NoPrescaler,Prescaler8,64,256,1024,Auto)
void Timer::Config_Timer_Prescaler(PrescalerType prescaler)
{
	//Config Timer Clock Source by Using Register [TCCR] (Timer/Counter Control Register) with Bits [CS]

	//For Timer 0 and Timer 1 ================================
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
	
	//For Timer2 =============================================
	/*
	D2  D1  D0
	0	0	0     No Clock Source (Timer Stopped)
	0	0	1     Clock (No Prescalling)
	0	1	0     Clock / 8
	0	1	1     Clock / 32
	1	0	0     Clock / 64
	1	0	1     Clock / 128
	1	1	0     Clock / 256
	1	1	1     Clock / 1024
	*/
	



	#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
	_log.WriteLine("====== Config Timer Prescaler ======");
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	#endif
	

	switch(_Timer)
	{
		case TIMER_0: //Use Timer Counter Control Register [TCCR0]

			if(prescaler==PRESCALER_NO)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer0 With Prescaler No");
				#endif
	
			}
			else if(prescaler==PRESCALER_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer0 With Prescaler 8");
				#endif
			}
			else if(prescaler==PRESCALER_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS01);   //CS01 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS02); //CS02 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer0 With Prescaler 64");
				#endif
			}
			else if(prescaler==PRESCALER_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS00); //CS00 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer0 With Prescaler 256");
				#endif
			}
			else if(prescaler==PRESCALER_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS00);   //CS00 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,CS01); //CS01 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,CS02);   //CS02 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer0 With Prescaler 1024");
				#endif
			}
		
		break;
		
		case TIMER_1_A://Use Timer Counter Control Register [TCCR1A]
		
			if(prescaler==PRESCALER_NO)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler No");
				#endif
			}
			else if(prescaler==PRESCALER_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 8");
				#endif
			}
			else if(prescaler==PRESCALER_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 64");
				#endif
			}
			else if(prescaler==PRESCALER_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 256");
				#endif
			}
			else if(prescaler==PRESCALER_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 1024");
				#endif
			}
			
			
		
		break;
		
		case TIMER_1_B://Use Timer Counter Control Register [TCCR1B]
		
			if(prescaler==PRESCALER_NO)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler No");
				#endif
			}
			else if(prescaler==PRESCALER_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 8");
				#endif
			}
			else if(prescaler==PRESCALER_64)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS11);   //CS11 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS12); //CS12 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 64");
				#endif
			}
			else if(prescaler==PRESCALER_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS10); //CS10 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 256");
				#endif
			}
			else if(prescaler==PRESCALER_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS10);   //CS10 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,CS11); //CS11 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,CS12);   //CS12 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer1 With Prescaler 1024");
				#endif
			}
					
		
		break;
		
		case TIMER_2://Use Timer Counter Control Register [TCCR2]
		
			if(prescaler==PRESCALER_NO)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0
				
				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler No");
				#endif
			}
			else if(prescaler==PRESCALER_8)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler 8");
				#endif
			}
			else if(prescaler==PRESCALER_32)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21);   //CS21 [1] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS22); //CS22 [2] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler 32");
				#endif
			}
			else if(prescaler==PRESCALER_64)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20); //CS20 [0] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler 64");
				#endif
			}
			else if(prescaler==PRESCALER_128)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   //CS20 [0] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS21); //CS21 [1] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   //CS22 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler 128");
				#endif
			}
			else if(prescaler==PRESCALER_256)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,CS20);   	//CS20 [0] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21); 		//CS21 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   	//CS22 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler 256");
				#endif
			}
			else if(prescaler==PRESCALER_1024)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS20);   	//CS20 [0] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS21); 		//CS21 [1] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,CS22);   	//CS22 [2] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
				_log.WriteLine("Timer2 With Prescaler 1024");
				#endif
			}

			
		break;
	}
	

	#ifdef ENABLE_LOGS_CONFIG_TIMER_PRESCALER
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	#endif

}

//[Tested]
//Set Initial Value For Timer inside Register [TCNT] depend on (No of Clocks)
void Timer::Config_Timer_Initial_Value(PrescalerInfo* prescalerInfo)
{
	//Calculate the Initial Value For Timer if Required

	if(_Timer == TIMER_0 || _Timer == TIMER_2)
	{
		//For 8 Bit Timer Register
		
		if(prescalerInfo->NumberOfClocks<=256)
		{
			//if required clocks lower than the overflow value for timer value register then will set initial timer for some value and after overflow then mean
			//timer reached to required time and count the number of clocks needed
			prescalerInfo->InitialTimerValue = 256 - prescalerInfo->NumberOfClocks;
			
			//Number of Clocks is Equal or Lower Than 256 then No Need For Overflow Count
			
			prescalerInfo->OverflowCounts=1;
			prescalerInfo->OverflowInteger=1;
			prescalerInfo->OverflowFractions=0;
			prescalerInfo->CurrentOverFlowCount=0;

		}
		else
		{
			// if required clocks is larger than 256 then we will use loop after every full overflow with some count
			prescalerInfo->InitialTimerValue=0;
			
			//Number of Clocks is Greater than 256 then Need for Overflow Count and may be need reminder
			prescalerInfo->OverflowCounts=prescalerInfo->NumberOfClocks / 256;
			prescalerInfo->OverflowInteger=prescalerInfo->NumberOfClocks / 256;
			prescalerInfo->OverflowFractions=prescalerInfo->NumberOfClocks % 256;
			prescalerInfo->CurrentOverFlowCount=0;
		}

	}
	else if(_Timer == TIMER_1)
	{
		//For 16 Bit Timer Register
		
		if(prescalerInfo->NumberOfClocks<=65536)
		{
			//if required clocks lower than the overflow value for timer value register then will set initial timer for some value and after overflow then mean
			//timer reached to required time and count the number of clocks needed
			prescalerInfo->InitialTimerValue = 65536 - prescalerInfo->NumberOfClocks;
			
			//Number of Clocks is Equal or Lower Than 65536 then No Need For Overflow Count
			prescalerInfo->OverflowCounts=1;
			prescalerInfo->OverflowInteger=1;
			prescalerInfo->OverflowFractions=0;

			prescalerInfo->CurrentOverFlowCount=0;
		}
		else
		{
			// if required clocks is larger than 256 then we will use loop after every full overflow with some count
			prescalerInfo->InitialTimerValue=0;
			
			//Number of Clocks is Greater than 256 then Need for Overflow Count and may be need reminder
			prescalerInfo->OverflowCounts=prescalerInfo->NumberOfClocks / 65536;
			prescalerInfo->OverflowInteger=prescalerInfo->NumberOfClocks / 65536;
			prescalerInfo->OverflowFractions=prescalerInfo->NumberOfClocks % 65536;

			prescalerInfo->CurrentOverFlowCount=0;
			
		}
	}
	
	//Set Initial Value For Timer
	switch(_Timer)
	{
		case TIMER_0:
			TIMER_REG_TCNT0 = BITWISE_GET_LOW_BYTE(prescalerInfo->InitialTimerValue);
		break;
		
		case TIMER_1:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(prescalerInfo->InitialTimerValue);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(prescalerInfo->InitialTimerValue);
		break;

		case TIMER_1_A:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(prescalerInfo->InitialTimerValue);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(prescalerInfo->InitialTimerValue);
		break;

		case TIMER_1_B:
			TIMER_REG_TCNT1L = BITWISE_GET_LOW_BYTE(prescalerInfo->InitialTimerValue);
			TIMER_REG_TCNT1H = BITWISE_GET_HIGH_BYTE(prescalerInfo->InitialTimerValue);
		break;
		
		case TIMER_2:
			TIMER_REG_TCNT2 = BITWISE_GET_LOW_BYTE(prescalerInfo->InitialTimerValue);
		break;
	}
	
	 #ifdef ENABLE_LOGS_CONFIG_TIMER_INITIAL_VALUE
	 _log.WriteLine("====== Config Timer Initial Value ======");
	 Print_Prescaler_Info(prescalerInfo);
	 #endif


}

//[Tested]
//Config Timer Mode (Normal,Compare,Correct PWM,Fast PWM)
void Timer::Config_Timer_Mode(TimerMode mode)
{
	#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
	_log.WriteLine("====== Config Timer Mode ======");
	//_log.WriteLine("TCCR0 = WGM01(3),WGM00(6)");
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	#endif
	
	switch(_Timer)
	{
		case TIMER_0: //Use Timer Counter Control Register [TCCR0]
		    
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer0 Set To Mode Normal");
				#endif
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer0 Set To Mode CTC");
				#endif
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer0 Set To Mode PWM Correct");
				#endif
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer0 Set To Mode PWM Fast");
				#endif
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM01); //WGM01 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR0,WGM00); //WGM00 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer0 Set To Mode Input Capture");
				#endif
			}
		
		break;
		
		case TIMER_1_A://Use Timer Counter Control Register [TCCR1A]
			
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode Normal");
				#endif
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode CTC");
				#endif
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode PWM Correct");
				#endif
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode PWM Fast");
				#endif
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1A,WGM10); //WGM10 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode Input Capture");
				#endif
			}
			
		break;
		
		case TIMER_1_B://Use Timer Counter Control Register [TCCR1B]
		
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode Normal");
				#endif
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode CTC");
				#endif
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode PWM Correct");
				#endif
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode PWM Fast");
				#endif
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM11); //WGM11 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR1B,WGM10); //WGM10 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer1 Set To Mode Input Capture");
				#endif
			}
		
		break;
		
		case TIMER_2://Use Timer Counter Control Register [TCCR2]
			
			if(mode==MODE_NORMAL)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer2 Set To Mode Normal");
				#endif
			}
			else if(mode==MODE_COMPARE_CTC)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 1
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer2 Set To Mode CTC");
				#endif
			}
			else if(mode==MODE_PWM_CORRECT)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 0
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer2 Set To Mode PWM Correct");
				#endif
			}
			else if(mode==MODE_PWM_FAST)
			{
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 1
				BITWISE_SET_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 1

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer2 Set To Mode PWM Fast");
				#endif
			}
			else if(mode==MODE_INPUT_CAPTURE_FLAG)
			{
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM21); //WGM21 [3] = 0
				BITWISE_CLEAR_BIT(TIMER_REG_TCCR2,WGM20); //WGM20 [6] = 0

				#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
				_log.WriteLine("Timer2 Set To Mode Input Capture");
				#endif
			}
			
		break;
	}

	#ifdef ENABLE_LOGS_CONFIG_TIMER_MODE
	//_log.WriteLine("TCCR0 = WGM01(3),WGM00(6)");
	//_log.WriteByteInfo(TIMER_REG_TCCR0);
	#endif
}



#pragma endregion
