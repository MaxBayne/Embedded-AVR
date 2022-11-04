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
//ENUMS
////////////////////////////////////////////

#pragma region ENUMS

//Define The Timer
typedef enum
{
	TIMER_0,
	TIMER_1,
	TIMER_1_A,
	TIMER_1_B,
	TIMER_2
}TIMER_SELECTOR;

//Define The Clock Source
typedef enum
{
	CLOCK_STOPPED,
	CLOCK_PRESCAL_No,
	CLOCK_PRESCAL_8,
	CLOCK_PRESCAL_64,
	CLOCK_PRESCAL_256,
	CLOCK_PRESCAL_1024,
	CLOCK_EXTERNAL_FALLING_EDGE,
	CLOCK_EXTERNAL_Rissing_EDGE
}CLOCK_SOURCE;

//Define The Mode of Timer (Normal,CTC,PWM)
typedef enum
{
	MODE_NORMAL,
	MODE_COMPARE_CTC,
	MODE_PWM_CORRECT,
	MODE_PWM_FAST,
	MODE_INPUT_CAPTURE_FLAG
}MODE_SELECTOR;

#pragma endregion ENUMS

#ifndef __TIMER_H__
#define __TIMER_H__


class Timer
{
	private:
	
	TIMER_SELECTOR _TimerSelector;
	CLOCK_SOURCE _ClockSource;
	MODE_SELECTOR _ModeSelector;
	bool _IsRunning;
	bool _IsInterrupt;
	int _NumberOfClock;
	uint16 _InitialTimerValue;
	uint16 _DelayTime;
	uint64 _Frequency;
	bool _NeedOverflowCount;
	uint8 _OverflowCount;
	uint8 _ReminderValue;
	
	public:
	
	
	//Constructors
	
	
	//Timer Without Interrupt
	Timer(TIMER_SELECTOR timer,CLOCK_SOURCE clockSource,uint64 frequency);
	
	//Timer With Interrupt
	Timer(TIMER_SELECTOR timer,CLOCK_SOURCE clockSource,uint64 frequency,void(*functionPtr)());
	
	
	//Start Active Timer as Normal Mode (Delay) with Some Period per Seconds
	void StartTimer(float delayTime);
	
	//Stop Active Timer
	void StopTimer();
	
	//Get Status of Active Timer (Running/Stopped)
	bool IsRunning();
	
	//Detect if Current Timer Support Interrupt
	bool IsInterrupt();


	private:
	
	//Calculate the Initial Value For Timer Depend on Frequency , Needed Delay Time Per Seconds and Set it
	void Config_Timer_Initial_Value(TIMER_SELECTOR timer,uint64 frequency,uint16 delayTime);
	
	//Config Timer Mode (Normal,Compare,Correct PWM,Fast PWM)
	void Config_Timer_Mode(TIMER_SELECTOR timer,MODE_SELECTOR mode);
	
	//Config Timer Clock Source (NoClock-Clock-Clock/8-Clock/64-Clock/256-Clock/1024-ExternalClockFallingEdge-ExternalClockRisingEdge)
	void Config_Timer_Clock_Source(TIMER_SELECTOR timer,CLOCK_SOURCE source);
	
	
	//Set Initial Value For Timer inside Register [TCNT]
	void Set_Timer_Initial_Value(TIMER_SELECTOR timer,uint16 value);

}; //Timer

#endif //__TIMER_H__
