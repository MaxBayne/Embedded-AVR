#include "Common/Common.h"
#include "GPIO/GPIO.h"



#include "ExternalInterrupt.h"


//Initialize External Interrupt
void EXTI_Initialize(EXTI_Pin pin,EXTI_TriggerMode triggerMode,void (*CallbackFunctionPointer)(void))
{
	//Clear External Interrupt Flag
	EXTI_Clear_Interrupt_Flag(pin);
	
	//Set Trigger Mode For External Interrupt (LowLevel,AnyChange,FallingEdge,RissingEdge)
	EXTI_Set_Trigger_Mode(pin,triggerMode);
	
	//Enable External Interrupt
	EXTI_Enable_External_Interrupt(pin);

	//Enable Global Interrupt
	EXTI_Enable_Global_Interrupt();

	//Set Callback Function that Fired When Interrupt Triggered
	EXTI_Set_Callback(pin,CallbackFunctionPointer);
}

//Set External Interrupt Trigger Mode
void EXTI_Set_Trigger_Mode(EXTI_Pin pin,EXTI_TriggerMode triggerMode) 
{
		if(pin==EXTI_Pin_INT0)
	{
		/*
		* Set Trigger Method For External Interrupt (INT0)

		- Low Level OF INT0	   : Set ISC01 (Bit-1)  = 0 , ISC00 (Bit-0) = 0 inside Register (MCUCR)
		- Any Changes OF INT0  : Set ISC01 (Bit-1)  = 0 , ISC00 (Bit-0) = 1 inside Register (MCUCR)
		- Falling Edge OF INT0 : Set ISC01 (Bit-1)  = 1 , ISC00 (Bit-0) = 0 inside Register (MCUCR)
		- Rising Edge OF INT0  : Set ISC01 (Bit-1)  = 1 , ISC00 (Bit-0) = 1 inside Register (MCUCR)
		*/
		
		switch(triggerMode)
		{
			case EXTI_TriggerMode_LowLevel:
				clear_bit(EXTI_REG_MCUCR,1);	//ISC01
				clear_bit(EXTI_REG_MCUCR,0);	//ISC00
			break;
			
			case EXTI_TriggerMode_AnyChange:
				clear_bit(EXTI_REG_MCUCR,1);	//ISC01
				set_bit(EXTI_REG_MCUCR,0);		//ISC00
			break;
			
			case EXTI_TriggerMode_FallingEdge:
				set_bit(EXTI_REG_MCUCR,1);		//ISC01
				clear_bit(EXTI_REG_MCUCR,0);	//ISC00
			break;
			
			case EXTI_TriggerMode_RissingEdge:
				clear_bit(EXTI_REG_MCUCR,1);	//ISC01
				set_bit(EXTI_REG_MCUCR,0);		//ISC00
			break;
			
		}
		
	}
	else if(pin==EXTI_Pin_INT1)
	{
		/*
		* Set Trigger Method For External Interrupt (INT1)

		- Low Level OF INT1	   : Set ISC11 (Bit-3) = 0 , ISC10 (Bit-2) = 0 inside Register (MCUCR)
		- Any Changes OF INT1  : Set ISC11 (Bit-3) = 0 , ISC10 (Bit-2) = 1 inside Register (MCUCR)
		- Falling Edge OF INT1 : Set ISC11 (Bit-3) = 1 , ISC10 (Bit-2) = 0 inside Register (MCUCR)
		- Rising Edge OF INT1  : Set ISC11 (Bit-3) = 1 , ISC10 (Bit-2) = 1 inside Register (MCUCR)
		*/
		
		switch(triggerMode)
		{
			case EXTI_TriggerMode_LowLevel:
			clear_bit(EXTI_REG_MCUCR,3);	//ISC11
			clear_bit(EXTI_REG_MCUCR,2);	//ISC10
			break;
			
			case EXTI_TriggerMode_AnyChange:
			clear_bit(EXTI_REG_MCUCR,3);	//ISC11
			set_bit(EXTI_REG_MCUCR,2);		//ISC10
			break;
			
			case EXTI_TriggerMode_FallingEdge:
			set_bit(EXTI_REG_MCUCR,3);		//ISC11
			clear_bit(EXTI_REG_MCUCR,2);	//ISC10
			break;
			
			case EXTI_TriggerMode_RissingEdge:
			clear_bit(EXTI_REG_MCUCR,3);	//ISC11
			set_bit(EXTI_REG_MCUCR,2);		//ISC10
			break;
			
		}
	}
	else if(pin==EXTI_Pin_INT2)
	{
		/*
		* Set Trigger Method For External Interrupt (INT2) Not Edited !!!!!!!!!!!!!!!!!!!!!

		- Falling Edge OF INT2 : Set ISC2 (Bit-6) = 0  inside Register (MCUCSR)
		- Rising Edge OF INT2  : Set ISC2 (Bit-6) = 1  inside Register (MCUCSR)
		*/
		
		if(triggerMode==EXTI_TriggerMode_FallingEdge)
		{
			clear_bit(EXTI_REG_MCUCSR,6);		//ISC2
		}
		else if(triggerMode==EXTI_TriggerMode_RissingEdge)
		{
			set_bit(EXTI_REG_MCUCSR,6);			//ISC2
		}
		
	}
}

//Enable External Interrupt
void EXTI_Enable_External_Interrupt(EXTI_Pin pin)
{
	/*
	-----------------------------------------------------------
	Enable External Interrupt For Specific Pin (INT0,INT1,INT2)
	-----------------------------------------------------------
	- for enable External Interrupt Over (INT0) : then set (INT0) (Bit-6) with HIGH inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT1) : then set (INT1) (Bit-7) with HIGH inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT2) : then set (INT2) (Bit-5) with HIGH inside Register (GICR) (General Interrupt Control Register)
	*/
	
	switch(pin)
	{
		case EXTI_Pin_INT0:
			set_bit(EXTI_REG_GICR,6);
		break;
		
		case EXTI_Pin_INT1:
			set_bit(EXTI_REG_GICR,7);
		break;
		
		case EXTI_Pin_INT2:
			set_bit(EXTI_REG_GICR,5);
		break;
	}

}

//Disable External Interrupt
void EXTI_Disable_External_Interrupt(EXTI_Pin pin)
{
	/*
	-----------------------------------------------------------
	Disable External Interrupt For Specific Pin (INT0,INT1,INT2)
	-----------------------------------------------------------
	- for enable External Interrupt Over (INT0) : then set (INT0) (Bit-6) with LOW inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT1) : then set (INT1) (Bit-7) with LOW inside Register (GICR) (General Interrupt Control Register)
	- for enable External Interrupt Over (INT2) : then set (INT2) (Bit-5) with LOW inside Register (GICR) (General Interrupt Control Register)
	*/
	
	switch(pin)
	{
		case EXTI_Pin_INT0:
			clear_bit(EXTI_REG_GICR,6);
		break;
		
		case EXTI_Pin_INT1:
			clear_bit(EXTI_REG_GICR,7);
		break;
		
		case EXTI_Pin_INT2:
			clear_bit(EXTI_REG_GICR,5);
		break;
	}
}

//Clear External Interrupt Flag
void EXTI_Clear_Interrupt_Flag(EXTI_Pin pin)
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
	
	switch(pin)
	{
		case EXTI_Pin_INT0:
		set_bit(EXTI_REG_GIFR,6);
		break;
		
		case  EXTI_Pin_INT1:
		set_bit(EXTI_REG_GIFR,7);
		break;
		
		case  EXTI_Pin_INT2:
		set_bit(EXTI_REG_GIFR,5);
		break;
	}

}



//Set Callback Function For ISR
void EXTI_Set_Callback(EXTI_Pin pin,void (*CallbackFunctionPointer)(void))
{
	switch(pin)
	{
		case EXTI_Pin_INT0:
		INT0_CallbackFunctionPointer = CallbackFunctionPointer;
		break;
		
		case  EXTI_Pin_INT1:
		INT1_CallbackFunctionPointer = CallbackFunctionPointer;
		break;
		
		case  EXTI_Pin_INT2:
		INT2_CallbackFunctionPointer = CallbackFunctionPointer;
		break;
	}
}



//Enable Global Interrupt
void EXTI_Enable_Global_Interrupt()
{
	/*
	-------------------------
	Enable Global Interrupt
	-------------------------

	- by set Bit7 inside Register (SREG)(Status Register) = HIGH For Enable otherwise Set Low For Disable Global Interrupt
	
	*/
	
	set_bit(EXTI_REG_SREG,7);
	
}

//Disable Global Interrupt
void EXTI_Disable_Global_Interrupt()
{
	/*
	-------------------------
	Disable Global Interrupt
	-------------------------

	- by set Bit7 inside Register (SREG)(Status Register) = LOW For Enable otherwise Set Low For Disable Global Interrupt
	
	*/
	
	clear_bit(EXTI_REG_SREG,7);
	
}




//---------------------------------------------------------
//External Interrupt Services Routine From Vector Table :
//---------------------------------------------------------

//Interrupt Service Routine For External Interrupt INT0

void __vector_1()
{
	//EXTI_Clear_Interrupt_Flag(EXTI_Pin_INT0);
	INT0_CallbackFunctionPointer();
}

//Interrupt Service Routine For External Interrupt INT1
void __vector_2()
{
	//EXTI_Clear_Interrupt_Flag(EXTI_Pin_INT1);
	INT1_CallbackFunctionPointer();
}

//Interrupt Service Routine For External Interrupt INT2
void __vector_3()
{
	//EXTI_Clear_Interrupt_Flag(EXTI_Pin_INT2);
	INT2_CallbackFunctionPointer();
}





