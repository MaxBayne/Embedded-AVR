/*

======= EEPROM Info ==========
 
 - Atmega32 has 1024 bytes of memory storage (1 KB)
 
 - Address of EEPROM Stored inside Two Registers (EEARH) (EEARL) where 2^10=1024 bytes so first 8 bits stored inside Register (EEARL) , reminder 2 bits stored inside Register (EEARH)
 
 ======= EEPROM Registers ==========
 
 - ATMEga32 Support Some Registers To Use EEPROM
 
 
 A- (EEDR) : EEPROM Data Register , its used to Hold Data For Read/Write Operations , its 8 bits Register
 B- (EEARL): EEPROM Address Register Low , its hold the Low Value of Address inside EEPROM mean 8 bits of address
 C- (EEARH): EEPROM Address Register HIGH , its hold the HIGH Value of Address inside EEPROM mean the reminder 2 bits of address
 D- (EECR) : EEPROM Control Register to control EEPROM Features like (Enable EEPROM Interrupt ,Enable Read Data , Enable Write Data )
			 EERIE (Bit3): (EEPROM Ready Interrupt Enable) set to 1 to enable otherwise set 0 to disable
			 EEMWE (Bit2): (EEPROM Master Write Enable) set to 1 to write data stored inside register (EEDR) to EEPROM Address stored inside Registers (EEARH + EEARL) , otherwise set to 0
			 EEWE  (Bit1): (EEPROM Write Enable) set to 1 to write data stored inside register (EEDR) to EEPROM Address stored inside Registers (EEARH + EEARL) , otherwise set to 0
			 EERE  (Bit0): (EEPROM Read Enable) set to 1 to Read data for EEPROM Address stored inside Registers (EEARH + EEARL) and store it inside Data Register (EEDR) , otherwise set to 0
 
 ======= EEPROM Write Operations ==========
 
 1- Wait until [EEWE (Bit1)] bit = 0 inside Register [EECR]
 2- Write the Address of EEPROM inside Register [EEARH] +[EEARL] 
 3- Write the Data inside Register [EEDR]
 4- Set [EEMWE (Bit2)] = 1 inside Register [EECR]
 5- Set [EEWE (Bit1)] = 1 inside Register [EECR]
 
 ======= EEPROM Read Operations ==========
 
 1- Wait until [EEWE (Bit1)] bit = 0 inside Register [EECR]
 2- Write the Address of EEPROM inside Register [EEARH] +[EEARL]
 3- Set [EERE (Bit0)] = 1 inside Register [EECR]
 4- Read Data From Register [EEDR] , its 8 bit mean 1 byte Register
 
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


class EEPROM
{
	private:
	
		
	public:
	
	//Constructor
	EEPROM();
	

	//Read Data Byte From EEPROM Address
	uint8 ReadByte(uint16 address);


	//Write Data Byte into EEPROM Address
	void WriteByte(uint8 data,uint16 address);
	

	//Enable EEPROM Ready Interrupt
	void Enable_Interrupt(void(* functionPtr)());
	
	//Disable EEPROM Ready Interrupt
	void Disable_Interrupt();
	
};



#endif /* EEPROM_H_ */