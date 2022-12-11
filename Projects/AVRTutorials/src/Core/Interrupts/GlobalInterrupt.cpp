#include "avr/interrupt.h"
#include "Common/Common.h"
#include "Common/Registers.h"
#include "Common/Logs.h"
#include "GPIO/GPIO.h"
#include "Interrupts/GlobalInterrupt.h"

//#define ENABLE_LOGS_GLOBAL_INTERRUPT

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
	#ifdef ENABLE_LOGS_GLOBAL_INTERRUPT
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);
	#endif


	BITWISE_SET_BIT(STATUS_REG_SREG,SREG_I); //7
	//sei();

	#ifdef ENABLE_LOGS_GLOBAL_INTERRUPT
	_log.WriteLine("Enabled Global Interrupt");
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
	
	//Config Log 
	#ifdef ENABLE_LOGS_GLOBAL_INTERRUPT
	Logs _log=Logs(); 
	_log.Initialize(LOGS_SOURCE_UART0);
	#endif

	BITWISE_CLEAR_BIT(STATUS_REG_SREG,SREG_I); //7
	//cli();

	#ifdef ENABLE_LOGS_GLOBAL_INTERRUPT
	_log.WriteLine("Disabled Global Interrupt");
	#endif
}

