//#include "../Common/Registers.h"
#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "GPIO/GPIO.h"
#include "Interrupts/GlobalInterrupt.h"



//Enable Global Interrupt
void GlobalInterrupt::Enable_Global_Interrupt()
{
	/*
	-------------------------
	Enable Global Interrupt
	-------------------------

	- by set Bit7 inside Register (SREG)(Status Register) = HIGH For Enable otherwise Set Low For Disable Global Interrupt
	
	*/
	
	//Config Log 
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);


	BITWISE_SET_BIT(STATUS_REG_SREG,7);

	_log.WriteLine("Enabled Global Interrupt");
	
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
	
	//Config Log 
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);

	BITWISE_CLEAR_BIT(STATUS_REG_SREG,7);
	
	_log.WriteLine("Disabled Global Interrupt");
}

