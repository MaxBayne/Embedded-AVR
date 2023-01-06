#include "avr/interrupt.h"
#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "GPIO/GPIO.h"
#include "Interrupts/GlobalInterrupt.h"

//#define ENABLE_LOGS_GLOBAL_INTERRUPT

GlobalInterrupt::GlobalInterrupt()
{
	_log=nullptr;
}
GlobalInterrupt::GlobalInterrupt(Logs* log)
{
	_log=log;
}

//Enable Global Interrupt
void GlobalInterrupt::Enable_Global_Interrupt()
{
	/*
	-------------------------
	Enable Global Interrupt
	-------------------------

	- by set Bit7 inside Register (SREG)(Status Register) = HIGH For Enable otherwise Set Low For Disable Global Interrupt
	
	*/
	

	BITWISE_SET_BIT(STATUS_REG_SREG,SREG_I); //7
	//sei();

	#ifdef ENABLE_LOGS_GLOBAL_INTERRUPT
	if(_log!=NULL)
	{
		_log.WriteLine("Enabled Global Interrupt");
	}
	#endif
	
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
	
	BITWISE_CLEAR_BIT(STATUS_REG_SREG,SREG_I); //7
	//cli();

	#ifdef ENABLE_LOGS_GLOBAL_INTERRUPT
	if(_log!=NULL)
		_log.WriteLine("Disabled Global Interrupt");
	#endif
}

