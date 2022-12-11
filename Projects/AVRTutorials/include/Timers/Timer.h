#ifndef __TIMER_H__
#define __TIMER_H__

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


class Timer
{
	private:
	
	#pragma region Members

	Logs _log; 						//Log to UART

	uint64 _BaseFrequency;			//The Frequency Of MicroController like 16 MHZ
	TimerType _Timer;      			//The Current Timer (Timer0,Timer1,Timer2)
	ClockType _Clock;				//The Current Clock Source (Stopped,CLock With Prescaler,ExternalEdge)
	PrescalerInfo _PrescalerInfo; 	//Current Used Prescaler with its info
	TimerMode _Mode;				//The Current Mode For Timer (Normal,Compare,Input Capture,PWM)
	TimerDuration _DurationMode;	//Run Timer Once or Repeated

	float _Time;					//Time Needed for Delay
	TimeUnit _TimeUnit;				//Timer Unit (MICRO,MILLI,SECOND,MINUTE,HOUR)

	bool _IsRunning;				//Is Timer Running or Stopped
	bool _IsInterrupt;				//Is Timer use Interrupt or Just Timer Status [IsRunning]
	bool _IsOverflow;			  	//Is Timer Overflow in Normal Mode

	public:

	void(*Overflow_Callback_Handler)(); //Overflow Handler
	
	#pragma endregion

	public:
	
	#pragma region Constructors

	//Constructor
	Timer(TimerType timer,uint64 baseFrequency);

	#pragma endregion

	#pragma region Methods	

	//Start Timer and Execute Action Every Period (Repeated Timer)
	void Start(float delayTime,TimeUnit unit,void(*functionPtr)(),ClockType clock=CLOCK_WITH_PRESCALER,PrescalerType prescaler=PRESCALER_Auto);

	//Delay Timer and Execute Action Once after Period (Once Timer)
	void Delay(float delayTime,TimeUnit unit,void(*functionPtr)(),ClockType clock=CLOCK_WITH_PRESCALER,PrescalerType prescaler=PRESCALER_Auto);

	//Stop Timer , Stop Interrupt
	void Stop();

	//Increase Current Overflow Count inside PrescalerInfo
	void IncreaseCurrentOverflowCount();

	//Set the Current Overflow Count inside PrescalerInfo
	void SetCurrentOverflowCount(uint16 count);

	//Set the Initial Timer Value For Current Timer
	void SetInitialTimerValue(uint16 value);

	
	//Get Initial Timer Value stored inside PresaclerInfo
	uint16 GetInitialTimerValue();

	//Get the Current Overflow Count inside PrescalerInfo
	uint16 GetCurrentOverflowCount();

	//Get the Overflow integer inside PrescalerInfo
	uint16 GetOverflowInteger();

	//Get Timer Type (Timer0,Timer1,Timer2)
	TimerType GetTimerType();

	//Get Timer Mode (Normal,Compare,PWM,InputCapture)
	TimerMode GetTimerMode();

	//Get Timer Duration (Once,Repeat)
	TimerDuration GetTimerDuration();

	//Get Timer Prescaler Info
	PrescalerInfo GetPrescalerInfo();




	#pragma endregion

	

	private:
	
	#pragma region Helpers

	//Convert Time From Any Unit To MicroSecond
	uint64 Convert_Time_To_MicroSecond(float time,TimeUnit unit);

	//Calc All Prescaler info like Overflow Counts depend on Timer, Base Frequency,time by microSecond Unit,preScaler (clock_Source)
	PrescalerInfo Get_Prescaler_Info(PrescalerType preScaler);

	//Select the Best Prescaler depend on Timer , Frequency , DelayTime , Time Unit and get the most accurated value
	PrescalerInfo Get_Prescaler_Auto();

	//Print Prescaler info to log
	void Print_Prescaler_Info(PrescalerInfo* prescaleInfo);


	//Config Timer Clock Source (NoClock-Clock With Prescaler-ExternalClock)
	void Config_Timer_Clock_Source(ClockType clock);
	
	//Config Timer Prescaler (NoPrescaler,Prescaler8,64,256,1024,Auto)
	void Config_Timer_Prescaler(PrescalerType prescaler);

	//Set Initial Value For Timer inside Register [TCNT] depend on (No of Clocks)
	void Config_Timer_Initial_Value(PrescalerInfo* prescalerInfo);

	//Config Timer Mode (Normal,Compare,Correct PWM,Fast PWM)
	void Config_Timer_Mode(TimerMode mode);

	#pragma endregion
	
}; //Timer

#endif //__TIMER_H__
