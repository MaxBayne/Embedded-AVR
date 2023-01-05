/*

################### External Interrupt ####################

-----------------------
External Interrupt Pins
-----------------------

ATMEGA32A has 3 Pin For External Interrupts

INT0: PD2 (16) = Port D Pin 2
INT1: PD3 (17) = Port D pin 3
INT2 PB2 (3)   =  Port B pin 2


---------------------------------------------
Control Trigger Method For External Interrupt
---------------------------------------------

* Triggers Methods That Generate Interrupt Request :

- Low Level
- Any Changes
- Falling Edge
- Rising Edge

* Set Trigger Method For External Interrupt Pins by below :

- Set Trigger Method For External Interrupt Over Pin (INT0) by using bit0 (ISC00)  , bit1 (ISC01) inside Register (MCUCR) (MCU Control Register)
- Set Trigger Method For External Interrupt Over Pin (INT1) by using bit2 (ISC10)  , bit3 (ISC11) inside Register (MCUCR) (MCU Control Register)
- Set Trigger Method For External Interrupt Over Pin (INT2) by using bit6 (ISC2 inside Register (MCUCSR) (MCU Control And Status Register)

* Set Trigger Method For External Interrupt (INT0)

- Low Level OF INT0	   : Set ISC01 (Bit-1)  = 0 , ISC00 (Bit-0) = 0 inside Register (MCUCR)
- Any Changes OF INT0  : Set ISC01 (Bit-1)  = 0 , ISC00 (Bit-0) = 1 inside Register (MCUCR)
- Falling Edge OF INT0 : Set ISC01 (Bit-1)  = 1 , ISC00 (Bit-0) = 0 inside Register (MCUCR)
- Rising Edge OF INT0  : Set ISC01 (Bit-1)  = 1 , ISC00 (Bit-0) = 1 inside Register (MCUCR)

* Set Trigger Method For External Interrupt (INT1)

- Low Level OF INT1	   : Set ISC11 (Bit-3) = 0 , ISC10 (Bit-2) = 0 inside Register (MCUCR)
- Any Changes OF INT1  : Set ISC11 (Bit-3) = 0 , ISC10 (Bit-2) = 1 inside Register (MCUCR)
- Falling Edge OF INT1 : Set ISC11 (Bit-3) = 1 , ISC10 (Bit-2) = 0 inside Register (MCUCR)
- Rising Edge OF INT1  : Set ISC11 (Bit-3) = 1 , ISC10 (Bit-2) = 1 inside Register (MCUCR)


* Set Trigger Method For External Interrupt (INT2) Not Edited !!!!!!!!!!!!!!!!!!!!!

- Falling Edge OF INT2 : Set ISC2 (Bit-6) = 0  inside Register (MCUCSR)
- Rising Edge OF INT2  : Set ISC2 (Bit-6) = 1  inside Register (MCUCSR)



-------------------------
Enable Global Interrupt 
-------------------------

- by set Bit7 inside Register (SREG)(Status Register) = HIGH For Enable otherwise Set Low For Disable Global Interrupt


-----------------------------------------------------------
Enable External Interrupt For Specific Pin (INT0,INT1,INT2)
-----------------------------------------------------------

- for enable External Interrupt Over (INT0) : then set (INT0) (Bit-6) with HIGH inside Register (GICR) (General Interrupt Control Register)
- for enable External Interrupt Over (INT1) : then set (INT1) (Bit-7) with HIGH inside Register (GICR) (General Interrupt Control Register)
- for enable External Interrupt Over (INT2) : then set (INT2) (Bit-5) with HIGH inside Register (GICR) (General Interrupt Control Register)

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

Example: --------------------------------------

void toggleLEDYellow()
{
	GPIO::Pin_Toggle(GPIO_IO_PA0);
}

int main(void)
{
	
	GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);
	GPIO::Port_Direction(GPIO_PORT_B,GPIO_DIRECTION_OUTPUT);
	
	GPIO::Pin_Set(GPIO_IO_PD2);
	
	ExternalInterrupt ExternalINT0(EXTI_PIN_INT0,EXTI_TRIGGER_MODE_FALLING_EDGE,&toggleLEDYellow);
	

	
	uint8 counter=0;
	while (1)
	{
		counter++;
		GPIO::Port_Write(GPIO_PORT_B,counter);
		delay_ms(1000);
	}
}



*/

#ifndef EXTERNALINTERRUPT_H_
#define EXTERNALINTERRUPT_H_


//----------------------------------------
//Define ENUMS
//----------------------------------------

#pragma region Enums


typedef enum
{
	EXTI_PIN_INT0,
	EXTI_PIN_INT1,
	EXTI_PIN_INT2
}EXTI_PIN;

typedef enum
{
	//Every Time Pin has Low Value then Generate External Interrupt Request
	EXTI_TRIGGER_MODE_LOW_LEVEL,
	
	//On Status Be Falling Edge or Be Rising Edge then Generate External Interrupt Request
	EXTI_TRIGGER_MODE_ANY_CHANGE,
	
	//On Status Goes From Rising To Falling Edge then Generate External Interrupt Request
	EXTI_TRIGGER_MODE_FALLING_EDGE,
	
	//On Status Goes From Falling To Rising Edge then Generate External Interrupt Request
	EXTI_TRIGGER_MODE_RISSING_EDGE
}EXTI_TRIGGER_MODE;


#pragma endregion Enums



class ExternalInterrupt
{
	private:
	
	//Interrupt Pin 
	EXTI_PIN _pin; 
	
	//Trigger Mode LowLeve , AnyChange , Falling , Rising Edge
	EXTI_TRIGGER_MODE _triggerMode;
	
	Logs* _log;
	
	public:
	
	//Constructor
	ExternalInterrupt(EXTI_PIN pin,EXTI_TRIGGER_MODE triggerMode,void (*functionPtr)());
	ExternalInterrupt(EXTI_PIN pin,EXTI_TRIGGER_MODE triggerMode,void (*functionPtr)(),Logs* log);
	
	private:
	
	//Set External Interrupt Trigger Mode
	void Set_Trigger_Mode();
	
	//Enable External Interrupt
	void Enable_External_Interrupt(void(* functionPtr)());
	
	//Disable External Interrupt
	void Disable_External_Interrupt();
	
	//Clear External Interrupt Flag
	void Clear_Interrupt_Flag();
	
};


#endif /* EXTERNALINTERRUPT_H_ */