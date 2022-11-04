#include "Common/Common.h"
#include "Common/Registers.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"

#include "Interrupts/GlobalInterrupt.h"
#include "Interrupts/ExternalInterrupt.h"

//Function Pointer For INT Callback Delegates
void(*INT0_Callback_Ptr)();
void(*INT1_Callback_Ptr)();
void(*INT2_Callback_Ptr)();


//Constructor
ExternalInterrupt::ExternalInterrupt(EXTI_PIN pin,EXTI_TRIGGER_MODE triggerMode,void(*functionPtr)())
{
	_pin=pin;
	_triggerMode=triggerMode;
	
	//Clear External Interrupt Flag
	Clear_Interrupt_Flag();
	
	//Set Trigger Mode For External Interrupt (LowLevel,AnyChange,FallingEdge,RissingEdge)
	Set_Trigger_Mode();
	
	//Enable External Interrupt
	Enable_External_Interrupt(functionPtr);

	//Enable Global Interrupt
	GlobalInterrupt::Enable_Global_Interrupt();
}


//Set External Interrupt Trigger Mode
void ExternalInterrupt::Set_Trigger_Mode() 
{
	if(_pin==EXTI_PIN_INT0)
	{
		/*
		* Set Trigger Method For External Interrupt (INT0)

		- Low Level OF INT0	   : Set ISC01 (Bit-1)  = 0 , ISC00 (Bit-0) = 0 inside Register (MCUCR)
		- Any Changes OF INT0  : Set ISC01 (Bit-1)  = 0 , ISC00 (Bit-0) = 1 inside Register (MCUCR)
		- Falling Edge OF INT0 : Set ISC01 (Bit-1)  = 1 , ISC00 (Bit-0) = 0 inside Register (MCUCR)
		- Rising Edge OF INT0  : Set ISC01 (Bit-1)  = 1 , ISC00 (Bit-0) = 1 inside Register (MCUCR)
		*/
		
		switch(_triggerMode)
		{
			case EXTI_TRIGGER_MODE_LOW_LEVEL:
				BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,1);	//ISC01
				BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,0);	//ISC00
			break;
			
			case EXTI_TRIGGER_MODE_ANY_CHANGE:
				BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,1);	//ISC01
				BITWISE_SET_BIT(INTERRUPT_REG_MCUCR,0);		//ISC00
			break;
			
			case EXTI_TRIGGER_MODE_FALLING_EDGE:
				BITWISE_SET_BIT(INTERRUPT_REG_MCUCR,1);		//ISC01
				BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,0);	//ISC00
			break;
			
			case EXTI_TRIGGER_MODE_RISSING_EDGE:
				BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,1);	//ISC01
				BITWISE_SET_BIT(INTERRUPT_REG_MCUCR,0);		//ISC00
			break;
			
		}
		
	}
	else if(_pin==EXTI_PIN_INT1)
	{
		/*
		* Set Trigger Method For External Interrupt (INT1)

		- Low Level OF INT1	   : Set ISC11 (Bit-3) = 0 , ISC10 (Bit-2) = 0 inside Register (MCUCR)
		- Any Changes OF INT1  : Set ISC11 (Bit-3) = 0 , ISC10 (Bit-2) = 1 inside Register (MCUCR)
		- Falling Edge OF INT1 : Set ISC11 (Bit-3) = 1 , ISC10 (Bit-2) = 0 inside Register (MCUCR)
		- Rising Edge OF INT1  : Set ISC11 (Bit-3) = 1 , ISC10 (Bit-2) = 1 inside Register (MCUCR)
		*/
		
		switch(_triggerMode)
		{
			case EXTI_TRIGGER_MODE_LOW_LEVEL:
			BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,3);	//ISC11
			BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,2);	//ISC10
			break;
			
			case EXTI_TRIGGER_MODE_ANY_CHANGE:
			BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,3);	//ISC11
			BITWISE_SET_BIT(INTERRUPT_REG_MCUCR,2);		//ISC10
			break;
			
			case EXTI_TRIGGER_MODE_FALLING_EDGE:
			BITWISE_SET_BIT(INTERRUPT_REG_MCUCR,3);		//ISC11
			BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,2);	//ISC10
			break;
			
			case EXTI_TRIGGER_MODE_RISSING_EDGE:
			BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCR,3);	//ISC11
			BITWISE_SET_BIT(INTERRUPT_REG_MCUCR,2);		//ISC10
			break;
			
		}
	}
	else if(_pin==EXTI_PIN_INT2)
	{
		/*
		* Set Trigger Method For External Interrupt (INT2) Not Edited !!!!!!!!!!!!!!!!!!!!!

		- Falling Edge OF INT2 : Set ISC2 (Bit-6) = 0  inside Register (MCUCSR)
		- Rising Edge OF INT2  : Set ISC2 (Bit-6) = 1  inside Register (MCUCSR)
		*/
		
		if(_triggerMode==EXTI_TRIGGER_MODE_FALLING_EDGE)
		{
			BITWISE_CLEAR_BIT(INTERRUPT_REG_MCUCSR,6);		//ISC2
		}
		else if(_triggerMode==EXTI_TRIGGER_MODE_RISSING_EDGE)
		{
			BITWISE_SET_BIT(INTERRUPT_REG_MCUCSR,6);			//ISC2
		}
		
	}
}

//Enable External Interrupt
void ExternalInterrupt::Enable_External_Interrupt(void(* functionPtr)())
{
	/*
	-----------------------------------------------------------
	Enable External Interrupt For Specific Pin (INT0,INT1,INT2)
	-----------------------------------------------------------
	- for enable External Interrupt Over (INT0) : then set (INT0) (Bit-6) with HIGH inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT1) : then set (INT1) (Bit-7) with HIGH inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT2) : then set (INT2) (Bit-5) with HIGH inside Register (GICR) (General Interrupt Control Register)
	*/
	
	switch(_pin)
	{
		case EXTI_PIN_INT0:
			INT0_Callback_Ptr = functionPtr;
			BITWISE_SET_BIT(INTERRUPT_REG_GICR,6);
		break;
		
		case EXTI_PIN_INT1:
			INT1_Callback_Ptr = functionPtr;
			BITWISE_SET_BIT(INTERRUPT_REG_GICR,7);
		break;
		
		case EXTI_PIN_INT2:
			INT2_Callback_Ptr = functionPtr;
			BITWISE_SET_BIT(INTERRUPT_REG_GICR,5);
		break;
	}

}

//Disable External Interrupt
void ExternalInterrupt::Disable_External_Interrupt()
{
	/*
	-----------------------------------------------------------
	Disable External Interrupt For Specific Pin (INT0,INT1,INT2)
	-----------------------------------------------------------
	- for enable External Interrupt Over (INT0) : then set (INT0) (Bit-6) with LOW inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT1) : then set (INT1) (Bit-7) with LOW inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT2) : then set (INT2) (Bit-5) with LOW inside Register (GICR) (General Interrupt Control Register)
	*/
	
	switch(_pin)
	{
		case EXTI_PIN_INT0:
			BITWISE_CLEAR_BIT(INTERRUPT_REG_GICR,6);
		break;
		
		case EXTI_PIN_INT1:
			BITWISE_CLEAR_BIT(INTERRUPT_REG_GICR,7);
		break;
		
		case EXTI_PIN_INT2:
			BITWISE_CLEAR_BIT(INTERRUPT_REG_GICR,5);
		break;
	}
}

//Clear External Interrupt Flag
void ExternalInterrupt::Clear_Interrupt_Flag()
{
	/*
	-----------------------
	External Interrupt Flag
	-----------------------

	- Interrupt Flag is Something Say that interrupt request generated mean Trigger Fired For Interrupt
	- to read the Status of Flag Interrupt we use Register (GIFR) (General Interrupt Flag Register)
	- Get External Interrupt Flag For INT0 we use INTF0 (Bit-6) inside Register (GIFR)
	- Get External Interrupt Flag For INT1 we use INTF1 (Bit-7) inside Register (GIFR)
	- Get External Interrupt Flag For INT2 we use INTF2 (Bit-5) inside Register (GIFR)

	- when External Interrupt Triggered over Pin (INT0,INT1,INT2) then Interrupt Flag Become 1 and Processor Jump TO Vector Table and Get the Interrupt Service Routine For That
	Interrupt and Execute it and when Finished then Clear Interrupt Flag inside Register (GIFR) to Avoid ReEnter the Service Routine Again

	*/
	
	switch(_pin)
	{
		case EXTI_PIN_INT0:
		BITWISE_SET_BIT(INTERRUPT_REG_GIFR,6);
		break;
		
		case  EXTI_PIN_INT1:
		BITWISE_SET_BIT(INTERRUPT_REG_GIFR,7);
		break;
		
		case  EXTI_PIN_INT2:
		BITWISE_SET_BIT(INTERRUPT_REG_GIFR,5);
		break;
	}

}





//---------------------------------------------------------
//External Interrupt Services Routine From Vector Table :
//---------------------------------------------------------

//Interrupt Service Routine For External Interrupt INT0
ISR(INT0_vect)
{
	if(INT0_Callback_Ptr!=0)
	{
		INT0_Callback_Ptr();	
	}
}

//Interrupt Service Routine For External Interrupt INT1
ISR(INT1_vect)
{
	if(INT1_Callback_Ptr!=0)
	{
		INT1_Callback_Ptr();
	}
}

//Interrupt Service Routine For External Interrupt INT2
ISR(INT2_vect)
{
	if(INT2_Callback_Ptr!=0)
	{
		INT2_Callback_Ptr();
	}
}
