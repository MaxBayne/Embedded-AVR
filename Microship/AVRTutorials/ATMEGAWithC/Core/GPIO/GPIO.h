#ifndef GPIO_H_
#define GPIO_H_

/*
AVR Registers ------------------------
DDR : this register used to set Direction of Port (Input/Output) like : [DDRA,DDRB,DDRC,DDRD] so set 1 mean output , set 0 mean input ,every pin take this example [DDA0:DDA7]
PORT: this register used to write low/high for pins set 1 for high , set 0 for low like [PORTA,PORTB,PORTC,PORTD] ,every pin take this example [PORTA0:PORTA7]
PIN : this register used to read pin Input status where 0 mean low , 1 mean high like [PINA,PINB,PINC,PIND] , every pin take this example [PINA0:PINA7]
*/


//enum Definitions -------------------------------------------------

//Define The Port ID
typedef enum
{
	PortA,
	PortB,
	PortC,
	PortD
}PORT;

//Define The Pin ID
typedef enum
{
	Pin0,
	Pin1,
	Pin2,
	Pin3,
	Pin4,
	Pin5,
	Pin6,
	Pin7
}PIN;

//Define the Data Direction
typedef enum
{
	INPUT,
	OUTPUT,
	PULLUP
}DIRECTION;

//Define the Data Status
typedef enum
{
	LOW,
	HIGH,
	UnKnown,
	TOGGLE
} VALUE;



//Pins Address  -------------------------------------------------

#define PORTA_ADDRESS 0x39				//Address Of PINA [ATMEGA32] as HexDecimal
#define PORTB_ADDRESS 0x36				//Address Of PINB [ATMEGA32] as HexDecimal
#define PORTC_ADDRESS 0x33				//Address Of PINC [ATMEGA32] as HexDecimal
#define PORTD_ADDRESS 0x30				//Address Of PIND [ATMEGA32] as HexDecimal

//Bits Operations ---------------------------------------------------

//Set 1 inside Bit For Register
#define set_bit(register,bitNumber) register |= (1<<bitNumber)

//Set 0 inside Bit For Register
#define clear_bit(register,bitNumber) register &= ~(1<<bitNumber)

//Toggle Bit Value For Register
#define toggle_bit(register,bitNumber) register ^= (1<<bitNumber)

//Read Bit Value 1 or 0
#define get_bit(register,bitNumber) ((register>>bitNumber) & 0x01)

//Port Operations ---------------------------------------------------

//Set 1 inside Port For Register
#define set_port(register) register = 0xff

//Set 0 inside Port For Register
#define clear_port(register) register = 0x00

//Toggle Bit Value For Register
#define toggle_port(register) register ^= 0xff


//Methods Operations ---------------------------------------------------

//Set Direction Of Port INPUT/OUTPUT
void Port_Direction(PORT port,DIRECTION direction);

//Write Value over Port pins
void Port_Write_Value(PORT port,unsigned char value);

//Write High/LOW/Toggle Over Port pins
void Port_Write(PORT port,VALUE value);

//Read Port Value (Byte)
char Port_Read(PORT port);



//Set Direction Of Pin INPUT/OUTPUT
void Pin_Direction(PORT port,PIN pin,DIRECTION direction);

//Write High/LOW/Toggle Over pin
void Pin_Write(PORT port,PIN pin,VALUE value);

//Read Pin Value (HIGH/LOW)
VALUE Pin_Read(PORT port,PIN pin);





#endif /* GPIO_H_ */