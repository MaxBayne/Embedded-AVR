#include "Common/Common.h"
#include "Common/Registers.h"
#include "GPIO/GPIO.h"
#include "avr/interrupt.h"
#include "Interrupts/GlobalInterrupt.h"
#include "EEPROM/EEPROM.h"


//Function Pointer For EEPROM Ready Callback Delegates
void(*EEPROM_Callback_Ptr)();

//Constructor
EEPROM::EEPROM(){}



//Read Data Byte From EEPROM Address
uint8 EEPROM::ReadByte(uint16 address)
{
	//======= EEPROM Read Operations ==========
	
	//1- Wait until [EEWE (Bit1)] bit = 0 inside Register [EECR]
	while(BITWISE_GET_BIT(EEPROM_REG_EECR,1));
	
	//2- Write the Address of EEPROM inside Register [EEARH] +[EEARL]
	//EEPROM_REG_EEARL = BITWISE_GET_LOW_BYTE(address);	//Get first 8 Low Bits
	//EEPROM_REG_EEARH = BITWISE_GET_HIGH_BYTE(address);	//Get Second 8 High Bits using Bitwise Right Shift
	EEPROM_REG_EEAR = address;
	
	//3- Execute Read Operation by Set [EERE (Bit0)] = 1 inside Register [EECR]
	BITWISE_SET_BIT(EEPROM_REG_EECR,0);
	
	//4- Read Data From Register [EEDR] , its 8 bit mean 1 byte Register
	return EEPROM_REG_EEDR;
}



//Write Data Byte into EEPROM Address
void EEPROM::WriteByte(uint8 data,uint16 address)
{
	//======= EEPROM Write Operations ==========
	
	//1- Wait for last write operation to finish mean wait until [EEWE (Bit1)] bit = 0 inside Register [EECR]
	while(BITWISE_GET_BIT(EEPROM_REG_EECR,1));
	
	//2- Write the Address of EEPROM inside Register [EEARH] +[EEARL]
	//EEPROM_REG_EEARL = BITWISE_GET_LOW_BYTE(address);	//Get first 8 Low Bits
	//EEPROM_REG_EEARH = BITWISE_GET_HIGH_BYTE(address);	//Get Second 8 High Bits using Bitwise Right Shift
	EEPROM_REG_EEAR = address;
	
	//3- Write the Data inside Register [EEDR]
	EEPROM_REG_EEDR = data;
	
	//4- Execute the Write Operation by Set [EEMWE (Bit2)] = 1 inside Register [EECR]
	BITWISE_SET_BIT(EEPROM_REG_EECR,2);
	
	//5- Execute the Write Operation by Set [EEWE (Bit1)] = 1 inside Register [EECR]
	BITWISE_SET_BIT(EEPROM_REG_EECR,1);
}


//Enable EEPROM Ready Interrupt
void EEPROM::Enable_Interrupt(void(* functionPtr)())
{
	
	//-----------------------------
	//Enable EEPROM Ready Interrupt
	//-----------------------------
	//- for enable EEPROM READY Interrupt  : then set (EERIE) (Bit-3) with HIGH inside Register (EECR)
	
	
	//Save EEPROM Interrupt CAllback Function
	EEPROM_Callback_Ptr = functionPtr;
	
	//Enable EEPROM READY Interrupt
	BITWISE_SET_BIT(EEPROM_REG_EECR,3);

}
	
//Disable EEPROM Ready Interrupt
void EEPROM::Disable_Interrupt()
{
	//Clear Function Pointer
	EEPROM_Callback_Ptr=0;
	
	//Disable EEPROM READY Interrupt
	BITWISE_CLEAR_BIT(EEPROM_REG_EECR,3);
}


//---------------------------------------------------------
//EEPROM Interrupt Services Routine From Vector Table No (18):
//---------------------------------------------------------

ISR(EE_RDY_vect)
{
	if(EEPROM_Callback_Ptr!=0)
	{
		EEPROM_Callback_Ptr();
	}
}
