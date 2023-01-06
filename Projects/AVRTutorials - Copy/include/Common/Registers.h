#ifndef REGISTERS_H_
#define REGISTERS_H_

////////////////////////////////////////////
//GPIO Registers
////////////////////////////////////////////

#pragma region GPIO Registers

#define IO_REG_PORTA 0x39				//Memory Address Of PINA [ATMEGA32] as HexDecimal
#define IO_REG_PORTB 0x36				//Memory Address Of PINB [ATMEGA32] as HexDecimal
#define IO_REG_PORTC 0x33				//Memory Address Of PINC [ATMEGA32] as HexDecimal
#define IO_REG_PORTD 0x30				//Memory Address Of PIND [ATMEGA32] as HexDecimal

#pragma endregion GPIO Registers

////////////////////////////////////////////
//Status Register
////////////////////////////////////////////

#pragma region Status Registers

//Bit7 : Global Interrupt Enable Flag
//Bit6 : Bit Copy Storage Flag
//Bit5 : Half Carry Flag
//Bit4 : Sign Flag
//Bit3 : Overflow Flag
//Bit2 : Negative Flag
//Bit1 : Zero Flag
//Bit0 : Carry Flag

//Address of Register (Status Register)
#define STATUS_REG_SREG *((volatile uint8*) 0x5F)

#pragma endregion Status Registers

////////////////////////////////////////////
//ADC Registers (Analog To Digital Converters)
////////////////////////////////////////////

#pragma region ADC Registers

#define ADC_REG_ADCL *((volatile uint8*) 0x24)		//Address of Register (ADC Data Register Low)
#define ADC_REG_ADCH *((volatile uint8*) 0x25)      //Address of Register (ADC Data Register High)
#define ADC_REG_ADCLH *((volatile uint16*) 0x24)    //Read Data 16 Bit From ADC Data Register LOW+High
#define ADC_REG_ADCSRA *((volatile uint8*) 0x26)	//Address of Register (ADC Control and Status Register)
#define ADC_REG_ADMUX *((volatile uint8*) 0x27)		//Address of Register (ADC Multiplexer Selection Register)
#define ADC_REG_SFIOR *((volatile uint8*) 0x50)		//Address of Register (Special Function Input/Output Register)


#pragma endregion ADC Registers

////////////////////////////////////////////
//Interrupts Registers 
////////////////////////////////////////////

#pragma region Interrupts Registers



//Address of Register (MCU Control Register)
#define INTERRUPT_REG_MCUCR *((volatile uint8*) 0x55)

//Address of Register (MCU Control and Status Register)
#define INTERRUPT_REG_MCUCSR *((volatile uint8*) 0x54)

//Address of Register (General Interrupt Control Register)
#define INTERRUPT_REG_GICR *((volatile uint8*) 0x5B)

//Address of Register (General Interrupt Flag Register)
#define INTERRUPT_REG_GIFR *((volatile uint8*) 0x5A)


//Address of Register (Timer Interrupt Flag Register)
#define INTERRUPT_REG_TIFR *((volatile uint8*) 0x58)

//Address of Register (Timer Interrupt Enable Register)
#define INTERRUPT_REG_TIMSK *((volatile uint8*) 0x59)

#pragma endregion Interrupts Registers

////////////////////////////////////////////
//EEPROM Registers
////////////////////////////////////////////

#pragma region EEPROM Registers

//Atmega32 has 1024 Bytes (1KB) EEPROM
//So it will need address of 2^10=1024 mean 10 Bits so first 8 Bit will store on Low Address Register (EEARL) and Reminder 2 will store in High Address Register (EEARH)

/*
(EEPROM Control Register)
use this Register to Control EEPROM Features like
EERIE (Bit3): (EEPROM Ready Interrupt Enable) set to 1 to enable otherwise set 0 to disable
EEMWE (Bit2): (EEPROM Master Write Enable) set to 1 to write data stored inside register (EEDR) to EEPROM Address stored inside Registers (EEARH + EEARL) , otherwise set to 0
EEWE  (Bit1): (EEPROM Write Enable) set to 1 to write data stored inside register (EEDR) to EEPROM Address stored inside Registers (EEARH + EEARL) , otherwise set to 0
EERE  (Bit0): (EEPROM Read Enable) set to 1 to Read data for EEPROM Address stored inside Registers (EEARH + EEARL) and store it inside Data Register (EEDR) , otherwise set to 0
*/
//Memory Address of Register (EEPROM Control Register) control EEPROM Features like (Enable EEPROM Interrupt ,Enable Read Data , Enable Write Data )
#define EEPROM_REG_EECR *((volatile uint8*) 0x3C)

//Memory Address of Register (EEPROM Data Register) Hold Data to be (Read/Write) inside EEPROM Address
#define EEPROM_REG_EEDR *((volatile uint8*) 0x3D)

//Memory Address of Register (EEPROM Address Register Low 8 Bits) 
#define EEPROM_REG_EEARL *((volatile uint8*) 0x3E)

//Memory Address of Register (EEPROM Address Register HIGH 2 Bits)
#define EEPROM_REG_EEARH *((volatile uint8*) 0x3F)

//Memory Address of Register (EEPROM Address Register High+Low [EEAR])
#define EEPROM_REG_EEAR EEAR 


#pragma endregion EEPROM Registers

////////////////////////////////////////////
//Timers Registers
////////////////////////////////////////////

#pragma region Timers Registers

/*
AtMega32A Timer
---------------
//Atmega32 has 3 Timers
//Timer0 : 8 Bit Register used for Timers,Counter
//Timer1 : 16 Bit Register used for Timers,Counter
//Timer2 : 8 Bit Register used for Timers Only

Timer Registers
---------------
- Registers For Hold Time Value like:
  TCNT0 : For Timer0 , its 8 Bit Register hold value from 0:255
  TCNT1 : For Timer1 , its 16 Bit Register hold value from 0:65536
  TCNT2 : For Timer2 , its 8 Bit Register hold value from 0:255

- Registers For Hold Compare Value like:
  OCR0  : For Compare With Timer0
  OCR1A : For Compare with Timer1 A Channel
  OCR1B : For Compare with Timer1 B Channel
  OCR2  : For Compare With Timer2

- Registers For Control Timers like:
  TCCR0 : For Control Timer0
  TCCR1A: For Control Timer1 A Channel
  TCCR1B: For Control Timer1 B Channel
  TCCR2 : For Control Timer2

- Registers For Interrupt Timers Like:
  TIFR  : For Control Interrupt with Timers 0,1,2

*/

//Memory Address of Register (EEPROM Control Register) control EEPROM Features like (Enable EEPROM Interrupt ,Enable Read Data , Enable Write Data )
#define TIMER_REG_TCNT0 *((volatile uint8*) 0x52)       //Timer0 Value Register
#define TIMER_REG_TCNT1L *((volatile uint8*) 0x4C)		//Timer1 Low Byte Value Register
#define TIMER_REG_TCNT1H *((volatile uint8*) 0x4D)		//Timer1 High Byte Value Register
#define TIMER_REG_TCNT2 *((volatile uint8*) 0x44)		//Timer2 Value Register

#define TIMER_REG_OCR0 *((volatile uint8*) 0x5C)		//Timer0 Output Compare Register
#define TIMER_REG_OCR1AH *((volatile uint8*) 0x4B)		//Timer1 Output Compare Register A High Byte
#define TIMER_REG_OCR1AL *((volatile uint8*) 0x4A)		//Timer1 Output Compare Register A Low Byte
#define TIMER_REG_OCR1BH *((volatile uint8*) 0x49)		//Timer1 Output Compare Register B High Byte
#define TIMER_REG_OCR1BL *((volatile uint8*) 0x48)		//Timer1 Output Compare Register B Low Byte
#define TIMER_REG_OCR2 *((volatile uint8*) 0x43)		//Timer2 Output Compare Register

#define TIMER_REG_TCCR0 *((volatile uint8*) 0x53)		//Timer0 Control Register
#define TIMER_REG_TCCR1A *((volatile uint8*) 0x4F)		//Timer1 Control Register A Channel
#define TIMER_REG_TCCR1B *((volatile uint8*) 0x4E)		//Timer1 Control Register B Channel
#define TIMER_REG_TCCR2 *((volatile uint8*) 0x45)		//Timer2 Control Register

#define TIMER_REG_ICR1H *((volatile uint8*) 0x47)		//Timer1 Input Capture Register High Byte
#define TIMER_REG_ICR1L *((volatile uint8*) 0x46)		//Timer1 Input Capture Register LOW Byte


#pragma endregion Timers Registers



////////////////////////////////////////////
//USART Registers 
////////////////////////////////////////////

#pragma region USART Registers

/*
USART (Universal Synchronize ASynchronize Receiver Transmitter)
---------------
- Serial Communications
- Support Async (not need Clock)
- Support Sync (need Clock To Work)
- Use Pins :
  RXD = PD0 (For Receive Serial Data)
  TXD = PD1 (For Transfer Serial Data)
  XCK = PB0 (Clock Source For Sync Mode Only)

USART Registers
---------------
- Registers For Hold Data Value :
  UTCNT0 : For Timer0 , its 8 Bit Register hold value from 0:255
  TCNT1 : For Timer1 , its 16 Bit Register hold value from 0:65536
  TCNT2 : For Timer2 , its 8 Bit Register hold value from 0:255

- Registers For Control USART :
  TCCR0 : For Control Timer0
  TCCR1A: For Control Timer1 A Channel
  TCCR1B: For Control Timer1 B Channel
  TCCR2 : For Control Timer2

- Registers For Interrupt Timers Like:
  TIFR  : For Control Interrupt with Timers 0,1,2

*/
#define USART_REG_UDR *((volatile uint8*) 0x2C)         //UART Data Register

#define USART_REG_UDRE *((volatile uint8*) 0x09)         //UART Data Register Enable



#define USART_REG_UBRR  *((volatile uint8*) 0x09)       //UART Baud Rate Register 16 Bits (2 Byte)
#define USART_REG_UBRRL *((volatile uint8*) 0x29)       //UART Baud Rate Register LOW
#define USART_REG_UBRRH *((volatile uint8*) 0x40)       //UART Baud Rate Register HIGH

#define USART_REG_UCSRA *((volatile uint8*) 0x2B)       //UART Control Status Register A
#define USART_REG_UCSRB *((volatile uint8*) 0x2A)       //UART Control Status Register B
#define USART_REG_UCSRC *((volatile uint8*) 0x00)       //UART Control Status Register C //40



#define USART_REG_UDRIE *((volatile uint8*) 0x5)       //UART Data Register Interrupt Enable



#pragma endregion USART Registers












#endif /* REGISTERS_H_ */