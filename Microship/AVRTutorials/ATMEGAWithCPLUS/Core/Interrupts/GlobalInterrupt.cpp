//#include "../Common/Registers.h"
#include "Common/Common.h"
#include "Common/Registers.h"
#include "GPIO/GPIO.h"
#include "GlobalInterrupt.h"



//Enable Global Interrupt
void GlobalInterrupt::Enable_Global_Interrupt()
{
	/*
	-------------------------
	Enable Global Interrupt
	-------------------------

	- by set Bit7 inside Register (SREG)(Status Register) = HIGH For Enable otherwise Set Low For Disable Global Interrupt
	
	*/
	
	BITWISE_SET_BIT(STATUS_REG_SREG,7);
	
}

//Disable Global Interrupt
void GlobalInterrupt::Disable_Global_Interrupt()
{
	/*
	-------------------------
	Disable Global Interrupt
	-------------------------

	- by set Bit7 inside Register (SREG)(Status Register) = LOW For Enable otherwise Set Low For Disable Global Interrupt
	
	*/
	
	BITWISE_CLEAR_BIT(STATUS_REG_SREG,7);
	
}

