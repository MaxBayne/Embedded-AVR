#include "../Common/Common.h"
#include "../Common/Registers.h"
#include "../GPIO/GPIO.h"
#include "avr/interrupt.h"

#include "GlobalInterrupt.h"
#include "TimerInterrupt.h"


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


//Timer Interrupt Without Enable Interrupt
TimerInterrupt::TimerInterrupt(INTERRUPT_TIMER timer,INTERRUPT_MODE mode)
{
	_timer=timer;
	_timerMode=mode;
	
	//Clear Timer Interrupt Flag For Active Timer and Mode
	Clear_Interrupt_Flag(timer,mode);
}

//Timer Interrupt With Enable Interrupt
TimerInterrupt::TimerInterrupt(INTERRUPT_TIMER timer,INTERRUPT_MODE mode,void(* functionPtr)())
{
	_timer=timer;
	_timerMode=mode;
	
	//Clear Timer Interrupt Flag For Active Timer and Mode
	Clear_Interrupt_Flag(timer,mode);
	
	//Enable Timer Interrupt For Active Timer and Mode
	Enable_Interrupt(functionPtr);
}


//Enable Timer Interrupt For Active Timer ,Mode  and Set Function Callback when Interrupt Fired
void TimerInterrupt::Enable_Interrupt(void(* functionPtr)())
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
	
	switch(_timer)
	{
		case INTERRUPT_TIMER_0:
		
		if(_timerMode==INTERRUPT_MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 0
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,0); //[TOIE0]
			Timer_0_Overflow_Callback_Ptr = functionPtr;
		}
		else if(_timerMode==INTERRUPT_MODE_OUTPUT_COMPARE)
		{
			//For Output Compare Timer 0
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,1); //[OCIE0]
			Timer_0_Compare_Match_Callback_Ptr = functionPtr;
		}
		
		
		break;
		
		case  INTERRUPT_TIMER_1:
		
		if(_timerMode==INTERRUPT_MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 1
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,2); //[TOIE1]
			Timer_1_Overflow_Callback_Ptr = functionPtr;
		}
		else if(_timerMode==INTERRUPT_MODE_OUTPUT_COMPARE)
		{
			//For Output Compare Timer 1 A,B Channel
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,3); //For Channel B  [OCIE1B]
			Timer_1_Compare_Match_B_Callback_Ptr = functionPtr;
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,4); //For Channel A  [OCIE1A]
			Timer_1_Compare_Match_A_Callback_Ptr = functionPtr;
		}
		else if(_timerMode==INTERRUPT_MODE_INPUT_CAPTURE_FLAG)
		{
			//For Input Capture Flag Timer 1
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,5);  //[TICIE1]
			Timer_1_Capture_Event_Callback_Ptr = functionPtr;
		}
		
		break;
		
		case  INTERRUPT_TIMER_2:
		
		if(_timerMode==INTERRUPT_MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 2
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,6); //[TOIE2]
			Timer_2_Overflow_Callback_Ptr = functionPtr;
		}
		else if(_timerMode==INTERRUPT_MODE_OUTPUT_COMPARE)
		{
			//For Output Compare Timer 2
			
			BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,7); //[OCIE2]
			Timer_2_Compare_Match_Callback_Ptr = functionPtr;
		}
		
		break;
	}


	//Enable Global Interrupt
	GlobalInterrupt::Enable_Global_Interrupt();
}

//Disable Timer Interrupt For Timer,Mode
void TimerInterrupt::Disable_Interrupt()
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
	
	switch(_timer)
	{
		case INTERRUPT_TIMER_0:
		
		if(_timerMode==INTERRUPT_MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 0
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,0); //[TOIE0]
		}
		else if(_timerMode==INTERRUPT_MODE_OUTPUT_COMPARE)
		{
			//For Output Compare Timer 0
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,1); //[OCIE0]
		}
		
		
		break;
		
		case  INTERRUPT_TIMER_1:
		
		if(_timerMode==INTERRUPT_MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 1
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,2); //[TOIE1]
		}
		else if(_timerMode==INTERRUPT_MODE_OUTPUT_COMPARE)
		{
			//For Output Compare Timer 1 A,B Channel
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,3); //For Channel B  [OCIE1B]
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,4); //For Channel A  [OCIE1A]
		}
		else if(_timerMode==INTERRUPT_MODE_INPUT_CAPTURE_FLAG)
		{
			//For Input Capture Flag Timer 1
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,5);  //[TICIE1]
		}
		
		break;
		
		case  INTERRUPT_TIMER_2:
		
		if(_timerMode==INTERRUPT_MODE_NORMAL)
		{
			//For Normal Mode (Overflow) Timer 2
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,6); //[TOIE2]
		}
		else if(_timerMode==INTERRUPT_MODE_OUTPUT_COMPARE)
		{
			//For Output Compare Timer 2
			
			BITWISE_CLEAR_BIT(INTERRUPT_REG_TIMSK,7); //[OCIE2]
		}
		
		break;
	}
	
	//Clear Timer Interrupt Flag For Active Timer and Mode
	Clear_Interrupt_Flag(_timer,_timerMode);
	
}




//Clear Interrupt Flag For Timer , Mode
void TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER timer,INTERRUPT_MODE mode)
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
	
	switch(timer)
	{
		case INTERRUPT_TIMER_0:
		
			if(mode==INTERRUPT_MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 0 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,0); //[TOV0]
			}
			else if(mode==INTERRUPT_MODE_OUTPUT_COMPARE)
			{
				//For Output Compare Timer 0 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,1); //[OCF0]
			}
		
		
		break;
		
		case  INTERRUPT_TIMER_1:
		
			if(mode==INTERRUPT_MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 1 
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,2); //[TOV1]
			}
			else if(mode==INTERRUPT_MODE_OUTPUT_COMPARE)
			{
				//For Output Compare Timer 1 A,B Channel
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,3); //For Channel B  [OCF1B]
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,4); //For Channel A  [OCF1A]
			}
			else if(mode==INTERRUPT_MODE_INPUT_CAPTURE_FLAG)
			{
				//For Input Capture Flag Timer 1 
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,5);  //[ICF1]
			}
		
		break;
		
		case  INTERRUPT_TIMER_2:
		
			if(mode==INTERRUPT_MODE_NORMAL)
			{
				//For Normal Mode (Overflow) Timer 2
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,6); //[TOV2]
			}
			else if(mode==INTERRUPT_MODE_OUTPUT_COMPARE)
			{
				//For Output Compare Timer 2
			
				BITWISE_SET_BIT(INTERRUPT_REG_TIFR,7); //[OCF2]
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
	if(Timer_0_Overflow_Callback_Ptr!=0)
	{
		Timer_0_Overflow_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_0,INTERRUPT_MODE_NORMAL);
	}
}

//Interrupt Service Routine For Timer 1 Overflow
ISR(TIMER1_OVF_vect)
{
	if(Timer_1_Overflow_Callback_Ptr!=0)
	{
		Timer_1_Overflow_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_1,INTERRUPT_MODE_NORMAL);
	}
}

//Interrupt Service Routine For Timer 2 Overflow
ISR(TIMER2_OVF_vect)
{
	if(Timer_2_Overflow_Callback_Ptr!=0)
	{
		Timer_2_Overflow_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_2,INTERRUPT_MODE_NORMAL);
	}
}

#pragma endregion Interrupts For Overflow

#pragma region Interrupts For Compare Match CTC

//Interrupt Service Routine For Timer 0 Compare Match CTC
ISR(TIMER0_COMP_vect)
{
	if(Timer_0_Compare_Match_Callback_Ptr!=0)
	{
		Timer_0_Compare_Match_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_0,INTERRUPT_MODE_OUTPUT_COMPARE);
	}
}

//Interrupt Service Routine For Timer 1 Channel A Compare Match CTC
ISR(TIMER1_COMPA_vect)
{
	if(Timer_1_Compare_Match_A_Callback_Ptr!=0)
	{
		Timer_1_Compare_Match_A_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_1,INTERRUPT_MODE_OUTPUT_COMPARE);
	}
}

//Interrupt Service Routine For Timer 1 Channel B Compare Match CTC
ISR(TIMER1_COMPB_vect)
{
	if(Timer_1_Compare_Match_B_Callback_Ptr!=0)
	{
		Timer_1_Compare_Match_B_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_1,INTERRUPT_MODE_OUTPUT_COMPARE);
	}
}

//Interrupt Service Routine For Timer 2 Compare Match CTC
ISR(TIMER2_COMP_vect)
{
	if(Timer_2_Compare_Match_Callback_Ptr!=0)
	{
		Timer_2_Compare_Match_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_2,INTERRUPT_MODE_OUTPUT_COMPARE);
	}
}


#pragma endregion Interrupts For Overflow

#pragma region Interrupts For Capture Event [Timer1 Only]

//Interrupt Service Routine For Timer 1 Capture Event
ISR(TIMER1_CAPT_vect)
{
	if(Timer_1_Capture_Event_Callback_Ptr!=0)
	{
		Timer_1_Capture_Event_Callback_Ptr();
		TimerInterrupt::Clear_Interrupt_Flag(INTERRUPT_TIMER_1,INTERRUPT_MODE_INPUT_CAPTURE_FLAG);
	}
}


#pragma endregion Interrupts For Overflow