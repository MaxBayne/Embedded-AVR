#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "Timers/Common"
#include "Timers/Timer.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"
#include "Interrupts/GlobalInterrupt.h"
#include "Interrupts/TimerInterrupt.h"

//#define ENABLE_LOGS_ENABLE_INTERRUPT
//#define ENABLE_LOGS_DISABLE_INTERRUPT
//#define ENABLE_LOGS_CLEAR_INTERRUPT_FLAG
//#define ENABLE_LOGS_STOP_TIMER


//Reference For Timer0
static Timer* TimerInterrupt::Timer0;

//Reference For Timer1
static Timer* TimerInterrupt::Timer1;

//Reference For Timer2
static Timer* TimerInterrupt::Timer2;


//Enable Timer Interrupt For Active Timer ,Mode  and Set Function Callback when Interrupt Fired
static void TimerInterrupt::Enable_Interrupt(Timer* timer)
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

	uint8* msg;
	
	//Config Log 
	 #ifdef ENABLE_LOGS_ENABLE_INTERRUPT
	 Logs _log=Logs();
	 _log.Initialize(LOGS_SOURCE_UART0);
	 _log.WriteLine("====== Enable Interrupt ======");
	 #endif
	
	//Enable Global Interrupt
	GlobalInterrupt::Enable_Global_Interrupt();

	//Enable Timer Interrupt
	TimerType timerType = timer->GetTimerType();
	TimerMode timerMode = timer->GetTimerMode();

	switch(timerType)
	{
		case TIMER_0 :
		
		if(timerMode==MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 0
			
			msg="Enabled Timer0 Interrupt (Noraml Mode)";
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,TOIE0); //[TOIE0]

			TimerInterrupt::Timer0=timer;
			
		}
		else if(timerMode==MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 0
			
			msg="Enabled Timer0 Interrupt (Output Compare Mode)";
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,OCIE0); //[OCIE0]

			TimerInterrupt::Timer0=timer;

		}
		
		
		break;
		
		case  TIMER_1:
		
		if(timerMode==MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 1
			
			msg="Enabled Timer1 Interrupt (Noraml Mode)";

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,TOIE1); //[TOIE1]
			
			TimerInterrupt::Timer1=timer;

			
		}
		else if(timerMode==MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 1 A,B Channel
			
			msg="Enabled Timer1 Interrupt (Output Compare Mode)";

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,OCIE1B); //For Channel B  [OCIE1B]
			
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,OCIE1A); //For Channel A  [OCIE1A]
			

			TimerInterrupt::Timer1=timer;

			
		}
		else if(timerMode==MODE_INPUT_CAPTURE_FLAG)
		{
			msg="Enabled Timer1 Interrupt (Input Capture Mode)";

			//For Input Capture Flag Timer 1
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,TICIE1);  //[TICIE1]
			
			TimerInterrupt::Timer1=timer;
			
		}
		
		break;
		
		case  TIMER_2:
		
		if(timerMode==MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 2
			
			msg="Enabled Timer2 Interrupt (Noraml Mode)";

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,TOIE2); //[TOIE2]
			
			TimerInterrupt::Timer2=timer;
			
		}
		else if(timerMode==MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 2
			
			msg="Enabled Timer2 Interrupt (Output Compare Mode)";

			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,OCIE2); //[OCIE2]
			
			TimerInterrupt::Timer2=timer;
			
		}
		
		break;
	}
	
	
	#ifdef ENABLE_LOGS_ENABLE_INTERRUPT
	_log.WriteLine(msg);
	#endif

	delay_us(1);
	

}

//Disable Timer Interrupt For Timer,Mode
static void TimerInterrupt::Disable_Interrupt(Timer* timer)
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
	
	uint8* msg;
	//Config Log 
	#ifdef ENABLE_LOGS_DISABLE_INTERRUPT
	Logs _log=Logs();
	
	_log.Initialize(LOGS_SOURCE_UART0);
	_log.WriteLine("====== Disable Interrupt ======");
	#endif

	//Disable Timer Interrupt
	switch(timer->GetTimerType())
	{
		case TIMER_0 :
		
		if(timer->GetTimerMode()==MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 0
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,TOIE0); //[TOIE0]

			msg="Disabled Timer0 Interrupt (Noraml Mode)";
			
		}
		else if(timer->GetTimerMode()==MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 0
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,OCIE0); //[OCIE0]

			msg="Disabled Timer0 Interrupt (Output Compare Mode)";
		}
		
		
		break;
		
		case  TIMER_1:
		
		if(timer->GetTimerMode()==MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 1
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,TOIE1); //[TOIE1]

			msg="Disabled Timer1 Interrupt (Noraml Mode)";
			
		}
		else if(timer->GetTimerMode()==MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 1 A,B Channel
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,OCIE1B); //For Channel B  [OCIE1B]
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,OCIE1A); //For Channel A  [OCIE1A]

			msg="Disabled Timer1 Interrupt (Output Compare Mode)";
		}
		else if(timer->GetTimerMode()==MODE_INPUT_CAPTURE_FLAG)
		{
			//For Input Capture Flag Timer 1
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,TICIE1);  //[TICIE1]

			msg="Disabled Timer1 Interrupt (Input Capture Mode)";
		}
		
		break;
		
		case  TIMER_2:
		
		if(timer->GetTimerMode()==MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 2
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,TOIE2); //[TOIE2]

			msg="Disabled Timer2 Interrupt (Noraml Mode)";
		}
		else if(timer->GetTimerMode()==MODE_COMPARE_CTC)
		{
			//For Output Compare Timer 2
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,OCIE2); //[OCIE2]
			
			msg="Disabled Timer2 Interrupt (Output Compare Mode)";
		}
		
		break;
	}
	
	#ifdef ENABLE_LOGS_DISABLE_INTERRUPT
	_log.WriteLine(msg);
	#endif

	//Clear Timer Interrupt Flag For Active Timer and Mode
	Clear_Interrupt_Flag(timer);
	
}

//Clear Interrupt Flag For Timer , Mode when Interrupt fired we clear it inside ISR to avoid ReEnter ISR Again
static void TimerInterrupt::Clear_Interrupt_Flag(Timer* timer)
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
	
	uint8* msg;

	//Config Log 
	#ifdef ENABLE_LOGS_CLEAR_INTERRUPT_FLAG
	Logs _log=Logs();
	_log.Initialize(LOGS_SOURCE_UART0);
	_log.WriteLine("====== Clear Interrupt ======");
	#endif

	switch(timer->GetTimerType())
	{
		case TIMER_0 :
		
			if(timer->GetTimerMode()==MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 0 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,TOV0); //[TOV0]

				msg="Cleared Interrupt Flag For Timer 0 (Normal Mode)";
				
			}
			else if(timer->GetTimerMode()==MODE_COMPARE_CTC)
			{
				//For Output Compare Timer 0 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,OCF0); //[OCF0]

				msg="Cleared Interrupt Flag For Timer 0 (Output Compare Mode)";
				
			}
		
		
		break;
		
		case  TIMER_1 :
		
			if(timer->GetTimerMode()==MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 1 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,TOV1); //[TOV1]

				msg="Cleared Interrupt Flag For Timer 1 (Normal Mode)";
				
			}
			else if(timer->GetTimerMode()==MODE_COMPARE_CTC)
			{
				//For Output Compare Timer 1 A,B Channel
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,OCF1B); //For Channel B  [OCF1B]
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,OCF1A); //For Channel A  [OCF1A]

				msg="Cleared Interrupt Flag For Timer 1 (Output Compare Mode)";
				
			}
			else if(timer->GetTimerMode()==MODE_INPUT_CAPTURE_FLAG)
			{
				//For Input Capture Flag Timer 1 
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,ICF1);  //[ICF1]

				msg="Cleared Interrupt Flag For Timer 1 (Input Capture Mode)";
			}
		
		break;
		
		case  TIMER_2:
		
			if(timer->GetTimerMode()==MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 2
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,TOV2); //[TOV2]

				msg="Cleared Interrupt Flag For Timer 2 (Normal Mode)";
			}
			else if(timer->GetTimerMode()==MODE_COMPARE_CTC)
			{
				//For Output Compare Timer 2
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,OCF2); //[OCF2]

				msg="Cleared Interrupt Flag For Timer 2 (Output Compare Mode)";
			}
		
		break;
	}
	
	#ifdef ENABLE_LOGS_CLEAR_INTERRUPT_FLAG
	_log.WriteLine(msg);
	#endif
}

//Read Interrupt Flag For Timer , Mode , return 1 when Flag On or 0 when flag off
static uint8 TimerInterrupt::Read_Interrupt_Flag(Timer* timer)
{
	TimerType timerType = timer->GetTimerType();

	switch (timer->GetTimerMode())
	{
		case MODE_NORMAL :

			if(timerType==TIMER_0)
			{
				//For Normal Mode (Overflow) Timer 0 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,TOV0); //[TOV0]
			}
			else if(timerType==TIMER_1)
			{
				//For Normal Mode (Overflow) Timer 1 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,TOV1); //[TOV1]
			}
			else if(timerType==TIMER_2)
			{
				//For Normal Mode (Overflow) Timer 2 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,TOV2); //[TOV2]
			}

		break;

		case MODE_COMPARE_CTC :

			if (timerType == TIMER_0)
			{
				// For Output Compare Timer 0
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR, OCF0); //[OCF0]
			}
			else if (timerType == TIMER_1_A)
			{
				//For Output Compare Timer 1 A Channel
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,OCF1A); //For Channel A  [OCF1A]
			}
			else if (timerType == TIMER_1_B)
			{
				//For Output Compare Timer 1 B Channel
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,OCF1B); //For Channel B  [OCF1B]
			}
			else if (timerType == TIMER_2)
			{
				//For Output Compare Timer 2
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,OCF2); //[OCF2]
			}

		break;

		case MODE_INPUT_CAPTURE_FLAG :
			if(timerType==TIMER_1)
			{
				//For Input Capture Flag Timer 1 
				return BITWISE_GET_BIT(INTERRUPT_REG_TIFR,ICF1);  //[ICF1]
			}
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
	Timer* timer0=TimerInterrupt::Timer0;

	//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
	TimerInterrupt::Clear_Interrupt_Flag(timer0);
	
	//Re Initial Timer Value To Start Count From it
  	//TCNT0=0;
	timer0->SetInitialTimerValue(timer0->GetInitialTimerValue());

	//Increase Overflow Counter
  	timer0->IncreaseCurrentOverflowCount();

 	//Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  	if(timer0->GetCurrentOverflowCount()==timer0->GetOverflowInteger())
  	{
		//Timer Reach to overflow count needed to wait for 1 second
    	timer0->SetCurrentOverflowCount(0);

		//Call Function inside Callback
		if(timer0->Overflow_Callback_Handler!=0)
		{
			timer0->Overflow_Callback_Handler();
		}
		
		//If Timer Mode Duration is Once then Stop Timer otherwise will be Repeated
		if(timer0->GetTimerDuration()==DURATION_ONCE)
		{
			//Stop Timer Here ---------------------
			timer0->Stop();
		}
    	
	}
}


//Interrupt Service Routine For Timer 1 Overflow
ISR(TIMER1_OVF_vect)
{
	Timer* timer1=TimerInterrupt::Timer1;

	//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
	TimerInterrupt::Clear_Interrupt_Flag(timer1);
	
	//Re Initial Timer Value To Start Count From it
  	//TCNT0=0;
	timer1->SetInitialTimerValue(timer1->GetInitialTimerValue());

	//Increase Overflow Counter
  	timer1->IncreaseCurrentOverflowCount();

 	//Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  	if(timer1->GetCurrentOverflowCount()==timer1->GetOverflowInteger())
  	{
		//Timer Reach to overflow count needed to wait for 1 second
    	timer1->SetCurrentOverflowCount(0);

		//Call Function inside Callback
		if(timer1->Overflow_Callback_Handler!=0)
		{
			timer1->Overflow_Callback_Handler();
		}
		
		//If Timer Mode Duration is Once then Stop Timer otherwise will be Repeated
		if(timer1->GetTimerDuration()==DURATION_ONCE)
		{
			//Stop Timer Here ---------------------
			timer1->Stop();
		}
    	
	}

}
/*
//Interrupt Service Routine For Timer 2 Overflow
ISR(TIMER2_OVF_vect)
{
	Timer* timer2=TimerInterrupt::Timer2;

	//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
	TimerInterrupt::Clear_Interrupt_Flag(timer2);
	
	//Re Initial Timer Value To Start Count From it
  	//TCNT0=0;
	timer2->SetInitialTimerValue(timer2->GetInitialTimerValue());

	//Increase Overflow Counter
  	timer2->IncreaseCurrentOverflowCount();

 	//Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  	if(timer2->GetCurrentOverflowCount()==timer2->GetOverflowInteger())
  	{
		//Timer Reach to overflow count needed to wait for 1 second
    	timer2->SetCurrentOverflowCount(0);

		//Call Function inside Callback
		if(timer2->Overflow_Callback_Handler!=0)
		{
			timer2->Overflow_Callback_Handler();
		}
		
		//If Timer Mode Duration is Once then Stop Timer otherwise will be Repeated
		if(timer2->GetTimerDuration()==DURATION_ONCE)
		{
			//Stop Timer Here ---------------------
			timer2->Stop();
		}
    	
	}


}
*/
#pragma endregion Interrupts For Overflow

#pragma region Interrupts For Compare Match CTC

//Interrupt Service Routine For Timer 0 Compare Match CTC
ISR(TIMER0_COMP_vect)
{

	
	
		//Call Function inside Callback
		//Timer_0_Compare_Match_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		//TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_0,MODE_SELECTOR::MODE_COMPARE_CTC);
	
}

//Interrupt Service Routine For Timer 1 Channel A Compare Match CTC
ISR(TIMER1_COMPA_vect)
{
	
		//Call Function inside Callback
		//Timer_1_Compare_Match_A_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		//TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_COMPARE_CTC);
	
}

//Interrupt Service Routine For Timer 1 Channel B Compare Match CTC
ISR(TIMER1_COMPB_vect)
{
	
		//Call Function inside Callback
		//Timer_1_Compare_Match_B_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		//TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_COMPARE_CTC);
	
}

//Interrupt Service Routine For Timer 2 Compare Match CTC
ISR(TIMER2_COMP_vect)
{
	
		//Call Function inside Callback
		//Timer_2_Compare_Match_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		//TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_2,MODE_SELECTOR::MODE_COMPARE_CTC);
	
}


#pragma endregion Interrupts For Overflow

#pragma region Interrupts For Capture Event [Timer1 Only]

//Interrupt Service Routine For Timer 1 Capture Event
ISR(TIMER1_CAPT_vect)
{
	
		//Call Function inside Callback
		//Timer_1_Capture_Event_Callback_Ptr();

		//Clear Timer Interrupt To Avoid reEnter ISR again wihout Interrupt Fired
		//TimerInterrupt::Clear_Interrupt_Flag(TIMER_SELECTOR::TIMER_1,MODE_SELECTOR::MODE_INPUT_CAPTURE_FLAG);
	
}


#pragma endregion Interrupts For Overflow