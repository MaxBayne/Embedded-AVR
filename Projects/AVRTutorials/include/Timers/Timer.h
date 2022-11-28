#ifndef __TIMER_H__
#define __TIMER_H__

//#include "Interrupts/TimerInterrupt.h"

////////////////////////////////////////////
//DESCRIPTION
////////////////////////////////////////////
/* 

AtMega32A Timer
---------------
- Timer0 (8 Bit Register , using as Timer , Counter)
- Timer1 (16 Bit Register , using as Timer , Counter)
- Timer2 (8 Bit Register , using as Timers Only)

Timer Modes
---------------
- Normal Mode (For Delay Operations)
- Counter Mode (Like Normal Mode but Count External Pulses)
- CTC Mode (For Compare Operations)
- PMW Correct Phase
- PWM Fast

Timer Registers
---------------
- Registers For Hold Time Value like:
  TCNT0 : For Timer0 , its 8 Bit Register hold value from 0:255
  TCNT1 : For Timer1 , its 16 Bit Register hold value from 0:65536
  TCNT2 : For Timer2 , its 8 Bit Register hold value from 0:255
  
- Registers For Hold Compare Value like:
  OCR0  : For Compare With Timer0
  OCR1A : For Compare with Timer1 A Channel
  OCR1B : For Compare with Timer1 B Channel
  OCR2  : For Compare With Timer2

- Registers For Control Timers like:
  TCCR0 : For Control Timer0
  TCCR1A: For Control Timer1 A Channel
  TCCR1B: For Control Timer1 B Channel
  TCCR2 : For Control Timer2

- Registers For Interrupt Timers Like:
  TIFR  : For Control Interrupt with Timers 0,1,2
  

Timer As Normal Mode (Delay)
----------------------------

- Set Initial Value For Timer inside Register [TCNT0]
- Select Timer Mode to be Normal Mode inside Register [TCCR0]
- Set Clock Source of Clock Pulse inside Register [TCCR0]
- Set Timer Interrupt inside Register [TIFR]
- Stop Timer
- Clear Interrupt Flag


*/



////////////////////////////////////////////
//CLASS
////////////////////////////////////////////

class Timer
{
	private:
	
	Logs _log;

	TIMER_SELECTOR _Timer;      	//The Current Timer
	CLOCK_SOURCE _Clock;			//The Current Clock Source
	MODE_SELECTOR _Mode;			//The Current Mode For Timer
	PrescalerInfo _Prescaler;		//The Prescaler that used with timer and its info
	bool _IsRunning;				//Is Timer Running or Stopped
	bool _IsInterrupt;				//Is Timer use Interrupt or Just Timer Status [IsRunning]
	//int _NumberOfClock;				//The Number of Clock Required To Achieve Timer Delay
	uint16 _InitialTimerValue;		//The Initial Value For Timer To Be Set To Achieve Timer Delay
	uint16 _DelayTime;				//The Deplay Time Per Second To Make Timer Work On
	uint64 _Frequency;				//The Frequency Of MicroController like 16 MHZ
	
	uint16 _CurrentOverFlowCount;	//Current Count of Overflow
	uint16 _RequiredOverflowCount;	//How Much Count of Overflow To Count To Achieve Delay Time
	uint8 _RequiredReminderValue;  	//Reminder Value To be used beside Overflow Count
	bool _IsOverflow;			  	//Is Timer Overflow in Normal Mode

	public:
	
	//Constructor
	Timer(TIMER_SELECTOR timer,uint64 frequency);

	//Delay With Some Time By Timer (seconds)
	void Delay(float delayTime,TIME_UNIT unit);

	//Start Timer as Normal Mode (Delay) with Some Period per Seconds Without using Interrupt , just Timer [IsRunning]
	void StartTimer(float delayTime,TIME_UNIT unit,DURATION_MODE mode);
	
	//Start Timer as Normal Mode (Delay) with Some Period per Seconds With using Interrupt To Fire function Callback
	void StartTimer(float delayTime,TIME_UNIT unit,DURATION_MODE mode,void(*functionPtr)());


	//Stop Active Timer
	void StopTimer();
	
	//Get Status of Active Timer (Running/Stopped)
	bool IsRunning();
	
	//Detect if Current Timer Use Interrupt or Not
	bool IsInterrupt();

	//Detect if Current Timer with Normal Mode is Overflow
	bool IsOverflow();


	private:
	
	//Convert Time From Any Unit To MicroSecond
	uint64 Time_To_MicroSecond(float time,TIME_UNIT unit);

	//Calc All Prescaler info like Overflow Counts depend on Timer, Base Frequency,time by microSecond Unit,preScaler (clock_Source)
	PrescalerInfo Get_Prescaler_Info(TIMER_SELECTOR timer,CLOCK_SOURCE preScaler,uint64 frequency,float time,TIME_UNIT unit);

	//Select the Best Prescaler depend on Timer , Frequency , DelayTime , Time Unit and get the most accurated value
	PrescalerInfo Get_Best_Prescaler(TIMER_SELECTOR timer , uint64 frequency,float delayTime,TIME_UNIT unit);



	//Set Initial Value For Timer inside Register [TCNT] depend on (No of Clocks)
	void Config_Timer_Initial_Value(TIMER_SELECTOR timer,PrescalerInfo prescaler);

	//Config Timer Mode (Normal,Compare,Correct PWM,Fast PWM)
	void Config_Timer_Mode(TIMER_SELECTOR timer,MODE_SELECTOR mode);
	
	//Config Timer Clock Source (NoClock-Clock-Clock/8-Clock/64-Clock/256-Clock/1024-ExternalClockFallingEdge-ExternalClockRisingEdge)
	void Config_Timer_Clock_Source(TIMER_SELECTOR timer,CLOCK_SOURCE source);
	
	
}; //Timer

#endif //__TIMER_H__
